#include "mainwindow.h"
#include <QDebug>
#include <QTextCodec>
#include <QTranslator>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
  QTextCodec *codec = QTextCodec::codecForName("utf-8");
  QTextCodec::setCodecForLocale(codec);

  QApplication app(argc, argv);

  QTranslator *translator = new QTranslator(&app);
  QLocale locale;
  QString translatorFileName = "";
  qDebug() << "locale.country" << locale.country()
           << ",china=" << QLocale::China;
  switch (locale.country()) {
  case QLocale::China:
  case QLocale::Taiwan:
  case QLocale::HongKong:
    translatorFileName = ":/i18n/zh_CN.qm";
    break;
  default:
    translatorFileName = "";
  }
  if (translatorFileName != "") {
    if (translator->load(translatorFileName)) {
      app.installTranslator(translator);
    }
  }

  /*
  app.connect( &app,
             SIGNAL(lastWindowClosed()),
             &app,
             SLOT(quit()));

*/

  MainWindow w(&app);

  return app.exec();
}
