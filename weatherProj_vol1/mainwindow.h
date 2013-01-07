#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include "qcustomplot.h"

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
    //void assignIndex(QString header, int* dateId, int* avgTempId, int* minTempId);
    void assignIndex(QString header, int*dateId, int*avgTempId, int*avgDewId, int*avgHumidityId, int*avgPressureId, int*avgWindId, int*precipitationId, int*cloudCoverId);
    void assignIndexFull(QString header, int*dateId, int*minTempId, int*avgTempId, int*maxTempId, int*minDewId, int*avgDewId, int*maxDewId,
                                     int*minHumidityId, int*avgHumidityId, int*maxHumidityId, int*minPressureId, int*avgPressureId, int*maxPressureId,
                                     int*avgWindId, int*maxWindId, int*precipitationId, int*cloudCoverId);
    //void assignData(QStringList allDataList, QList<float>* maxTemp, QList<float>* avgTemp, QList<float>* minTemp, QList<QDate>* date, int dateId, int maxTempId, int avgTempId, int minTempId);
    void loadData(QStringList dayData, int id, QVector<double>* list);
    void loadDataForSummary(QStringList dayData, int id, QVector<double>* list);
    void assignData(QStringList allDataList, QList<QDate>* date, QVector<double>* avgTemp, QVector<double>* avgDew,
                    QVector<double>* avgHumidity, QVector<double>* avgPressure, QVector<double>* avgWind, QVector<double>* precipitation,
                    QVector<double>* cloudCover, int dateId, int avgTempId, int avgDewId, int avgHumidityId, int avgPressureId, int avgWindId,
                    int precipitationId, int cloudCoverId);
    void assignDataFull(QStringList allDataList, QList<QDate>* date, QVector<double>* minTemp, QVector<double>*avgTemp, QVector<double>*maxTemp,
                         QVector<double>*minDew, QVector<double>*avgDew, QVector<double>*maxDew, QVector<double>*minHumidity, QVector<double>*avgHumidity,
                         QVector<double>*maxHumidity, QVector<double>*minPressure, QVector<double>*avgPressure, QVector<double>*maxPressure,
                         QVector<double>*avgWind, QVector<double>*maxWind, QVector<double>*precipitation, QVector<double>*cloudCover,
                         int dateId, int minTempId, int avgTempId, int maxTempId, int minDewId, int avgDewId, int maxDewId, int minHumidityId,
                         int avgHumidityId, int maxHumidityId, int minPressureId, int avgPressureId, int maxPressureId, int avgWindId, int maxWindId,
                         int precipitationId, int cloudCoverId);
    QString getMin(QVector<double> vector);
    QString getMax(QVector<double> vector);
    QString getAvg(QVector<double> vector);
    void replacePolishDiacritics(QString*word);


private slots:
    void on_showData1_clicked();
    void on_showData2_clicked();
    void on_pushButton1_clicked();
    void on_pushButton_clicked();
    void replyFinished1(QNetworkReply* reply);
    void replyFinished2(QNetworkReply* reply);    
    void replyFinished4(QNetworkReply* reply);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
