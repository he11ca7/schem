#include <QApplication>
#include <QDir>
#include <QStyleFactory>
#include "globals.h"
#include "window.h"

int main(
    int argc,
    char *argv[])
{
  QApplication a(argc, argv);
  QApplication::setStyle(QStyleFactory::create("fusion")); // NOTE fusion windowsxp

  QDir current = QDir(QCoreApplication::applicationDirPath());
  current.cdUp();

  G_PATH = current.absolutePath();

  (new Window)->show();

  return a.exec();
}
