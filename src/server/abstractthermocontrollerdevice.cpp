#include "abstractthermocontrollerdevice.h"

AbstractThermocontrollerDevice::AbstractThermocontrollerDevice(QObject *parent):
        AbstractDevice(parent)
{
    setControlLoopNum(1);

    controlEngaged.fill(false);
    P.fill(0.0);
    I.fill(0.0);
    D.fill(0.0);
    /* Don't need to fill QStrings as they are constructed empty
    controlChannel.fill();
    targetValue.fill();
    */
}


bool AbstractThermocontrollerDevice::isControlEngaged(int controlLoop) const {
    return controlEngaged.at(controlLoop);
}

void AbstractThermocontrollerDevice::engageControl(bool engage, int controlLoop) {
    controlEngaged[controlLoop]=engage;
}

double AbstractThermocontrollerDevice::getP(int controlLoop) const {
    return P.at(controlLoop);
}

double AbstractThermocontrollerDevice::getI(int controlLoop) const {
    return I.at(controlLoop);
}

double AbstractThermocontrollerDevice::getD(int controlLoop) const {
    return D.at(controlLoop);
}

void AbstractThermocontrollerDevice::setP(double p, int controlLoop) {
    if (p>0) {
        P[controlLoop]=p;
    } else {
        qWarning()<<"P should be >0"<<p;
    }
}

void AbstractThermocontrollerDevice::setI(double i, int controlLoop) {
    if (i>0) {
        I[controlLoop]=i;
    } else {
        qWarning()<<"I should be >0"<<i;
    }
}

void AbstractThermocontrollerDevice::setD(double d, int controlLoop) {
    if (d>0) {
        D[controlLoop]=d;
    } else {
        qWarning()<<"D should be >0"<<d;
    }
}

QString AbstractThermocontrollerDevice::getTargetValue(int controlLoop) const {
    return targetValue.at(controlLoop);
}

int AbstractThermocontrollerDevice::getControlLoopNum() const {
    return controlLoopNum;
}

void AbstractThermocontrollerDevice::setControlLoopNum(int loops) {
    if (loops<1) return;
    controlLoopNum=loops;
    controlEngaged.resize(loops);
    P.resize(loops);
    I.resize(loops);
    D.resize(loops);
    controlChannel.resize(loops);
    targetValue.resize(loops);
}
