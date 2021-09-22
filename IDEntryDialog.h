#ifndef IDENTRYDIALOG_H
#define IDENTRYDIALOG_H

#include <QDialog>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class IDEntryDialog; }
QT_END_NAMESPACE

class IDEntryDialog : public QDialog
{
    Q_OBJECT

public:
    IDEntryDialog(QWidget *parent = nullptr);
    ~IDEntryDialog();

    QString getName() const {return m_username;}
    QString getPassword() const {return m_password;}

private:
    Ui::IDEntryDialog *ui;
    QString m_username;
    QString m_password;
    QMap<QString,QString> m_settingsMap;
};
#endif // IDENTRYDIALOG_H
