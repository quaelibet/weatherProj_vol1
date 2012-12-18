#include "plots.h"
#include "ui_plots.h"
#include "qcustomplot.h"
#include <QVector>

Plots::Plots(QWidget *parent, QVector<double> x, QVector<double> y, QVector<double> y2) :
    QDialog(parent),
    ui(new Ui::Plots)
{
    ui->setupUi(this);
    //ui->temperaturePlot->
            // temperature plot
    ui->temperaturePlot->setTitle("Temperatura / Punkt rosy");
    ui->temperaturePlot->setTitleFont(QFont("Arial",8));
    ui->temperaturePlot->addGraph();
    ui->temperaturePlot->graph(0)->setData(x,y);
    ui->temperaturePlot->xAxis->setLabel("dzień");
    ui->temperaturePlot->yAxis->setLabel("*C");
    ui->temperaturePlot->xAxis->setRange(1,31);
    ui->temperaturePlot->yAxis->setRange(-20,50);
    ui->temperaturePlot->xAxis->setTicks(true);
    ui->temperaturePlot->xAxis->setTickLabels(true);
    ui->temperaturePlot->xAxis->setTickStep(1.0);
    ui->temperaturePlot->xAxis->setNumberPrecision(1);
    ui->temperaturePlot->addGraph();
    ui->temperaturePlot->graph(1)->setPen(QPen(Qt::red));
    ui->temperaturePlot->graph(1)->setData(x,y2);
    //ui->temp_plot->xAxis->setLabel("dzień");
    //ui->temp_plot->yAxis->setLabel("*C");
    //ui->temp_plot->xAxis->setRange(1,31);
    //ui->temp_plot->yAxis->setRange(-20,50);
    ui->temperaturePlot->replot();
}



Plots::~Plots()
{
    delete ui;
}
