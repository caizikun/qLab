#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>

class TcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);


signals:

public slots:

    void askInitialData();
    void getData();

};

#endif // TCPCLIENT_H
