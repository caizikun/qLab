#ifndef GPIBTHERMOCONTROLLERDEVICE_H
#define GPIBTHERMOCONTROLLERDEVICE_H

#include "abstractthermocontrollerdevice.h"
#include "gpibdevice.h"
#include <QStringList>
#include <QByteArray>

class GpibThermocontrollerDevice : public GpibDevice,
public AbstractThermocontrollerDevice

{
    Q_OBJECT

public:
    GpibThermocontrollerDevice(QByteArray &shortName, Experiment *exp=0);
    virtual bool setTargetValue(QString value, int loopIndex=0);
    virtual bool setControlChannel(QString channel, int loopIndex=0);
    virtual QString getControlChannel(int loopIndex=0);
    virtual QString getControlPower(int controlLoop=0);
    virtual bool setCurrentControlTypeIndex(int typeIndex, int loopIndex);

private:
    QString engageControlsCommand,
    disengageControlsCommand,
    statusControlsCommand,
    statusControlsEngagedResponse,
    setControlChannelCommand,
    getControlChannelCommand,
    setTypeControlCommand,
    getTypeControlCommand,
    offControlCommand,
    pidControlCommand,
    getPControlCommand,
    getIControlCommand,
    getDControlCommand,
    setPControlCommand,
    setIControlCommand,
    setDControlCommand,
    getControlTargetCommand,
    setControlTargetCommand,
    getControlPowerCommand,
    getControlPowerRangeCommand;

    QString getTargetValue(int controlLoop);

};

#endif // GPIBTHERMOCONTROLLERDEVICE_H
