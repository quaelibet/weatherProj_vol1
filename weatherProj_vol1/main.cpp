#include <QApplication>
#include "mainwindow.h"
#include "qcustomplot.h"

/**
 *
 * \mainpage %Weather Project Documentation
 *
 *
 *
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Weather Project");
    w.show();
    
    return a.exec();
}
