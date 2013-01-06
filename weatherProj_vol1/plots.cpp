#include "plots.h"
#include "ui_plots.h"
#include "qcustomplot.h"
#include <QVector>

Plots::Plots(QWidget *parent, QString minTemp, QString avgTemp, QString maxTemp, QString minDew, QString avgDew, QString maxDew, QString minHumidity,
             QString avgHumidity, QString maxHumidity, QString minPressure, QString avgPressure, QString maxPressure, QString avgWind, QString maxWind) :
    QDialog(parent),
    ui(new Ui::Plots)
{
    ui->setupUi(this);
    //ui->temperaturePlot->
            // temperature plot
    ui->minTemp->setText(minTemp);
    ui->avgTemp->setText(avgTemp);
    ui->maxTemp->setText(maxTemp);
    ui->minDew->setText(minDew);
    ui->avgDew->setText(avgDew);
    ui->maxDew->setText(maxDew);
    ui->minHumidity->setText(minHumidity);
    ui->avgHumidity->setText(avgHumidity);
    ui->maxHumidity->setText(maxHumidity);
    ui->minPressure->setText(minPressure);
    ui->avgPressure->setText(avgPressure);
    ui->maxPressure->setText(maxPressure);
    ui->avgWind->setText(avgWind);
    ui->maxWind->setText(maxWind);
}



Plots::~Plots()
{
    delete ui;
}
