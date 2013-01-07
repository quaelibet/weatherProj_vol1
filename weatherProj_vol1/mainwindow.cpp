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

/**
 * @brief MainWindow::MainWindow
 * @param parent
 * Main window constructor - stores are binded to adequate comboboxes here
 */
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

/**
 * @brief MainWindow::~MainWindow
 * Main window destructor
 */
MainWindow::~MainWindow()
{
    delete ui;
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
void MainWindow::assignIndex(QString header, int*dateId, int*avgTempId, int*avgDewId, int*avgHumidityId, int*avgPressureId, int*avgWindId,
                             int*precipitationId, int*cloudCoverId)
{
    // header: CET,Max TemperatureC,Mean TemperatureC,Min TemperatureC,Dew PointC,MeanDew PointC,Min DewpointC,Max Humidity, Mean Humidity,
    // Min Humidity, Max Sea Level PressurehPa, Mean Sea Level PressurehPa, Min Sea Level PressurehPa, Max VisibilityKm, Mean VisibilityKm,
    // Min VisibilitykM, Max Wind SpeedKm/h, Mean Wind SpeedKm/h, Max Gust SpeedKm/h,Precipitationmm, CloudCover, Events,WindDirDegrees

    QStringList indexData = header.split(",");
    for (int i = 0; i < indexData.length(); i++)
    {
        indexData[i] = indexData[i].simplified();
    }

    for (int i = 0; i < indexData.length(); i++)
    {
        if (indexData[i] == "CET") { *dateId = i; }        
        else if (indexData[i] == "Mean TemperatureC") { *avgTempId = i; }        
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
        msgBox.setWindowTitle("!");
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
        msgBox.setWindowTitle("!");
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

/**
 * @brief MainWindow::loadData
 * @param dayData
 * @param id
 * @param list
 * Loads data from given day from column pointed by id to adequate vector
 * When column value is empty - 0 is loaded to vector - it's important to always have some value remembered for given day
 */
void MainWindow::loadData(QStringList dayData, int id, QVector<double>* list)
{
    bool success;
    if (dayData[id].isEmpty())
    {        
        list->append(0);
    }
    else
    {       
        double temp = dayData[id].toDouble(&success);
        if (success)
        {
            list->append(temp);
        }
        else
        {
            list->append(0);
        }
    }
}

/**
 * @brief MainWindow::loadDataForSummary
 * @param dayData
 * @param id
 * @param list
 * Loads data from given day from column pointed by id to adequate vector
 * When column value is empty - nothing is loaded to vector
 */
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

/**
 * @brief MainWindow::getMin
 * @param vector
 * @return QString min
 * Finds minimum value in given vector and returns it as QString
 */
QString MainWindow::getMin(QVector<double> vector)
{
    if (vector.count() == 0)
    {
        return "Brak danych";
    }
    double min;    
    qSort(vector);
    min = vector.first();

    return QString::number(min);
}

/**
 * @brief MainWindow::getMax
 * @param vector
 * @return QString max
 * Finds maximum value in given vector and returns it as QString
 */
QString MainWindow::getMax(QVector<double> vector)
{
    if (vector.count() == 0)
    {
        return "Brak danych";
    }
    double max;
    qSort(vector);
    max = vector.last();

    return QString::number(max);
}

/**
 * @brief MainWindow::getAvg
 * @param vector
 * @return QString avg
 * Finds average value in given vector and returns it as QString
 */
QString MainWindow::getAvg(QVector<double> vector)
{
    if (vector.count() == 0)
    {
        return "Brak danych";
    }
    double avg = 0;
    for (int i =0; i < vector.count(); i++)
    {
        avg +=vector[i];
    }
    avg = avg/vector.count();

    return QString::number(avg);
}

/**
 * @brief MainWindow::replacePolishDiacritics
 * @param word
 * Replaces polish diactritics in given QString
 */
void MainWindow::replacePolishDiacritics(QString*word)
{
    word->replace("ą","a");
    word->replace("ć","c");
    word->replace("ę","e");
    word->replace("ł","l");
    word->replace("ń","n");
    word->replace("ó","o");
    word->replace("ś","s");
    word->replace("ź","z");
    word->replace("ż","z");
}

/**
 * @brief MainWindow::processReply
 * @param reply
 * @return QStringList temp
 * Processes network reply
 */
QStringList MainWindow::processReply(QNetworkReply* reply)
{
    QByteArray rep = reply->readAll();
    QString str(rep);
    // remove headers ..
    // remove data to first line break!
    QStringList temp = str.split(QRegExp("(\\r\\n)|(\\n\\r)|\\r|\\n"), QString::SkipEmptyParts);

    return temp;
}

/**
 * @brief MainWindow::on_showData1_clicked
 * Gets data from inputs and sends request (left panel)
 */
void MainWindow::on_showData1_clicked()
{
    // read chosen values    
    QString city = ui->cityLine1->text();
    // check if city if set
    if (city == "" || city.contains(".") || city.contains("/"))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("!");
        msgBox.setText("Brak podanej lokalizacji!");
        msgBox.exec();
    }
    else
    {
        // strip city of polish letters
        replacePolishDiacritics(&city);

        QString country = ui->countryCombo1->currentText();
        //QString month = ui->monthCombo1->currentText();
        QString month = QString::number(ui->monthCombo1->currentIndex()+1);
        QString year = ui->yearCombo1->currentText();

        QString stream = "http://www.wunderground.com/history/airport/"+city+"/"+year+"/"+month+"/1/MonthlyHistory.html?req_city="+city+"&req_state=&req_statename="+country+"&format=1";

        QNetworkAccessManager* manager = new QNetworkAccessManager(this);

        QUrl url(stream);
        QNetworkRequest *req = new QNetworkRequest(url);
        req->setRawHeader("Accept-Language", "en-us,en;q=0.5");

        QNetworkReply* reply = manager->get(*req);       

        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished1(QNetworkReply*)));        
    }
}

/**
 * @brief MainWindow::replyFinished1
 * @param reply
 * Assigns data to plots (left panel)
 */
void MainWindow::replyFinished1(QNetworkReply* reply)
{
    QStringList allDataList = processReply(reply);

    int dateId, avgTempId, avgDewId, avgHumidityId, avgPressureId, avgWindId, precipitationId, cloudCoverId;

    QVector<double> avgTemp, avgDew, avgHumidity, avgPressure, avgWind, precipitation, cloudCover;
    QList<QDate> date;

    if (allDataList.length() == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Wygląda na to, że wystąpił problem z połączeniem internetowym...");
        msgBox.setWindowTitle("!");
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
        ui->temp_plot_l->clearGraphs();
        ui->temp_plot_l->setTitle("Temperatura / Punkt rosy");
        ui->temp_plot_l->setTitleFont(QFont("Arial",8));
        ui->temp_plot_l->legend->setVisible(true);
        ui->temp_plot_l->legend->setFont(QFont("Arial",7));
        ui->temp_plot_l->legend->setPositionStyle(QCPLegend::psManual); //psBottomRight);
        ui->temp_plot_l->legend->setPosition(QPoint(350,0));
        ui->temp_plot_l->addGraph();
        ui->temp_plot_l->graph(0)->setData(temp_x,avgTemp);
        ui->temp_plot_l->graph(0)->setName("temperatura");
        ui->temp_plot_l->xAxis->setLabel("dzień");
        ui->temp_plot_l->yAxis->setLabel("*C");
        ui->temp_plot_l->xAxis->setRange(1,31);
        ui->temp_plot_l->yAxis->setRange(-20,50);
        ui->temp_plot_l->xAxis->setTicks(true);
        ui->temp_plot_l->xAxis->setTickLabels(true);
        ui->temp_plot_l->xAxis->setTickStep(1.0);
        ui->temp_plot_l->xAxis->setNumberPrecision(1);
        ui->temp_plot_l->addGraph();
        ui->temp_plot_l->graph(1)->setPen(QPen(Qt::red));
        ui->temp_plot_l->graph(1)->setData(temp_x,avgDew);
        ui->temp_plot_l->graph(1)->setName("punkt rosy");
        ui->temp_plot_l->replot();

        //pressure plot
        ui->pressure_plot_l->clearGraphs();
        ui->pressure_plot_l->setTitle("Ciśnienie");
        ui->pressure_plot_l->setTitleFont(QFont("Arial",8));
        ui->pressure_plot_l->addGraph();
        ui->pressure_plot_l->graph(0)->setData(temp_x,avgPressure);
        ui->pressure_plot_l->xAxis->setLabel("dzień");
        ui->pressure_plot_l->yAxis->setLabel("hPa");
        ui->pressure_plot_l->xAxis->setRange(1,31);
        ui->pressure_plot_l->xAxis->setTicks(true);
        ui->pressure_plot_l->xAxis->setTickLabels(true);
        ui->pressure_plot_l->xAxis->setTickStep(1.0);
        ui->pressure_plot_l->xAxis->setNumberPrecision(1);
        ui->pressure_plot_l->yAxis->setRange(700,1300);
        ui->pressure_plot_l->replot();

        //humidity plot
        ui->downpour_plot_l->clearPlottables();
        ui->downpour_plot_l->setTitle("Wilgotność");
        ui->downpour_plot_l->setTitleFont(QFont("Arial",8));
        QCPBars *bars = new QCPBars(ui->downpour_plot_l->xAxis, ui->downpour_plot_l->yAxis);
        ui->downpour_plot_l->addPlottable(bars);
        bars->setData(temp_x,avgHumidity);
        ui->downpour_plot_l->xAxis->setLabel("dzień");
        ui->downpour_plot_l->yAxis->setLabel("%");
        ui->downpour_plot_l->xAxis->setRange(1,31);
        ui->downpour_plot_l->xAxis->setTicks(true);
        ui->downpour_plot_l->xAxis->setTickLabels(true);
        ui->downpour_plot_l->xAxis->setTickStep(1.0);
        ui->downpour_plot_l->xAxis->setNumberPrecision(1);
        ui->downpour_plot_l->yAxis->setRange(0,100);
        ui->downpour_plot_l->rescaleAxes();
        ui->downpour_plot_l->replot();        
    }
}

/**
 * @brief MainWindow::on_showData2_clicked
 * Gets data from inputs and sends request (right panel)
 */
void MainWindow::on_showData2_clicked()
{
    // read chosen values    
    QString city = ui->cityLine2->text();
    // check if city if set
    if (city == "" || city.contains(".") || city.contains("/"))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("!");
        msgBox.setText("Brak podanej lokalizacji!");
        msgBox.exec();
    }
    else
    {
        // strip city of polish letters
        replacePolishDiacritics(&city);

        QString country = ui->countryCombo2->currentText();        
        QString month = QString::number(ui->monthCombo2->currentIndex()+1);
        QString year = ui->yearCombo2->currentText();

        QString stream = "http://www.wunderground.com/history/airport/"+city+"/"+year+"/"+month+"/1/MonthlyHistory.html?req_city="+city+"&req_state=&req_statename="+country+"&format=1";

        QNetworkAccessManager* manager = new QNetworkAccessManager(this);

        QUrl url(stream);
        QNetworkRequest *req = new QNetworkRequest(url);
        req->setRawHeader("Accept-Language", "en-us,en;q=0.5");

        QNetworkReply* reply = manager->get(*req);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished2(QNetworkReply*)));
    }
}

/**
 * @brief MainWindow::replyFinished2
 * @param reply
 * Assigns data to plots (right panel)
 */
void MainWindow::replyFinished2(QNetworkReply* reply)
{    
    QStringList allDataList = processReply(reply);

    int dateId, avgTempId, avgDewId, avgHumidityId, avgPressureId, avgWindId, precipitationId, cloudCoverId;

    QVector<double> avgTemp, avgDew, avgHumidity, avgPressure, avgWind, precipitation, cloudCover;
    QList<QDate> date;

    if (allDataList.length() == 0)
    {        
        QMessageBox msgBox;
        msgBox.setText("Wygląda na to, że wystąpił problem z połączeniem internetowym...");
        msgBox.setWindowTitle("!");
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
        ui->temp_plot->clearGraphs();
        ui->temp_plot->setTitle("Temperatura / Punkt rosy");
        ui->temp_plot->setTitleFont(QFont("Arial",8));
        ui->temp_plot->legend->setVisible(true);
        ui->temp_plot->legend->setFont(QFont("Arial",7));
        ui->temp_plot->legend->setPositionStyle(QCPLegend::psManual); //psBottomRight);
        ui->temp_plot->legend->setPosition(QPoint(350,0));
        ui->temp_plot->addGraph();
        ui->temp_plot->graph(0)->setData(temp_x,avgTemp);
        ui->temp_plot->graph(0)->setName("temperatura");
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
        ui->temp_plot->graph(1)->setName("punkt rosy");
        ui->temp_plot->replot();

        //pressure plot
        ui->pressure_plot->clearGraphs();
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
        ui->downpour_plot->clearPlottables();
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
    }
}

/**
 * @brief MainWindow::on_pushButton_clicked
 * Gets data from inputs and sends request (right panel)
 */
void MainWindow::on_pushButton_clicked()
{
    // read chosen values
    QString city = ui->cityLine2->text();
    // check if city if set
    if (city == "" || city.contains(".") || city.contains("/"))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("!");
        msgBox.setText("Brak podanej lokalizacji!");
        msgBox.exec();
    }
    else
    {
        // strip city of polish letters
        replacePolishDiacritics(&city);

        QString country = ui->countryCombo2->currentText();
        //QString month = ui->monthCombo2->currentText();
        QString month = QString::number(ui->monthCombo2->currentIndex()+1);
        QString year = ui->yearCombo2->currentText();

        QString stream = "http://www.wunderground.com/history/airport/"+city+"/"+year+"/"+month+"/1/MonthlyHistory.html?req_city="+city+"&req_state=&req_statename="+country+"&format=1";

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
 * @brief MainWindow::on_pushButton1_clicked
 * Gets data from inputs and sends request (left panel)
 */
void MainWindow::on_pushButton1_clicked()
{
    // read chosen values
    QString city = ui->cityLine1->text();
    // check if city if set
    if (city == "" || city.contains(".") || city.contains("/"))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("!");
        msgBox.setText("Brak podanej lokalizacji!");
        msgBox.exec();
    }
    else
    {
        // strip city of polish letters
        replacePolishDiacritics(&city);

        QString country = ui->countryCombo1->currentText();
        //QString month = ui->monthCombo1->currentText();
        QString month = QString::number(ui->monthCombo1->currentIndex()+1);
        QString year = ui->yearCombo1->currentText();

        QString stream = "http://www.wunderground.com/history/airport/"+city+"/"+year+"/"+month+"/1/MonthlyHistory.html?req_city="+city+"&req_state=&req_statename="+country+"&format=1";

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
 * Sets data to monthly summary box - in both panels
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

    QString monthMinTemp, monthAvgTemp, monthMaxTemp, monthMinDew, monthAvgDew, monthMaxDew, monthMinHumidity, monthAvgHumidity,
            monthMaxHumidity, monthMinPressure, monthAvgPressure, monthMaxPressure, monthAvgWind, monthMaxWind, monthPrecipitation, monthCloudCover;

    // temperature summary
    monthMinTemp = getMin(minTemp);
    monthAvgTemp = getAvg(avgTemp);
    monthMaxTemp = getMax(maxTemp);
    // dew summary
    monthMinDew = getMin(minDew);
    monthAvgDew = getAvg(avgDew);
    monthMaxDew = getMax(maxDew);
    // humidity summary
    monthMinHumidity = getMin(minHumidity);
    monthAvgHumidity = getAvg(avgHumidity);
    monthMaxHumidity = getMax(maxHumidity);
    // pressure summary
    monthMinPressure = getMin(minPressure);
    monthAvgPressure = getAvg(avgPressure);
    monthMaxPressure = getMax(maxPressure);
    // wind summary
    monthAvgWind = getAvg(avgWind);
    monthMaxWind = getMax(maxWind);
    // precipitation summary
    monthPrecipitation = getAvg(precipitation);
    // cloud cover summary
    monthCloudCover = getAvg(cloudCover);

    // show summary window
    Plots *plots = new Plots(this, monthMinTemp, monthAvgTemp, monthMaxTemp, monthMinDew, monthAvgDew, monthMaxDew, monthMinHumidity, monthAvgHumidity,
                             monthMaxHumidity, monthMinPressure, monthAvgPressure, monthMaxPressure, monthAvgWind, monthMaxWind);
    plots->setWindowTitle("Podsumowanie miesiąca");
    plots->show();
}
