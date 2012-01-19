#ifndef EXPERIMENTDATA_H
#define EXPERIMENTDATA_H

#include <QAbstractTableModel>
#include <QVector>
#include <QDebug>
#include <QStringList>

class ExperimentData : public QAbstractTableModel
{
Q_OBJECT
public:
    explicit ExperimentData(QObject *parent = 0);
    /// Implementation of virtual member of QAbstractTableModel
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    /// Implementation of virtual member of QAbstractTableModel
    int columnCount(const QModelIndex &parent=QModelIndex()) const;
    /// Implementation of virtual member of QAbstractTableModel
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    /// Implementation of virtual member of QAbstractTableModel
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /// returns reference to QStringList containing ascii;
    const QStringList & getAscii() const;

    /// Enumeration of available expectations for parseComment() functions
    enum Expect {
        expectNone,
        expectDevice,
        expectLabel,
        expectMin,
        expectMax,
        expectUnit
    };

private:
    /// This is the DATA STORAGE for table of doubles
    QVector<QVector<double> > dataTable;
    /** The string list containing raw ascii data before conversion.

      Here we can find comment lines and lines containing measured data
      **/
    QStringList asciiTable;

    /// Variable that keeps type of next expected value (for comment parsing)
    Expect expect;

    /// Array of shortnames of the data columns
    QStringList columnShortname;
    /// Array of labels of the data columns
    QStringList columnLabel;
    /// Array of min values for columns
    QList<double> columnMin;
    /// Array of max values for columns
    QList<double> columnMax;
    /// Array of units for columns
    QStringList columnUnit;

    /// Access functions to expect variable
    inline void setExpect(Expect expectation);
    inline Expect getExpect(void) const;



signals:
    /// Emitted when new data is appended to dataTable array
    void newDataAvailable();


public slots:
    /// Resets dataTable and prepares to read data from file or from another server
    void resetData(void);
    /// Parses any incoming line of data. Line can contain comment or measured values separated by tabs (\t)
    void parseLine(QByteArray &line);
    /// Parses comment. This function is called from parseLine for strings that starts from # or * or //
    void parseComment(QByteArray &commentLine);
    /// Parses data. This function is called from parseLine for strings that starts from digit
    void parseData(QByteArray &dataLine);

};

#endif // EXPERIMENTDATA_H
