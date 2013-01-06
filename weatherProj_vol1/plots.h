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
    explicit Plots(QWidget *parent = 0, QString minTemp = "", QString avgTemp = "", QString maxTemp = "", QString minDew = "", QString avgDew = "",
                   QString maxDew = "", QString minHumidity = "", QString avgHumidity = "", QString maxHumidity = "", QString minPressure = "",
                   QString avgPressure = "", QString maxPressure = "", QString avgWind = "", QString maxWind = "");
    ~Plots();
    
private:
    Ui::Plots *ui;
};

#endif // PLOTS_H
