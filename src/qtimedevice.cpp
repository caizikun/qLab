#include "qtimedevice.h"
#include <QByteArray>

QTimeDevice::QTimeDevice(QObject *parent) :
        QAbstractDevice(parent)
{
    bus=Time;
    shortname="time";
    setLabel("Time");
    setUnit("H:mm:ss");
}

bool QTimeDevice::readValue(QByteArray &returnValue, QStringList) {
    returnValue=QTime::currentTime().toString("H:mm:ss").toAscii();
    return true;
}
