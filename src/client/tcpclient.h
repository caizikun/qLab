#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QBuffer>
#include <QStringList>



class TcpClient : public QTcpSocket
{
    Q_OBJECT
    //unidentified copy constructor
    TcpClient(const TcpClient&);
public:
    explicit TcpClient(QObject *parent = 0);
    enum Expect {
        expectCommand,
        expectData,
        expectInterval,
        expectControls,
        expectTarget,
    };

    enum QueryRequest {
        queryInitialData,
        queryInterval,
        queryControls,
        queryStatus,
    };

private:
    QByteArray buffer;
    Expect expect;
    void setExpect(Expect Expectation);
    Expect getExpect(void) const;
    static int readLineCount;
    QStringList tmpStringList;


signals:

    /// Signal is emitted when we have new line with comment or data to be passed to ExperimentData
    void dataLine(QByteArray &dataLine);
//    /// Signal is emitted when new portion of data is arrived to buffer;
//    void newDataArrived(void);

    /// Signal is emitted when initial data is come. It should be connected to ExperimentData to clear the data
    void initialData(void);
    /// Signal is emitted when server stops to measure
    void serverStatus (bool running);
    /// Signal is emitted when server changes measuring interval
    void serverInterval(int interval);

    void bytesWritten(int bytes);
    void bytesRead(int bytes);
    void serverControlTarget(int controlIndex, QString target);
    void serverControlList(QStringList controlList);

public slots:

    inline void askInitialData() {query(queryInitialData);}

    void getData();
    /** Slot gets the line from network and process it.
      It can emit dataLine signal to ExperimentData if it's comment or data line.
      Or it can emit other signals to update app information about the server (i.e. interval change or status change)
      */
    void protocolParser(QByteArray& line);
    void query(QueryRequest q);

    void setMeasureInterval(int interval);

private slots:
    void bytesWritten(qint64 bytes);


};


#endif // TCPCLIENT_H
