#include <QtGui/QApplication>
#include "mainwindow.h"
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w(&a, argv[1]);
    w.show();

    return a.exec();
}
