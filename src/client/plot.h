#ifndef PLOT_H
#define PLOT_H
#include "qwt_plot.h"
#include "seriesdata.h"
#include "experimentdata.h"
#include "qwt_plot_curve.h"
#include "qwt_symbol.h"
#include "qwt_plot_grid.h"
#include "qwt_legend.h"
#include "qwt_plot_picker.h"
#include "qwt_picker_machine.h"
#include "qwt_plot_directpainter.h"
#include "qwt_plot_zoomer.h"
#include "qwt_plot_panner.h"
#include "qwt_plot_magnifier.h"
#include "qwt_scale_widget.h"
#include "qwt_plot_marker.h"
#include "scalemagnifier.h"
#include "scaleplotmagnifier.h"
#include "scaleplotpanner.h"


#include <QDebug>
#include <QMap>
#include <QSize>

#if QT_VERSION>=0x040700
#include <QElapsedTimer>
#endif

class AbstractInterpolation;

class Plot : public QwtPlot
{
    Q_OBJECT
    //unidentified copy constructor
    Plot(const Plot&);
public:
    explicit Plot(QWidget *parent, ExperimentData* data);
    ~Plot();
    void addCurve(int yColumn, int xColumn=-1);
    double getMonitoringInterval(void) const;
    inline const AbstractInterpolation * getInterpolation() const {
        return interpolation;
    };

    QVector<QPointF> getSelectedPoints(QwtPlotCurve *curve=NULL) const;

public slots:
    void replot();
    void normalMode();
    void selectPointsMode(bool select=true);
    void selectT0(bool on=true);
    void drawLastPoint(int size);
    void clear(void);
    void zoomExtents(void);
    void initialize(void);
    void setIncrementalDraw(bool on=true);
    void setMonitoringInterval(double interval);
    void appendMarker(QPointF point,int control=0);
    void appendMarker(int rowIndex);

signals:
    void curvePointClicked(QwtPlotCurve *curve,int index);
    void xValueSelected(double value);
    void message(QString message);


private:
    const ExperimentData *experimentData;
    const QVector<QVector<double> > *dataTable;
    int xCol;
    QwtPlotCurve *selectedCurve;
    QMap<QwtPlotCurve *,QwtPlotCurve *> markCurveMap;
    QMap<QwtPlotCurve*, QVector<QPointF> > dataMap;
    int selectedPoint;
    QMap<int,QPointF> selectedPoints;
    QwtPicker *selectPointPicker;
    QwtPicker *selectRangePicker;
    QwtPicker *appendPointPicker;
    QwtPlotZoomer *zoomerLeft;
    QwtPlotZoomer *zoomerRight;
    QwtPlotZoomer *zoomerExclusiveLeft;
    QwtPlotZoomer *zoomerExclusiveRight;
    QwtPlotMagnifier *magnifier;
    ScalePlotMagnifier *yLeftMagnifier;
    ScalePlotMagnifier *yRightMagnifier;
    ScalePlotMagnifier *xBottomMagnifier;
    ScalePlotPanner *yLeftPanner;
    ScalePlotPanner *yRightPanner;
    ScalePlotPanner *xBottomPanner;

    bool incrementalDraw;
    double monitoringInterval;
    static int markerCount;
    AbstractInterpolation* interpolation;

private slots:
    void hidePlotItem(QwtPlotItem *plotItem, bool hide);

    /** Receives point coordinates of plot canvas and selects nearest point of all curves that are on the plot */

    void selectPoint(const QPoint &point);
    /** Selects range of points on current curve between selectedPoint and nearest point of the curve to the current click position.
      If no points were selected previously then it calls selectPoint();

      \sa selectedCurve, selectedPoint, selectPoint(), appendPoint();
      */
    void selectRange(const QPoint &point);

    /** Selects (or deselects) nearest point on the currently selected curve. If no curve selected then it calls selectPoint();

      \sa selectedCurve, selectedPoint, selectPoint(), selectRange();
      */
    void appendPoint(const QPoint &point);

    /** Returns true if curve point near clicked position (QPoint point) was selected or false if it wasn't.
      On success sets selectedCurve pointer and selectedPoint index or resets them on false.
      */
    bool getCurvePoint(const QPoint &point, QwtPlotCurve *curve=NULL);

    /** Draws a QwtPlotCurve with selectedPoints as data. This curve has z index
      of 50 and is drawn over all other curves.
*/
    void markSelectedPoints();



};

#endif // PLOT_H
