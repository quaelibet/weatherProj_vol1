#include "plots.h"
#include "ui_plots.h"
#include "qcustomplot.h"
#include <QVector>

/**
 * @brief Summary window constructor - all data to be displayed is assigned here
 * @param parent
 * @param minTemp QString containing minimum temperature value
 * @param avgTemp QString containing average temperature value
 * @param maxTemp QString containing maximum temperature value
 * @param minDew QString containing minimum dew point value
 * @param avgDew QString containing average dew point value
 * @param maxDew QString containing maximum dew point value
 * @param minHumidity QString containing minimum humidity value
 * @param avgHumidity QString containing average humidity value
 * @param maxHumidity QString containing maximum humidity value
 * @param minPressure QString containing minimum pressure value
 * @param avgPressure QString containing average pressure value
 * @param maxPressure QString containing maximum pressure value
 * @param avgWind QString containing average wind speed value
 * @param maxWind QString containing maximum wind speed value
 */
Plots::Plots(QWidget *parent, QString minTemp, QString avgTemp, QString maxTemp, QString minDew, QString avgDew, QString maxDew, QString minHumidity,
             QString avgHumidity, QString maxHumidity, QString minPressure, QString avgPressure, QString maxPressure, QString avgWind, QString maxWind) :
    QDialog(parent),
    ui(new Ui::Plots)
{
    ui->setupUi(this);

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

/**
 * @brief Summary window destructor
 */
Plots::~Plots()
{
    delete ui;
}
