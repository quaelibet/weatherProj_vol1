#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include "plots.h"

#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QDate>
#include <QTextCodec>
#include <QVector>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set proper encoding
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));    
    // create store for country combobox
    QStringList countries;
    countries << "Polska";
    // create store for months combobox
    QStringList months;
    //months << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12";
    months << "Styczeń" << "Luty" << "Marzec" << "Kwiecień" << "Maj" << "Czerwiec" << "Lipiec" << "Sierpień" << "Wrzesień" << "Październik" << "Listopad" << "Grudzień";
    // create store for years combobox
    QStringList years;
    years << "2013" << "2012" << "2011" << "2010" << "2009" << "2008" << "2007" << "2006" << "2005" << "2004" << "2003" << "2002" << "2001" << "2000";

    // bind stores to comboboxes    
    ui->countryCombo1->addItems(countries);
    ui->monthCombo1->addItems(months);
    ui->yearCombo1->addItems(years);

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
    //QString city = ui->cityCombo1->currentText();
    QString city = ui->cityLine1->text();
    // check if city if set
    if (city == "" || city.contains(".") || city.contains("/"))
    {
        QMessageBox msgBox;
        msgBox.setText("Brak podanej lokalizacji!");
        msgBox.exec();
    }
    else
    {
        QString country = ui->countryCombo1->currentText();
        //QString month = ui->monthCombo1->currentText();
        QString month = QString::number(ui->monthCombo1->currentIndex()+1);
        QString year = ui->yearCombo1->currentText();

        QString stream = "http://www.wunderground.com/history/airport/"+city+"/"+year+"/"+month+"/1/MonthlyHistory.html?req_city="+city+"&req_state=&req_statename="+country+"&format=1";
        //QString stream = "http://www.wunderground.com/history/airport/"+city+"/"+year+"/"+month+"/1/MonthlyHistory.html?format=1";

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
}

void MainWindow::on_showData2_clicked()
{
    // read chosen values
    //QString city = ui->cityCombo2->currentText();
    QString city = ui->cityLine2->text();
    // check if city if set
    if (city == "" || city.contains(".") || city.contains("/"))
    {
        QMessageBox msgBox;
        msgBox.setText("Brak podanej lokalizacji!");
        msgBox.exec();
    }
    else
    {
        QString country = ui->countryCombo2->currentText();
        //QString month = ui->monthCombo2->currentText();
        QString month = QString::number(ui->monthCombo1->currentIndex()+1);
        QString year = ui->yearCombo2->currentText();

        //QString stream = "http://www.wunderground.com/history/airport/EPKT/"+year+"/"+month+"/1/MonthlyHistory.html?req_city="+city+"&req_state=&req_statename="+country+"&format=1";
        QString stream = "http://www.wunderground.com/history/airport/"+city+"/"+year+"/"+month+"/1/MonthlyHistory.html?req_city="+city+"&req_state=&req_statename="+country+"&format=1";

        QNetworkAccessManager* manager = new QNetworkAccessManager(this);

        QUrl url(stream);
        QNetworkRequest *req = new QNetworkRequest(url);
        req->setRawHeader("Accept-Language", "en-us,en;q=0.5");

        //QNetworkReply* reply = manager->get(QNetworkRequest(url));
        QNetworkReply* reply = manager->get(*req);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished2(QNetworkReply*)));
    }
}

void MainWindow::replyFinished1(QNetworkReply* reply)
{
//    QByteArray rep = reply->readAll();
//    QString str(rep);

    QStringList allDataList = processReply(reply);
    if (allDataList.length() == 0)
    {
        //ui->textEdit1->append(":(");
        QMessageBox msgBox;
        msgBox.setText("Wygląda na to, że wystąpił problem z połączeniem internetowym...");
        msgBox.exec();
    }
    else
    {
        if (allDataList.length() < 2)
        {
            QMessageBox msgBox;
            msgBox.setText("Brak danych dla podanej lokalizacji");
            msgBox.exec();
        }
        else
        {
            //QString str = processReply(reply);
            foreach(QString str, allDataList)
            {
                ui->textEdit1->append(str);
            }
        }
    }
}

void MainWindow::replyFinished2(QNetworkReply* reply)
{    
    QStringList allDataList = processReply(reply);

    int dateId, avgTempId, avgDewId, avgHumidityId, avgPressureId, avgWindId, precipitationId, cloudCoverId;

    QVector<double> avgTemp, avgDew, avgHumidity, avgPressure, avgWind, precipitation, cloudCover;
    QList<QDate> date;

    if (allDataList.length() == 0)
    {
        //ui->textEdit1->append(":(");
        QMessageBox msgBox;
        msgBox.setText("Wygląda na to, że wystąpił problem z połączeniem internetowym...");
        msgBox.exec();
    }
    else
    {
        // assign indexes of adequate values
        assignIndex(allDataList[0],&dateId, &avgTempId, &avgDewId, &avgHumidityId, &avgPressureId, &avgWindId, &precipitationId, &cloudCoverId);

        // assign data to tables
        assignData(allDataList, &date, &avgTemp, &avgDew, &avgHumidity, &avgPressure, &avgWind, &precipitation, &cloudCover, dateId, avgTempId, avgDewId, avgHumidityId, avgPressureId, avgWindId, precipitationId, cloudCoverId);

        QVector<double> temp_x;
        //QVector<double> temp_y;

        for (int i = 1; i <= avgTemp.count(); i++)
        {
            //temp_y.append(avgTemp[i]);
            temp_x.append(i);
        }

        // temperature plot
        ui->temp_plot->setTitle("Temperatura / Punkt rosy");
        ui->temp_plot->setTitleFont(QFont("Arial",8));
        ui->temp_plot->addGraph();
        ui->temp_plot->graph(0)->setData(temp_x,avgTemp);
        ui->temp_plot->xAxis->setLabel("dzień");
        ui->temp_plot->yAxis->setLabel("*C");
        ui->temp_plot->xAxis->setRange(1,31);
        ui->temp_plot->yAxis->setRange(-20,50);
        ui->temp_plot->xAxis->setTicks(true);
        ui->temp_plot->xAxis->setTickLabels(true);
        ui->temp_plot->xAxis->setTickStep(1.0);
        ui->temp_plot->xAxis->setNumberPrecision(1);
        ui->temp_plot->addGraph();
        ui->temp_plot->graph(1)->setPen(QPen(Qt::red));
        ui->temp_plot->graph(1)->setData(temp_x,avgDew);
        ui->temp_plot->replot();

        //pressure plot
        ui->pressure_plot->setTitle("Ciśnienie");
        ui->pressure_plot->setTitleFont(QFont("Arial",8));
        ui->pressure_plot->addGraph();
        ui->pressure_plot->graph(0)->setData(temp_x,avgPressure);
        ui->pressure_plot->xAxis->setLabel("dzień");
        ui->pressure_plot->yAxis->setLabel("hPa");
        ui->pressure_plot->xAxis->setRange(1,31);
        ui->pressure_plot->xAxis->setTicks(true);
        ui->pressure_plot->xAxis->setTickLabels(true);
        ui->pressure_plot->xAxis->setTickStep(1.0);
        ui->pressure_plot->xAxis->setNumberPrecision(1);
        ui->pressure_plot->yAxis->setRange(700,1300);
        ui->pressure_plot->replot();

        //humidity plot
        ui->downpour_plot->setTitle("Wilgotność");
        ui->downpour_plot->setTitleFont(QFont("Arial",8));
        QCPBars *bars = new QCPBars(ui->downpour_plot->xAxis, ui->downpour_plot->yAxis);
        ui->downpour_plot->addPlottable(bars);
        bars->setData(temp_x,avgHumidity);
        ui->downpour_plot->xAxis->setLabel("dzień");
        ui->downpour_plot->yAxis->setLabel("%");
        ui->downpour_plot->xAxis->setRange(1,31);
        ui->downpour_plot->xAxis->setTicks(true);
        ui->downpour_plot->xAxis->setTickLabels(true);
        ui->downpour_plot->xAxis->setTickStep(1.0);
        ui->downpour_plot->xAxis->setNumberPrecision(1);
        ui->downpour_plot->yAxis->setRange(0,100);
        ui->downpour_plot->rescaleAxes();
        ui->downpour_plot->replot();

        //wind plot
    //    ui->wind_plot->setTitle("Siła wiatru");
    //    ui->wind_plot->setTitleFont(QFont("Arial",8));
    //    ui->wind_plot->addGraph();
    //    ui->wind_plot->graph(0)->setData(temp_x,avgWind);
    //    ui->wind_plot->xAxis->setLabel("dzień");
    //    ui->wind_plot->yAxis->setLabel("km/h");
    //    ui->wind_plot->xAxis->setRange(1,31);
    //    ui->wind_plot->xAxis->setTicks(true);
    //    ui->wind_plot->xAxis->setTickLabels(true);
    //    ui->wind_plot->xAxis->setTickStep(1.0);
    //    ui->wind_plot->xAxis->setNumberPrecision(1);
    //    ui->wind_plot->yAxis->setRange(0,300);
    //    ui->wind_plot->replot();

        //ui->textEdit2->append("Srednie cisnienie:");
    //    foreach(QDate d, date)
    //    {
    //        ui->textEdit2->append(d.toString("dd-MM-yyyy"));
    //    }
    //    if (!avgTemp.isEmpty())
    //    {
    //        ui->textEdit2->append("Średnia temperatura:");
    //        foreach(float n,avgTemp)
    //        {
    //            if (n == 9999)
    //            {
    //                ui->textEdit2->append("Brak danych");
    //            }
    //            else
    //            {
    //                ui->textEdit2->append(QString::number(n));
    //            }
    //        }
    //    }
    //    if (!avgPressure.isEmpty())
    //    {
    //        ui->textEdit2->append("Średnie ciśnienie:");
    //        foreach(float n,avgPressure)
    //        {
    //            if (n == 9999)
    //            {
    //                ui->textEdit2->append("Brak danych");
    //            }
    //            else
    //            {
    //                ui->textEdit2->append(QString::number(n));
    //            }
    //        }
    //    }
    //    if (!cloudCover.isEmpty())
    //    {
    //        ui->textEdit2->append("Zachmurzenie:");
    //        foreach(float n,cloudCover)
    //        {
    //            if (n == 9999)
    //            {
    //                ui->textEdit2->append("Brak danych");
    //            }
    //            else
    //            {
    //                ui->textEdit2->append(QString::number(n));
    //            }
    //        }
    //    }

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

/**
 * @brief MainWindow::assignIndex
 * @param header
 * @param dateId
 * @param avgTempId
 * @param avgDewId
 * @param avgHumidityId
 * @param avgPressureId
 * @param avgWindId
 * @param precipitationId
 * @param cloudCoverId
 * Assigns indexes needed for collecting data for plots
 */
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

/**
 * @brief MainWindow::assignIndexFull
 * @param header
 * @param dateId
 * @param minTempId
 * @param avgTempId
 * @param maxTempId
 * @param minDewId
 * @param avgDewId
 * @param maxDewId
 * @param minHumidityId
 * @param avgHumidityId
 * @param maxHumidityId
 * @param minPressureId
 * @param avgPressureId
 * @param maxPressureId
 * @param avgWindId
 * @param maxWindId
 * @param precipitationId
 * @param cloudCoverId
 * Assigns indexes needed for monthly summary
 */
void MainWindow::assignIndexFull(QString header, int*dateId, int*minTempId, int*avgTempId, int*maxTempId, int*minDewId, int*avgDewId, int*maxDewId,
                                 int*minHumidityId, int*avgHumidityId, int*maxHumidityId, int*minPressureId, int*avgPressureId, int*maxPressureId,
                                 int*avgWindId, int*maxWindId, int*precipitationId, int*cloudCoverId)
{

    // CET,Max TemperatureC,Mean TemperatureC,Min TemperatureC,Dew PointC,MeanDew PointC,Min DewpointC,Max Humidity,
    // Mean Humidity, Min Humidity, Max Sea Level PressurehPa, Mean Sea Level PressurehPa, Min Sea Level PressurehPa, Max VisibilityKm,
    // Mean VisibilityKm, Min VisibilitykM, Max Wind SpeedKm/h, Mean Wind SpeedKm/h, Max Gust SpeedKm/h,Precipitationmm, CloudCover, Events,
    // WindDirDegrees

    QStringList indexData = header.split(",");
    for (int i = 0; i < indexData.length(); i++)
    {
        indexData[i] = indexData[i].simplified();
    }

    for (int i = 0; i < indexData.length(); i++)
    {
        if (indexData[i] == "CET") { *dateId = i; }
        else if (indexData[i] == "Max TemperatureC") { *maxTempId = i; }
        else if (indexData[i] == "Mean TemperatureC") { *avgTempId = i; }
        else if (indexData[i] == "Min TemperatureC") { *minTempId = i; }
        else if (indexData[i] == "Dew PointC") { *maxDewId = i; }
        else if (indexData[i] == "MeanDew PointC") { *avgDewId = i; }
        else if (indexData[i] == "Min DewpointC") { *minDewId = i; }
        else if (indexData[i] == "Max Humidity") { *maxHumidityId = i; }
        else if (indexData[i] == "Mean Humidity") { *avgHumidityId = i; }
        else if (indexData[i] == "Min Humidity") { *minHumidityId = i; }
        else if (indexData[i] == "Max Sea Level PressurehPa") { *maxPressureId = i; }
        else if (indexData[i] == "Mean Sea Level PressurehPa") { *avgPressureId = i; }
        else if (indexData[i] == "Min Sea Level PressurehPa") { *minPressureId = i; }
        else if (indexData[i] == "Max Wind SpeedKm/h") { *maxWindId = i; }
        else if (indexData[i] == "Mean Wind SpeedKm/h") { *avgWindId = i; }
        else if (indexData[i] == "Precipitationmm") { *precipitationId = i; }
        else if (indexData[i] == "CloudCover") { *cloudCoverId = i; }
    }
}

/**
 * @brief MainWindow::assignData
 * @param allDataList
 * @param date
 * @param avgTemp
 * @param avgDew
 * @param avgHumidity
 * @param avgPressure
 * @param avgWind
 * @param precipitation
 * @param cloudCover
 * @param dateId
 * @param avgTempId
 * @param avgDewId
 * @param avgHumidityId
 * @param avgPressureId
 * @param avgWindId
 * @param precipitationId
 * @param cloudCoverId
 * Assigns data needed for plots
 */
void MainWindow::assignData(QStringList allDataList, QList<QDate>* date, QVector<double>* avgTemp, QVector<double>* avgDew,
                            QVector<double>* avgHumidity, QVector<double>* avgPressure, QVector<double>* avgWind, QVector<double>* precipitation,
                            QVector<double>* cloudCover, int dateId, int avgTempId, int avgDewId, int avgHumidityId, int avgPressureId,
                            int avgWindId, int precipitationId, int cloudCoverId)
{
    if (allDataList.length() < 2)
    {
        QMessageBox msgBox;
        msgBox.setText("Brak danych dla podanej lokalizacji");
        msgBox.exec();
    }
    else
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
        }
    }
}

/**
 * @brief MainWindow::assignDataFull
 * @param allDataList
 * @param date
 * @param minTemp
 * @param avgTemp
 * @param maxTemp
 * @param minDew
 * @param avgDew
 * @param maxDew
 * @param minHumidity
 * @param avgHumidity
 * @param maxHumidity
 * @param minPressure
 * @param avgPressure
 * @param maxPressure
 * @param avgWind
 * @param maxWind
 * @param precipitation
 * @param cloudCover
 * @param dateId
 * @param minTempId
 * @param avgTempId
 * @param maxTempId
 * @param minDewId
 * @param avgDewId
 * @param maxDewId
 * @param minHumidityId
 * @param avgHumidityId
 * @param maxHumidityId
 * @param minPressureId
 * @param avgPressureId
 * @param maxPressureId
 * @param avgWindId
 * @param maxWindId
 * @param precipitationId
 * @param cloudCoverId
 * Assigns data needed for monthly summary
 */
void MainWindow::assignDataFull(QStringList allDataList, QList<QDate>* date, QVector<double>* minTemp, QVector<double>*avgTemp, QVector<double>*maxTemp,
                                QVector<double>*minDew, QVector<double>*avgDew, QVector<double>*maxDew, QVector<double>*minHumidity, QVector<double>*avgHumidity,
                                QVector<double>*maxHumidity, QVector<double>*minPressure, QVector<double>*avgPressure, QVector<double>*maxPressure,
                                QVector<double>*avgWind, QVector<double>*maxWind, QVector<double>*precipitation, QVector<double>*cloudCover,
                                int dateId, int minTempId, int avgTempId, int maxTempId, int minDewId, int avgDewId, int maxDewId, int minHumidityId,
                                int avgHumidityId, int maxHumidityId, int minPressureId, int avgPressureId, int maxPressureId, int avgWindId, int maxWindId,
                                int precipitationId, int cloudCoverId)
{
    if (allDataList.length() < 2)
    {
        QMessageBox msgBox;
        msgBox.setText("Brak danych dla podanej lokalizacji");
        msgBox.exec();
    }
    else
    {
        for (int i =1; i < allDataList.length(); i++)
        {
            QStringList dayData = allDataList[i].split(",");
            // assign data to tables
            if (minTempId) { loadDataForSummary(dayData, minTempId, minTemp); }
            if (avgTempId) { loadDataForSummary(dayData, avgTempId, avgTemp); }
            if (maxTempId) { loadDataForSummary(dayData, maxTempId, maxTemp); }
            if (minDewId) { loadDataForSummary(dayData, minDewId, minDew); }
            if (avgDewId) { loadDataForSummary(dayData, avgDewId, avgDew); }
            if (maxDewId) { loadDataForSummary(dayData, maxDewId, maxDew); }
            if (minHumidityId) { loadDataForSummary(dayData, minHumidityId, minHumidity); }
            if (avgHumidityId) { loadDataForSummary(dayData, avgHumidityId, avgHumidity); }
            if (maxHumidityId) { loadDataForSummary(dayData, maxHumidityId, maxHumidity); }
            if (minPressureId) { loadDataForSummary(dayData, minPressureId, minPressure); }
            if (avgPressureId) { loadDataForSummary(dayData, avgPressureId, avgPressure); }
            if (maxPressureId) { loadDataForSummary(dayData, maxPressureId, maxPressure); }
            if (avgWindId) { loadDataForSummary(dayData, avgWindId, avgWind); }
            if (maxWindId) { loadDataForSummary(dayData, maxWindId, maxWind); }
            if (precipitationId) { loadDataForSummary(dayData, precipitationId, precipitation); }
            if (cloudCoverId) { loadDataForSummary(dayData, cloudCoverId, cloudCover); }

            QDate d = QDate::fromString(dayData[dateId],"yyyy-M-d");
            date->push_back(d);
        }
    }
}

void MainWindow::loadData(QStringList dayData, int id, QVector<double>* list)
{
    bool success;
    if (dayData[id].isEmpty())
    {
        //list->push_back(9999);
        list->append(9999);
    }
    else
    {
       // float temp = dayData[id].toFloat(&success);
        double temp = dayData[id].toDouble(&success);
        if (success)
        {
            list->append(temp);
        }
        else
        {
            list->append(9999);
        }
    }
}

void MainWindow::loadDataForSummary(QStringList dayData, int id, QVector<double>* list)
{
    bool success;
    if (dayData[id].isEmpty())
    {
        //don't add anything
    }
    else
    {
       // float temp = dayData[id].toFloat(&success);
        double temp = dayData[id].toDouble(&success);
        if (success)
        {
            list->append(temp);
        }
        else
        {
            // don't add anything
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    // read chosen values
    QString city = ui->cityLine2->text();
    // check if city if set
    if (city == "" || city.contains(".") || city.contains("/"))
    {
        QMessageBox msgBox;
        msgBox.setText("Brak podanej lokalizacji!");
        msgBox.exec();
    }
    else
    {
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
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished4(QNetworkReply*)));
    }
}

/**
 * @brief MainWindow::replyFinished4
 * @param reply
 * Sets data to monthly summary box - in the right panel
 */
void MainWindow::replyFinished4(QNetworkReply* reply)
{
    QStringList allDataList = processReply(reply);

    int dateId, minTempId, avgTempId, maxTempId, minDewId, avgDewId, maxDewId, minHumidityId, avgHumidityId, maxHumidityId, minPressureId,
            avgPressureId, maxPressureId, avgWindId, maxWindId, precipitationId, cloudCoverId;

    QVector<double> minTemp, avgTemp, maxTemp, minDew, avgDew, maxDew, minHumidity, avgHumidity, maxHumidity, minPressure, avgPressure,
            maxPressure, avgWind, maxWind, precipitation, cloudCover;

    QList<QDate> date;

    // assign indexes of adequate values
    assignIndexFull(allDataList[0],&dateId, &minTempId, &avgTempId, &maxTempId, &minDewId, &avgDewId, &maxDewId, &minHumidityId, &avgHumidityId,
                &maxHumidityId, &minPressureId, &avgPressureId, &maxPressureId, &avgWindId, &maxWindId, &precipitationId, &cloudCoverId);

    // assign data to tables
    assignDataFull(allDataList, &date, &minTemp, &avgTemp, &maxTemp, &minDew, &avgDew, &maxDew, &minHumidity, &avgHumidity,
                   &maxHumidity, &minPressure, &avgPressure, &maxPressure, &avgWind, &maxWind, &precipitation, &cloudCover,
                   dateId, minTempId, avgTempId, maxTempId, minDewId, avgDewId, maxDewId, minHumidityId, avgHumidityId,
                   maxHumidityId, minPressureId, avgPressureId, maxPressureId, avgWindId, maxWindId, precipitationId, cloudCoverId);

    double monthMinTemp, monthAvgTemp, monthMaxTemp, monthMinDew, monthAvgDew, monthMaxDew, monthMinHumidity, monthAvgHumidity,
            monthMaxHumidity, monthMinPressure, monthAvgPressure, monthMaxPressure, monthAvgWind, monthMaxWind, monthPrecipitation, monthCloudCover;


    // show plots window
    //Plots *plots = new Plots(this);
    //plots->show();
    QString info = "";

    // temperature summary
    qSort(minTemp);
    monthMinTemp = minTemp.first();
    qSort(maxTemp);
    monthMaxTemp = maxTemp.last();
    monthAvgTemp = 0;
    for (int i =0; i < avgTemp.count(); i++)
    {
        monthAvgTemp +=avgTemp[i];
    }
    monthAvgTemp = monthAvgTemp/avgTemp.count();
    info.append("Temperatura min.: "+QString::number(monthMinTemp)+"\nTemperatura śr.: "+QString::number(monthAvgTemp)+
                "\nTemperatura maks.: "+QString::number(monthMaxTemp)+"\n\n");

    // dew summary
    qSort(minDew);
    monthMinDew = minDew.first();
    qSort(maxDew);
    monthMaxDew = maxDew.last();
    monthAvgDew = 0;
    for (int i =0; i < avgDew.count(); i++)
    {
        monthAvgDew +=avgDew[i];
    }
    monthAvgDew = monthAvgDew/avgDew.count();
    info.append("Punkt rosy - min.: "+QString::number(monthMinDew)+"\nPunkt rosy - śr.: "+QString::number(monthAvgDew)+
                "\nPunkt rosy - maks.: "+QString::number(monthMaxDew)+"\n\n");

    // humidity summary
    qSort(minHumidity);
    monthMinHumidity = minHumidity.first();
    qSort(maxHumidity);
    monthMaxHumidity = maxHumidity.last();
    monthAvgHumidity = 0;
    for (int i =0; i < avgHumidity.count(); i++)
    {
        monthAvgHumidity +=avgHumidity[i];
    }
    monthAvgHumidity = monthAvgHumidity/avgHumidity.count();
    info.append("Punkt rosy - min.: "+QString::number(monthMinDew)+"\nPunkt rosy - śr.: "+QString::number(monthAvgDew)+
                "\nPunkt rosy - maks.: "+QString::number(monthMaxDew)+"\n\n");
    // pressure summary

    // wind summary

    // precipitation summary

    // cloud cover summary

    QMessageBox msgBox;
    msgBox.setWindowTitle(" ");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("Podsumowanie miesiąca:");
    msgBox.setInformativeText(info);
    msgBox.exec();


}

/**
 * @brief MainWindow::replyFinished3
 * @param reply
 * Sets data to month's details box - in the left panel
 */
void MainWindow::replyFinished3(QNetworkReply* reply)
{

}

QString MainWindow::getMin(QVector<double> vector)
{
    double min;
    qSort(vector);
    min = vector.first();

    return QString::number(min);
}

QString MainWindow::getMax(QVector<double> vector)
{
    double max;
    qSort(vector);
    max = vector.last();

    return QString::number(max);
}

QString MainWindow::getAvg(QVector<double> vector)
{
    double avg = 0;

    for (int i =0; i < vector.count(); i++)
    {
        avg +=vector[i];
    }
    avg = avg/vector.count();

    return QString::number(avg);
}
