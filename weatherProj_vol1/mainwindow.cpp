#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QDate>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set proper encoding
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    // create store for city combobox
    QStringList cities;
    cities << "Katowice" << "Gliwice" << "Warszawa";
    // create store for country combobox
    QStringList countries;
    countries << "Polska";
    // create store for months combobox
    QStringList months;
    //months << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12";
    months << "Styczeń" << "Luty" << "Marzec" << "Kwiecień" << "Maj" << "Czerwiec" << "Lipiec" << "Sierpień" << "Wrzesień" << "Październik" << "Listopad" << "Grudzień";
    // create store for years combobox
    QStringList years;
    years << "2012" << "2011" << "2010" << "2009" << "2008" << "2007" << "2006" << "2005" << "2004" << "2003" << "2002" << "2001" << "2000";

    // bind stores to comboboxes
    ui->cityCombo1->addItems(cities);
    ui->countryCombo1->addItems(countries);
    ui->monthCombo1->addItems(months);
    ui->yearCombo1->addItems(years);
    ui->cityCombo2->addItems(cities);
    ui->countryCombo2->addItems(countries);
    ui->monthCombo2->addItems(months);
    ui->yearCombo2->addItems(years);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_showData1_clicked()
{
    // read chosen values
    QString city = ui->cityCombo1->currentText();
    QString country = ui->countryCombo1->currentText();
    //QString month = ui->monthCombo1->currentText();
    QString month = QString::number(ui->monthCombo1->currentIndex()+1);
    QString year = ui->yearCombo1->currentText();

    QString stream = "http://www.wunderground.com/history/airport/EPKT/"+year+"/"+month+"/1/MonthlyHistory.html?req_city="+city+"&req_state=&req_statename="+country+"&format=1";

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    QUrl url(stream);
    QNetworkRequest *req = new QNetworkRequest(url);
    req->setRawHeader("Accept-Language", "en-us,en;q=0.5");

    //QNetworkReply* reply = manager->get(QNetworkRequest(url));
    QNetworkReply* reply = manager->get(*req);
    //QNetworkReply* reply = manager->get(QNetworkRequest(url));

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished1(QNetworkReply*)));

    //ui->textEdit1->append(temp);
}

void MainWindow::on_showData2_clicked()
{
    // read chosen values
    QString city = ui->cityCombo2->currentText();
    QString country = ui->countryCombo2->currentText();
    //QString month = ui->monthCombo2->currentText();
    QString month = QString::number(ui->monthCombo1->currentIndex()+1);
    QString year = ui->yearCombo2->currentText();

    QString stream = "http://www.wunderground.com/history/airport/EPKT/"+year+"/"+month+"/1/MonthlyHistory.html?req_city="+city+"&req_state=&req_statename="+country+"&format=1";

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    QUrl url(stream);
    QNetworkRequest *req = new QNetworkRequest(url);
    req->setRawHeader("Accept-Language", "en-us,en;q=0.5");

    //QNetworkReply* reply = manager->get(QNetworkRequest(url));
    QNetworkReply* reply = manager->get(*req);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished2(QNetworkReply*)));

}

void MainWindow::replyFinished1(QNetworkReply* reply)
{
//    QByteArray rep = reply->readAll();
//    QString str(rep);
    QStringList allDataList = processReply(reply);
    //QString str = processReply(reply);
    foreach(QString str, allDataList)
    {
        ui->textEdit1->append(str);
    }
}

void MainWindow::replyFinished2(QNetworkReply* reply)
{
    //QByteArray rep = reply->readAll();
    //QString str(rep);
    //QString str = processReply(reply);
    //QList<QStringList> allData = processReply(reply);
    QStringList allDataList = processReply(reply);

    //int dateId, maxTempId, avgTempId, minTempId;
    int dateId, avgTempId, avgDewId, avgHumidityId, avgPressureId, avgWindId, precipitationId, cloudCoverId;
    //QList<float> maxTemp, avgTemp, minTemp;
    QList<float> avgTemp, avgDew, avgHumidity, avgPressure, avgWind, precipitation, cloudCover;
    QList<QDate> date;

    // assign indexes of adequate values
    //assignIndex(allDataList[0],&dateId, &maxTempId, &avgTempId, &minTempId);
    assignIndex(allDataList[0],&dateId, &avgTempId, &avgDewId, &avgHumidityId, &avgPressureId, &avgWindId, &precipitationId, &cloudCoverId);

    // assign data to tables
    //assignData(allDataList,&maxTemp, &avgTemp, &minTemp, &date, dateId, maxTempId, avgTempId, minTempId);
    assignData(allDataList, &date, &avgTemp, &avgDew, &avgHumidity, &avgPressure, &avgWind, &precipitation, &cloudCover, dateId, avgTempId, avgDewId, avgHumidityId, avgPressureId, avgWindId, precipitationId, cloudCoverId);
    //ui->textEdit2->append(allDataList[0]);

    //ui->textEdit2->append("Srednie cisnienie:");
    foreach(QDate d, date)
    {
        ui->textEdit2->append(d.toString("dd-MM-yyyy"));
    }
    if (!avgTemp.isEmpty())
    {
        ui->textEdit2->append("Średnia temperatura:");
        foreach(float n,avgTemp)
        {
            if (n == 9999)
            {
                ui->textEdit2->append("Brak danych");
            }
            else
            {
                ui->textEdit2->append(QString::number(n));
            }
        }
    }
    if (!avgPressure.isEmpty())
    {
        ui->textEdit2->append("Średnie ciśnienie:");
        foreach(float n,avgPressure)
        {
            if (n == 9999)
            {
                ui->textEdit2->append("Brak danych");
            }
            else
            {
                ui->textEdit2->append(QString::number(n));
            }
        }
    }
    if (!cloudCover.isEmpty())
    {
        ui->textEdit2->append("Zachmurzenie:");
        foreach(float n,cloudCover)
        {
            if (n == 9999)
            {
                ui->textEdit2->append("Brak danych");
            }
            else
            {
                ui->textEdit2->append(QString::number(n));
            }
        }
    }

//    ui->textEdit2->append(QString::number(dateId)); //0 ok
//    ui->textEdit2->append(QString::number(avgTempId)); //2 ok
//    ui->textEdit2->append(QString::number(avgDewId)); //5 ok
//    ui->textEdit2->append(QString::number(avgHumidityId)); //0 :( 8
//    ui->textEdit2->append(QString::number(avgPressureId)); //1 :( 11
//    ui->textEdit2->append(QString::number(avgWindId)); //0 :( 17
//    ui->textEdit2->append(QString::number(precipitationId));//19
//    ui->textEdit2->append(QString::number(cloudCoverId));//0 :( 20


    //ui->textEdit2->append(QString::number(minTempId));
}

//QList<QStringList> MainWindow::processReply(QNetworkReply* reply)
QStringList MainWindow::processReply(QNetworkReply* reply)
{
    QByteArray rep = reply->readAll();
    QString str(rep);
    // remove headers ..
    // remove data to first line break!
    QStringList temp = str.split(QRegExp("(\\r\\n)|(\\n\\r)|\\r|\\n"), QString::SkipEmptyParts);
 //   QString weatherData = "";
 //   QList<QStringList> allData;
 //   for (int i =1; i < temp.length(); i++)
 //   {
 //       QStringList dayData = temp[i].split(",");
 //       allData.push_back(dayData);
 //   }
    // separate data
    // separate on linebreaks
    // next separate on commas
     return temp;
 //   return allData;
}

void MainWindow::assignIndex(QString header, int*dateId, int*avgTempId, int*avgDewId, int*avgHumidityId, int*avgPressureId, int*avgWindId, int*precipitationId, int*cloudCoverId)
{
    // header: CET,Max TemperatureC,Mean TemperatureC,Min TemperatureC,Dew PointC,MeanDew PointC,Min DewpointC,Max Humidity, Mean Humidity, Min Humidity, Max Sea Level PressurehPa, Mean Sea Level PressurehPa, Min Sea Level PressurehPa, Max VisibilityKm, Mean VisibilityKm, Min VisibilitykM, Max Wind SpeedKm/h, Mean Wind SpeedKm/h, Max Gust SpeedKm/h,Precipitationmm, CloudCover, Events,WindDirDegrees
    QStringList indexData = header.split(",");
    for (int i = 0; i < indexData.length(); i++)
    {
        indexData[i] = indexData[i].simplified();
    }

    for (int i = 0; i < indexData.length(); i++)
    {
        if (indexData[i] == "CET") { *dateId = i; }
        //else if (indexData[i] == "Max TemperatureC") { *maxTempId = i; }
        else if (indexData[i] == "Mean TemperatureC") { *avgTempId = i; }
        //else if (indexData[i] == "Min TemperatureC") { *minTempId = i; }
        else if (indexData[i] == "MeanDew PointC") { *avgDewId = i; }
        else if (indexData[i] == "Mean Humidity") { *avgHumidityId = i; }
        else if (indexData[i] == "Mean Sea Level PressurehPa") { *avgPressureId = i; }
        else if (indexData[i] == "Mean Wind SpeedKm/h") { *avgWindId = i; }
        else if (indexData[i] == "Precipitationmm") { *precipitationId = i; }
        else if (indexData[i] == "CloudCover") { *cloudCoverId = i; }
    }
}
// avgTemp, avgDew, avgHumidity, avgPressure, avgWind, precipitation, cloudCover
//assignData(allDataList, &date, &avgTemp, &avgDew, &avgHumidity, &avgPressure, &avgWind, &precipitation, &cloudCover,
//           dateId, avgTempId, avgDewId, avgHumidityId, avgPressureId, avgWindId, precipitationId, cloudCoverId);

//void MainWindow::assignData(QStringList allDataList, QList<float>* maxTemp, QList<float>* avgTemp, QList<float>* minTemp, QList<QDate>* date, int dateId, int maxTempId, int avgTempId, int minTempId)
void MainWindow::assignData(QStringList allDataList, QList<QDate>* date, QList<float>* avgTemp, QList<float>* avgDew,
                            QList<float>* avgHumidity, QList<float>* avgPressure, QList<float>* avgWind, QList<float>* precipitation,
                            QList<float>* cloudCover, int dateId, int avgTempId, int avgDewId, int avgHumidityId, int avgPressureId,
                            int avgWindId, int precipitationId, int cloudCoverId)
{
    for (int i =1; i < allDataList.length(); i++)
    {
        QStringList dayData = allDataList[i].split(",");
        // assign data to tables

        if (avgTempId) { loadData(dayData, avgTempId, avgTemp); }
        if (avgDewId) { loadData(dayData, avgDewId, avgDew); }
        if (avgHumidityId) { loadData(dayData, avgHumidityId, avgHumidity); }
        if (avgPressureId) { loadData(dayData, avgPressureId, avgPressure); }
        if (avgWindId) { loadData(dayData, avgWindId, avgWind); }
        if (precipitationId) { loadData(dayData, precipitationId, precipitation); }
        if (cloudCoverId) { loadData(dayData, cloudCoverId, cloudCover); }

        QDate d = QDate::fromString(dayData[dateId],"yyyy-M-d");
        date->push_back(d);
        //        float maxT = dayData[maxTempId].toFloat(&success);
        //        if (success)
        //        {
        //            maxTemp->push_back(maxT);
        //        }
        //        float minT = dayData[avgTempId].toFloat(&success);
        //        if (success)
        //        {
        //            minTemp->push_back(minT);
        //        }
    }
}

void MainWindow::loadData(QStringList dayData, int id, QList<float>* list)
{
    bool success;
    if (dayData[id].isEmpty())
    {
        list->push_back(9999);
    }
    else
    {
        float temp = dayData[id].toFloat(&success);
        if (success)
        {
            list->push_back(temp);
        }
        else
        {
            list->push_back(9999);
        }
    }
}
