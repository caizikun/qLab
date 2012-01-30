#include "plot.h"


Plot::Plot(QWidget *parent, ExperimentData *data) :
        QwtPlot(parent)
{
    xCol=-1;
    selectedCurve=NULL; // set null pointer (i.e. no curve selected)
    selectedPoint=-1; //no point selected. 0 - means first point of the curve.
    experimentData=data;
    dataTable=data->getDataTable();
    setAutoDelete(true);
    setAutoReplot(false);

    //legend
    QwtLegend *legend = new QwtLegend(this);
    //legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    legend->setItemMode(QwtLegend::CheckableItem);
    insertLegend(legend, QwtPlot::BottomLegend);

    //click on legend item allows to hide items from the plot
    connect(this,SIGNAL(legendChecked(QwtPlotItem*,bool)),this, SLOT(hidePlotItem(QwtPlotItem*,bool)));

    //picker
    Picker *picker = new Picker (this);
    qDebug()<<"picker is enabled:"<<picker->isEnabled();

    //QwtPickerMachine *pickerMachine= new QwtPickerClickPointMachine;
    //QwtPickerMachine *pickerMachine= new PickerMachine;
    //picker->setStateMachine(pickerMachine);

    //connect(picker,SIGNAL(appended(QPoint)),SLOT(getSelectedCanvasPoints(QPoint)));
    connect(picker,SIGNAL(selectSignle(QPoint)),SLOT(selectPoint(QPoint)));
    connect(picker,SIGNAL(selectRange(QPoint)),SLOT(selectRange(QPoint)));
    connect(picker,SIGNAL(appendSingle(QPoint)),SLOT(appendPoint(QPoint)));

    //it's safe to call initialize even without data. It will reset plot to default state, add grids, etc.
    initialize();
}

Plot::~Plot() {
    clear();
}

void Plot::addCurve(int yColumn, int xColumn) {
    if (xColumn<0) xColumn=0;
    if (yColumn>dataTable->size() || yColumn<0 || xColumn>dataTable->size()) {
        qWarning()<<"yColumn is"<<yColumn<<"and xColumn is"<<xColumn<<"and number of columns is"<<dataTable->size();
        return;
    }
    QwtPlotCurve *plotCurve=new QwtPlotCurve(experimentData->getColumnLabel(yColumn));
    plotCurve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
    plotCurve->setLegendAttribute(QwtPlotCurve::LegendShowSymbol);
    SeriesData *series=new SeriesData(experimentData->getDataTable(), yColumn, xColumn);
    plotCurve->setData(series);

    // if curve should be plotted in xBottom-yRight axes
    if (experimentData->getColumnAxis(yColumn)==yRight) {
        plotCurve->setYAxis(yRight);
    }

    // specify a point marker (so called 'symbol') for curve
    QwtSymbol *symbol = new QwtSymbol (
            QwtSymbol::Triangle,        //default is the triangle
            QBrush(Qt::black),          //default color
            QPen(Qt::NoPen),            //empty pen
            QSize(7,7));                //hard-coded size
    plotCurve->setSymbol(symbol);
    plotCurve->attach(this);
}

void Plot::clear() {
    // set xCol to -1 to indicate that absciss axis is not defined in data object to prevent automatic curve generation
    xCol=-1;
    // detach all items from the plot
    detachItems( QwtPlotItem::Rtti_PlotItem, autoDelete() );
    //disable yRight and xTop axes if they were enabled for some reason
    enableAxis(yRight,false);
    enableAxis(xTop,false);
    replot();
}

void Plot::initialize() {
    // Clear plot from all items
    clear();
    // re-create plot grids
    QwtPlotGrid *grid=new QwtPlotGrid;
    grid->setMajPen(QPen(Qt::darkBlue,0,Qt::SolidLine));
    grid->attach(this);

    if (dataTable->size()<1) {
        qWarning()<<"dataTable is empty! Can not initialize";
        return;
    }
    for (int i=0;i<dataTable->size();i++) {
        if (experimentData->getColumnAxis(i)==xBottom) {
            qDebug()<<"Found xBottom at column"<<i;
            xCol=i;
        }
    }

    if (xCol<0) {
        qWarning()<<"Failed to find xBottom axis from data source";
        return;
    }

    bool rightGridEnabled=false;
    for (int i=0;i<dataTable->size();i++) {
        if (experimentData->getColumnAxis(i)!=xBottom && experimentData->getColumnAxis(i)!=xTop &&
                (experimentData->getColumnAxis(i)==yLeft || experimentData->getColumnAxis(i)==yRight) ) {
            qDebug()<<"Adding curve for column"<<i;

            // create grid for yRight if we have at least one curve at yRight axis
            if (experimentData->getColumnAxis(i)==yRight && !rightGridEnabled) {
                QwtPlotGrid *grid=new QwtPlotGrid;
                grid->setMajPen(QPen(Qt::darkRed,0,Qt::SolidLine));
                // disable grid for X axis, because vertical grid will be painted using grid attached to xLeft axis
                grid->enableX(false);
                grid->setYAxis(yRight);
                grid->attach(this);
            }

            // enable yRight axis
            enableAxis(experimentData->getColumnAxis(i));

            // add curve to the plot
            addCurve(i,xCol);
        }
    }
}

void Plot::hidePlotItem(QwtPlotItem *plotItem, bool hide)
{
    qDebug()<<"Hiding item"<<hide;
    plotItem->setVisible(!hide);
    replot();
}

void Plot::markCurvePoint(QwtPlotCurve *curve, int from, int to) {
    QwtSymbol *symbol = const_cast<QwtSymbol *>(curve->symbol());
    //remember brush
    const QBrush brush = symbol->brush();
        symbol->setBrush(QBrush(QColor(Qt::red)));
        selectedCurve=curve;
        selectedPoint=from;

    QwtPlotDirectPainter directPainter;

    //if to defaulting to -1 then we want to mark just one curve point
    if (to==-1) to=from;
    directPainter.drawSeries(curve, from, to);
    symbol->setBrush(brush); // reset brush

}

void Plot::unmarkCurvePoint(QwtPlotCurve *curve, int from, int to) {
    QwtPlotDirectPainter directPainter;
    //if "to" defaulting to -1 then we want to mark just one curve point
    if (to==-1) to=from;
    directPainter.drawSeries(curve, from, to);
}


void Plot::replot()
{
    QwtPlot::replot();
    if (selectedCurve!=NULL && !selectedPoints.isEmpty()) {
        for (QMap<int,QPointF>::const_iterator i=selectedPoints.constBegin();i!=selectedPoints.constEnd();i++) {
            markCurvePoint(selectedCurve,i.key());
        }
//        QMap<int, QPointF>::const_iterator i = selectedPoints.constBegin();
//         while (i != map.constEnd()) {

//             ++i;
//         }
    }
}

void Plot::clearPointSelection() {
    QwtPlotCurve *curve = NULL;
    QwtPlotDirectPainter directPainter;

    const QwtPlotItemList& itmList = itemList();
    //iterate over all plot items
    for ( QwtPlotItemIterator it = itmList.begin();
        it != itmList.end(); ++it )
    {
        // if plot item is of type PlotCurve then find closest point
        if ( (*it)->rtti() == QwtPlotItem::Rtti_PlotCurve)
        {
            curve= (QwtPlotCurve*)(*it);
            if (curve->isVisible()) {
                directPainter.drawSeries(curve,0,curve->dataSize()-1);
            }
        }
    }
}

void Plot::selectPoint(const QPoint &point) {
    selectedCurve=NULL;
    selectedPoint=-1;
    selectedPoints.clear();
    clearPointSelection();

    if (getCurvePoint(point)) {
        markCurvePoint(selectedCurve,selectedPoint);
        selectedPoints.insert(selectedPoint,point);
    } else {
        selectedCurve=NULL;
        selectedPoint=-1;
        selectedPoints.clear();
    }


}

void Plot::selectRange(const QPoint &point)
{
    if (selectedCurve==NULL) {
        selectPoint(point);
        return;
    }

    int i=selectedPoint;
    if (getCurvePoint(point,selectedCurve)) {
        if (i<selectedPoint) {
            for (int j=i;j<=selectedPoint;j++) {
                selectedPoints.insert(j,selectedCurve->sample(j));
            }
        } else if (i>selectedPoint) {
            for (int j=selectedPoint;j<=i;j++) {
                selectedPoints.insert(j,selectedCurve->sample(j));
            }
        } // else if (i==selectedPoint) do nothing :)
        markCurvePoint(selectedCurve,i,selectedPoint);
    }
}

void Plot::appendPoint(const QPoint &point)
{
    if (selectedCurve==NULL) {
        selectPoint(point);
        return;
    }

    if (getCurvePoint(point,selectedCurve)) {
        if (selectedPoints.contains(selectedPoint)) {
            qDebug()<<"Removing point from selection";
            selectedPoints.remove(selectedPoint);
            unmarkCurvePoint(selectedCurve,selectedPoint);
        } else {
            qDebug()<<"Appending point to selection";
            selectedPoints.insert(selectedPoint,selectedCurve->sample(selectedPoint));
            markCurvePoint(selectedCurve,selectedPoint);
        }

    } else {
        qDebug()<<"Failed to get curve point near"<<point;
    }
}

bool Plot::getCurvePoint(const QPoint &point, QwtPlotCurve *curve)
{
    qDebug()<<"Point selected:"<<point;
    double dist = 10e10;
    int index = -1;
    if (curve!=NULL) {
        index=curve->closestPoint(point,&dist);
    } else {
        const QwtPlotItemList& itmList = itemList();
        //iterate over all plot items
        for ( QwtPlotItemIterator it = itmList.begin();
             it != itmList.end(); ++it )
        {
            // if plot item is of type PlotCurve then find closest point
            if ( (*it)->rtti() == QwtPlotItem::Rtti_PlotCurve && (QwtPlotCurve*)(*it)->isVisible())
            {
                QwtPlotCurve *c = (QwtPlotCurve*)(*it);
                double d;
                int idx = c->closestPoint(point, &d);
                if ( d < dist )
                {
                    curve = c;
                    index = idx;
                    dist = d;
                }
            }
        }
    }
    // if there's a curve and index and it's closer than 15px then emit signal
    if (curve!=NULL && index !=-1 && dist < 15) {
        qDebug()<<"Curve point"<< index<<"selected on curve"<<curve->title().text();
        selectedCurve=curve;
        selectedPoint=index;
        emit curvePointClicked(curve,index);
        return true;
    } else {
        return false;
    }
}
