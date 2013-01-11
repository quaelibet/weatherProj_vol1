#include <QApplication>
#include "mainwindow.h"
#include "qcustomplot.h"

/**
 *
 * \mainpage %Weather Project Documentation
 *
 * @author Dorota Tomalska
 *
 * Programowanie w Systemie Linux [SSM]
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
