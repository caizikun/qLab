#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this,SIGNAL(connected()),this,SLOT(askInitialData()));
    connect(this,SIGNAL(readyRead()),this,SLOT(getData()));
}

void TcpClient::askInitialData()
{
    this->write("get all\n");
}

void TcpClient::getData()
{

    if (bytesAvailable()<=0) {
        qWarning()<<"No bytes available to read from network";
        //return;
    }
    QByteArray buf;
    buf=readAll();
    qDebug()<<"Read buffer: "<<buf;
    if (buf.startsWith("400") || buf.startsWith("403") || buf.startsWith("404")) {
        qWarning()<<"Server said:"<<buf;
        return;
    }
    if (buf.startsWith("200 Initial data:")) {
//truncate first string (evertything before \n)
        buf=buf.right(buf.size()-buf.indexOf('\n')+1);
        emit initialData(buf);
    }


}
