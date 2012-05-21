#include "testdevice.h"

TestDevice::TestDevice(Experiment *exp):
        AbstractDevice(exp)
{
}


bool TestDevice::readValue(QByteArray &returnValue, QStringList parameters) {
    if (parameters.isEmpty()) {
        returnValue="0.000";
    } else {
        returnValue=parameters.at(0).toAscii();
    }
    lastValue=returnValue;
    return true;
}
