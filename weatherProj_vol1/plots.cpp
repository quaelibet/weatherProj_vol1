#include "plots.h"
#include "ui_plots.h"
#include "qcustomplot.h"
#include <QVector>

Plots::Plots(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Plots)
{
    ui->setupUi(this);
    //ui->temperaturePlot->
            // temperature plot

}



Plots::~Plots()
{
    delete ui;
}
