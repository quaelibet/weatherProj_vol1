#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_showData1_clicked();
    void replyFinished1(QNetworkReply* reply);
    void replyFinished2(QNetworkReply* reply);

    void on_showData2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
