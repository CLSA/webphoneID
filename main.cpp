#include "IDEntryDialog.h"

#include <QApplication>
#include <QTextStream>
#include <QMessageBox>

int main(int argc, char *argv[])
{
  QCoreApplication::setOrganizationName( "CLSA" );
  QCoreApplication::setOrganizationDomain( "clsa-elcv.ca" );
  QCoreApplication::setApplicationName( "WebPhone User ID" );

  QApplication app(argc, argv);

  IDEntryDialog dialog;
  dialog.show();
  app.exec();
  return dialog.getSystemReturnValue();
}
