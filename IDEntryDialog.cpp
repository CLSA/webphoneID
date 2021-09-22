#include "IDEntryDialog.h"

#include "ui_IDEntryDialog.h"

#include <curl/curl.h>

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>

#ifdef WITH_QDEBUG
  #include <QDebug>
#endif

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  reinterpret_cast<QByteArray*>(userp)->append( reinterpret_cast<char*>(contents), size * nmemb );
  return size * nmemb;
}

IDEntryDialog::IDEntryDialog(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::IDEntryDialog)
{
  ui->setupUi(this);
  ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

  connect(ui->usernameLineEdit,&QLineEdit::editingFinished,
      this,[this](){
     m_username = ui->usernameLineEdit->text();
  });
  connect(ui->passwordLineEdit,&QLineEdit::editingFinished,
      this,[this](){
     m_password = ui->passwordLineEdit->text();
  });
  QPushButton* okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
  QPushButton* cancelButton = ui->buttonBox->button(QDialogButtonBox::Cancel );

  // read settings from .ini file
  //
  QDir dir = QCoreApplication::applicationDirPath();
  QSettings settings(dir.filePath("settings.local.ini"), QSettings::IniFormat);
  m_settingsMap["cenozoUrl"] = settings.value("cenozo/url").toString();
  settings.beginGroup("webphone");
  for(QString key : settings.childKeys())
  {
      m_settingsMap[key] = settings.value(key).toString();
  }
  settings.endGroup();

  connect(okButton,&QPushButton::clicked,
      this,[this](){
      if(m_username.isEmpty() || m_password.isEmpty())
      {
        QMessageBox::warning(
          this,
          "WARNING",
          "Incomplete user credentials"
        );
      }
      else
      {
        // get the user's ID from the server
        int error = 0;
        CURL *curl;
        CURLcode curlResponse;
        QByteArray readBuffer;

        curl = curl_easy_init();
        if( !curl ) error = 1;
        else
        {
          // get the user ID from the server
          QString userPass = QString("%1:%2").arg(m_username, m_password);
          QString auth = "Authorization: Basic ";
          auth.append(userPass.toUtf8().toBase64());

#ifdef WITH_QDEBUG
          qDebug() << "authorization string: " << auth;
#endif

          long responseCode = 0;
          char errbuf[CURL_ERROR_SIZE];
          struct curl_slist *list = NULL;
          list = curl_slist_append( list, auth.toStdString().c_str() );
          list = curl_slist_append( list, "No-Activity:true" );

          curl_easy_setopt( curl, CURLOPT_ERRORBUFFER, errbuf );
          curl_easy_setopt( curl, CURLOPT_URL, m_settingsMap["cenozoUrl"].toStdString().c_str() );
          curl_easy_setopt( curl, CURLOPT_HTTPHEADER, list );
          curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 0 );
          curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteCallback );
          curl_easy_setopt( curl, CURLOPT_WRITEDATA, &readBuffer );
          curlResponse = curl_easy_perform( curl );
          curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &responseCode );
          curl_slist_free_all( list );
          curl_easy_cleanup( curl );

          if( 0 < curlResponse ) error = 2;
          else if( 200 < responseCode ) error = 3;

          if( 0 < error )
          {
            QMessageBox::warning(
              this,
              "ERROR",
              1 == error ? "Unable to open connection to remote server." :
              2 == error ? "Failed to connect to remote server." :
              "Invalid username and/or password."
            );
          }
          else
          {
            // parse the JSON response from the server and pull out the ID

            QJsonParseError jsonErr;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(readBuffer,&jsonErr);
            QJsonObject jsonObj = jsonDoc.object();

#ifdef WITH_QDEBUG
            qDebug() << "readbuffer: " << readBuffer;
            qDebug() << "as compact json: " << jsonDoc.toJson(QJsonDocument::Indented);
#endif

            if( QJsonParseError::NoError != jsonErr.error ||
                !jsonObj.contains( "user" ) ||
                !jsonObj["user"].toObject().contains( "id" ) )
            {
              QMessageBox::warning( this, "ERROR", "Invalid response from server." );
            }
            else
            {
              close();

              // launch the webphone

              int userID = jsonObj["user"].toObject().value("id").toInt() + 10000000;
              QStringList commandStream;
              commandStream
                << "java -jar webphone.jar"
                << QString("username:").append(QString::number(userID));

              for(QString key : m_settingsMap.keys() )
              {
                  if("cenozoUrl" == key) continue;
                  if("codec" == key || "mayscript" == key)
                    commandStream << QString("%1:\"%2\"").arg(key,m_settingsMap[key]);
                  else
                    commandStream << QString("%1:%2").arg(key,m_settingsMap[key]);
              }

#ifdef WITH_QDEBUG
              qDebug() << "system command: " << commandStream.join(" ");
#endif
              system( commandStream.join(" ").toStdString().c_str() );
            }
          }
        }
      }
    }
  );
  connect(cancelButton,&QPushButton::clicked,
      this,[this](){
    m_username.clear();
    m_password.clear();
    close();
    }
  );
}

IDEntryDialog::~IDEntryDialog()
{
  delete ui;
}
