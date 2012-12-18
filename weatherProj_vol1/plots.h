#ifndef PLOTS_H
#define PLOTS_H

#include <QDialog>

namespace Ui {
class Plots;
}

class Plots : public QDialog
{
    Q_OBJECT
    
public:
    explicit Plots(QWidget *parent = 0, QVector<double> x=0, QVector<double> y=0, QVector<double> y2=0);
    ~Plots();
    
private:
    Ui::Plots *ui;
};

#endif // PLOTS_H