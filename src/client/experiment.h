#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QObject>
#include <QStringList>
#include "tcpclient.h"

class Experiment : public QObject
{
Q_OBJECT
public:
    explicit Experiment(TcpClient *,QObject *parent = 0);

signals:

public slots:
    void setInterval(int interval);
    int getInterval() const;

    QStringList getControl() const;
    void setControl(QStringList controls);

    void setControlTarget(int controlIndex,QString target);
    QString getControlTarget(int controlIndex) const;
    void initialize();

private:
    int interval;
    QStringList controlList;
    QStringList targetList;
    TcpClient *tcpClient;

};

#endif // EXPERIMENT_H
