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
    //QList<QStringList> processReply(QNetworkReply* reply);
    QStringList processReply(QNetworkReply* reply);
    void assignIndex(QString header, int* dateId, int* maxTempId, int* avgTempId, int* minTempId);
    void assignData(QStringList allDataList, QList<float>* maxTemp, QList<float>* avgTemp, QList<float>* minTemp, QList<QDate>* date, int dateId, int maxTempId, int avgTempId, int minTempId);
    
private slots:
    void on_showData1_clicked();
    void replyFinished1(QNetworkReply* reply);
    void replyFinished2(QNetworkReply* reply);

    void on_showData2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
