#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create store for city combobox
    QStringList cities;
    cities << "Katowice" << "Gliwice" << "Warszawa";
    // create store for country combobox
    QStringList countries;
    countries << "Poland";
    // create store for months combobox
    QStringList months;
    months << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12";
    // create store for years combobox
    QStringList years;
    years << "2012" << "2011" << "2010" << "2009" << "2008";

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
    QString month = ui->monthCombo1->currentText();
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
    QString month = ui->monthCombo2->currentText();
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
    QByteArray rep = reply->readAll();
    QString str(rep);
    ui->textEdit1->append(str);
}

void MainWindow::replyFinished2(QNetworkReply* reply)
{
    QByteArray rep = reply->readAll();
    QString str(rep);
    ui->textEdit2->append(str);
}


