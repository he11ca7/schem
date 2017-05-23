#include <QApplication>
#include <QDir>
#include <QStyleFactory>
//#include <QTextCodec>
#include "globals.h"
#include "window.h"

int main(
    int argc,
    char *argv[])
{
  //  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
  //  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  //  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

  QApplication a(argc, argv);
  QApplication::setStyle(QStyleFactory::create("fusion")); // fusion windowsxp

  QDir current = QDir(QCoreApplication::applicationDirPath());
  current.cdUp();

  G_PATH = current.absolutePath();

  (new Window)->show();

  return a.exec();
}
