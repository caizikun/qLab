#ifndef PLOT_H
#define PLOT_H
#include "qwt_plot.h"

class Plot : public QwtPlot
{
    Q_OBJECT
    //unidentified copy constructor
    Plot(const Plot&);
public:
    explicit Plot(QWidget *parent = 0);
    ~Plot();


signals:

public slots:

};

#endif // PLOT_H
