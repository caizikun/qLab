#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent)
{
    quint16 port=25050;
    while (!listen(QHostAddress::Any,port) && port<65535) {
        qWarning()<<"Failed to listen port"<<port;
        ++port;
    }
    qDebug()<<"Listening on port"<<serverPort();
    connect(this,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    experiment=0;
}

void TcpServer::acceptConnection() {
    while (hasPendingConnections()) {
        QTcpSocket *socket=nextPendingConnection();
        //remember connection
        clientSocket.insert(socket);
        //set monitoring mode off (it will be swithced to on when client ask for initial data
        setClientMonitoringMode(socket,false);

        //inform that we have +1 to connection count
        emit clientCountChanged(clientSocket.count());

        qDebug()<<"Client connected. Client count:"<<getClientCount();
        connect(socket,SIGNAL(disconnected()),this,SLOT(removeConnection()));
        connect(socket,SIGNAL(readyRead()),this,SLOT(readCommand()));
        socket->write("You are connected to qLab server\n");

        if (!experiment) {
            qWarning()<<"No pointer to experiment object set in TcpServer class";
            continue;
        }

    }
}

void TcpServer::removeConnection() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) return;

    if (clientSocket.remove(socket)) {
        clientSocketMonitorMode.remove(socket);
        emit clientCountChanged(clientSocket.count());
    }
    else {
        qWarning()<<"";
    }
    qDebug()<<"Client disconnected. Client count:"<<getClientCount();
    socket->deleteLater();
}



int TcpServer::getClientCount() const {
    return clientSocket.count();
}

void TcpServer::setExperiment(Experiment *experiment) {
    this->experiment=experiment;
    //measured data from experiment will go to all clients that are in monitoring mode
    connect(experiment,SIGNAL(measured(QString)),SLOT(measured(QString)));
}

void TcpServer::readCommand() {
    //Notice: I think of pointer type conversion as of dirty hack.
    //This was took from bittorrent qt example.
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    QByteArray peer;
    qint64 bufsize;

    //set peer as string containging "ip:port"
    peer.append(socket->peerAddress().toString().toAscii()).append(':').append(QByteArray::number(socket->peerPort()));

    //if no bytes available in the socket then quit
    if (socket->bytesAvailable()<1) {
        qWarning()<<"bytesAvailable() ="<<socket->bytesAvailable()<<
                "peer"<<peer;
        return;
    }
    bufsize=socket->bytesAvailable();
    QByteArray buf;
    qDebug()<<"Bytes available to read from client"<<peer<<bufsize;
    if (!socket->canReadLine()) {
        qWarning()<<"Could not read line from client";
        qDebug()<<socket->readAll();
        return;
    }

    buf=socket->read(bufsize);
    if (buf.size()!=bufsize) {
        qWarning()<<"Read"<<buf.size()<<"bytes of"<<bufsize;
        qDebug()<<"Buffer:"<<buf;
    }
    //here we gonna recognize request and send an answer
    buf=buf.trimmed();
    if (buf=="ping") {
        socket->write("200 pong\n");
        return;
    }
    if (buf=="help") {
        socket->write("200 Help:\n"
                      "quit - close this channel\n"
                      "exit - close this channel\n"
                      "ping - ask if server is online. 'pong' reply should be get\n"
                      "version - program version\n"
                      "get interval - measuring interval in msec\n"
                      "set interval=NUMBER - set measuring interval in msec\n"
                      "get initial data - get initial experimental data with header\n"
                      "monitor on - set monitoring mode on\n"
                      "monitor off - set monitoring mode off\n"
                      ""
                      ""
                      "");
        return;
    }

    if (buf=="get interval") {
        socket->write("200 Interval:\n"+QByteArray::number(experiment->getInterval())+'\n');
        return;
    }
    if (buf.startsWith("set interval=")) {
        bool ok=false;
        int interval=buf.remove(0,13).trimmed().toInt(&ok);
        if (ok) {
            experiment->setInterval(interval);
        } else {
            qWarning()<<"Failed to get interval from request:\n"<<buf;
            socket->write("400 Bad request\nInterval should be specified as a number");
        }
        return;
    }

    if (buf=="get latest") {
        //socket->write(experiment->dataStringList)
    }

    if (buf=="get initial data") {
        socket->write(experiment->getHeader().toAscii());
        socket->write(experiment->getData().toAscii());
        socket->write("\n");
        setClientMonitoringMode(socket, true);
        return;
    }

    if (buf=="monitor on") {
        setClientMonitoringMode(socket,true);
        socket->write("200 Monitoring on\n");
        return;
    }

    if (buf=="monitor off") {
        setClientMonitoringMode(socket,false);
        socket->write("200 Monitoring off\n");
        return;
    }

    if (buf=="get monitor") {
        if (getClientMonitoringMode(socket)) {
            socket->write("200 Monitoring on\n");
        } else {
            socket->write("200 Monitoring off\n");
        }
        return;
    }

    if (buf=="start") {
        experiment->start();
        return;
    }

    if (buf=="stop") {
        experiment->stop();
        return;
    }

    if (buf=="status") {
        if (experiment->isActive())
           socket->write("200 Running\n");
        else
           socket->write("200 Idle\n");
        return;
    }


    if (buf=="version") {
        socket->write(QCoreApplication::applicationName().toAscii()+" version "+QCoreApplication::applicationVersion().toAscii()+'\n');
        return;
    }

    if (buf=="quit" || buf=="exit") {
        socket->write("200 Bye!\n");
        socket->disconnectFromHost();
        return;
    }


    //if we did not recogized request
        socket->write("400 Bad request\nType 'help' for full list of supported commands\n");
}

void TcpServer::experimentStatusChanged(bool running) {
    QByteArray message;
    if (running) {
        message="200 Running\n";
    } else {
        message="200 Idle\n";
    }
    foreach (QTcpSocket *socket, clientSocket) {
        socket->write(message);
    }
}

void TcpServer::experimentIntervalChanged(int msec) {
    if (msec<100) {
        qWarning()<<"Interval is too low!"<<msec;
        return;
    }
    foreach (QTcpSocket *socket, clientSocket) {
        socket->write("200 Interval:\n"+QByteArray::number(msec)+'\n');
    }
}

void TcpServer::experimentForbidden(QString message) {
    foreach (QTcpSocket *socket, clientSocket) {
        socket->write("403 "+message.toAscii()+'\n');
    }
}

void TcpServer::disconnectClients() {
    foreach (QTcpSocket *socket, clientSocket) {
        socket->write("200 You are about to be disconnected. Bye!\n");
        socket->disconnectFromHost();
    }
}

void TcpServer::setClientMonitoringMode(QTcpSocket *socket, bool on) {
    if (!socket) {
        qWarning()<<"Empty socket to set monitoring mode on";
        return;
    }
    clientSocketMonitorMode.insert(socket,on);
}

bool TcpServer::getClientMonitoringMode(QTcpSocket *socket) const {
    if (!socket) {
        qWarning()<<"Empty socket to get monitoring mode from";
        return false;
    }
    return clientSocketMonitorMode.value(socket);
}

void TcpServer::measured(QString dataLine) {
    QHashIterator<QTcpSocket *, bool> i(clientSocketMonitorMode);
    while (i.hasNext()) {
        i.next();
        if (i.value()) {
            i.key()->write(dataLine.toAscii().append("\n"));
        }
    }
}
