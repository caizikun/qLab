#include "gpibthermocontrollerdevice.h"

GpibThermocontrollerDevice::GpibThermocontrollerDevice(QByteArray &shortName, Experiment *exp):
        GpibDevice(shortName, exp),
        AbstractThermocontrollerDevice()

{
    setCapability("thermocontroller");
    //entering device options group to read values
    qDebug()<<deviceSettings->group();
    deviceSettings->beginGroup(shortName);
    qDebug()<<deviceSettings->group();
    //reading channels string
    QString channels=deviceSettings->value("channels").toString();
    //reading controls string
    QString controls=deviceSettings->value("controls").toString();
    //split comma-separated values in string to stringlist
    QStringList channelList=channels.split(",",QString::SkipEmptyParts);
    QStringList controlList=controls.split(",",QString::SkipEmptyParts);

    //set controlloopnum to the controlList.size()
    if (controlList.size()>0) {
        qDebug()<<"Setting loop num of"<<shortname<<"to"<<controlList.size();
        setControlLoopNum(controlList.size());
    } else {
        qWarning()<<"Control loop num read from config for"<<shortname<<"is"<<controlList.size();
    }

    //setting loop names as they should appear in gpib commands
    for (int i=0;i<controlList.size();++i) {
        loopName[i]=controlList.at(i).trimmed();
    }

    //initializing gpib commands from config
    engageControlsCommand=deviceSettings->value("control/engage").toString();
    disengageControlsCommand=deviceSettings->value("control/disengage").toString();
    statusControlsCommand=deviceSettings->value("control/status").toString();
    statusControlsEngagedResponse=deviceSettings->value("control/status-on").toString();
    setTypeControlCommand=deviceSettings->value("control-set/type").toString();
    getTypeControlCommand=deviceSettings->value("control/type").toString();
    controlTypes=deviceSettings->value("control/types").toString();
    controlTypeList=controlTypes.split(",");
    offControlCommand=deviceSettings->value("control/type-off").toString();
    pidControlCommand=deviceSettings->value("control/type-pid").toString();
    getControlTargetCommand=deviceSettings->value("control/target").toString();
    setControlTargetCommand=deviceSettings->value("control-set/target").toString();
    getPControlCommand=deviceSettings->value("control/P").toString();
    getIControlCommand=deviceSettings->value("control/I").toString();
    getDControlCommand=deviceSettings->value("control/D").toString();
    setPControlCommand=deviceSettings->value("control-set/P").toString();
    setIControlCommand=deviceSettings->value("control-set/I").toString();
    setDControlCommand=deviceSettings->value("control-set/D").toString();
    getControlChannelCommand=deviceSettings->value("control/channel").toString();
    setControlChannelCommand=deviceSettings->value("control-set/channel").toString();
    getControlPowerCommand=deviceSettings->value("control/power").toString();
    getControlPowerRangeCommand=deviceSettings->value("control/power-range").toString();


//    QStringList expControls=experimentSettings->value("controls").toString().split(",",QString::SkipEmptyParts);

    deviceSettings->endGroup();

}

bool GpibThermocontrollerDevice::setTargetValue(QString value, int loopIndex) {
    //FIXME: arg() not works ok if %1 is not specified on the string
    //this causes error for lakeshore330 because no %1 in set target
    QString command=setControlTargetCommand.arg(loopName.at(loopIndex),value).toLatin1();
    qDebug()<<shortname<<"issue command"<<command;
    if (set(command.toLatin1())) {
        qDebug()<<shortname<<"target for loop"<<loopName.at(loopIndex)<<"set"<<value;
        targetValue[loopIndex]=value;
        return true;
    } else {
        qWarning()<<shortname<<"Failed to set target value"<<value;
        return false;
    }
}

bool GpibThermocontrollerDevice::setControlChannel(QString channel, int loopIndex) {
    QString command=setControlChannelCommand.arg(loopName.at(loopIndex),channel);
    qDebug()<<shortname<<"issue command"<<command;
    if (set(command.toLatin1())) {
        qDebug()<<shortname<<"channel for loop"<<loopName.at(loopIndex)<<"set"<<channel;
        controlChannel[loopIndex]=channel;
        return true;
    } else {
        qWarning()<<shortname<<"Failed to set channel"<<channel;
        return false;
    }
}

QString GpibThermocontrollerDevice::getControlChannel(int loopIndex) {
    if (loopIndex<0) {
        qWarning()<<"Was trying to get control channel of loop"<<loopIndex;
        return "";
    }
    if (loopIndex>=controlChannel.size()) {
        qWarning()<<"Was trying to get control channel of loop"<<loopIndex;
        return "";
    }

    return controlChannel.at(loopIndex);
}

bool GpibThermocontrollerDevice::setCurrentControlTypeIndex(int typeIndex, int loopIndex) {
    if (typeIndex==getCurrentControlTypeIndex(loopIndex)) {
        return true;
    }

    QString command=setTypeControlCommand.arg(loopName.at(loopIndex),getControlTypesList().at(typeIndex));
    qDebug()<<shortname<<"issue command"<<command;
    if (set(command.toLatin1())) {
        qDebug()<<shortname<<"control type for loop"<<loopName.at(loopIndex)<<"set"<<typeIndex;
        currentControlType[loopIndex]=typeIndex;
        return true;
    } else {
        qWarning()<<shortname<<"Failed to set control type index"<<typeIndex;
        return false;
    }
}

QString GpibThermocontrollerDevice::getTargetValue(int controlLoop) {
    QString target=AbstractThermocontrollerDevice::getTargetValue(controlLoop);
    if (!target.isEmpty()) {
        return target;
    }
    QString loopName=getLoopName(controlLoop);
    QByteArray reply;
    if (ask(getControlTargetCommand.arg(loopName).toLatin1(),reply)) {
        return QString(reply);
    } else {
        qWarning()<<"Failed to get target value of loop"<<loopName;
    }
    return "Error";
}

QString GpibThermocontrollerDevice::getControlPower(int loopIndex) {
    if (getControlPowerCommand.isEmpty()) {
        qWarning()<<"No control/power command specified for the device. Returning -0.000";
        return "-0.000";
    }
    QString loopName=getLoopName(loopIndex);

    QByteArray reply;
    if (ask(getControlPowerCommand.arg(loopName).toLatin1(),reply)) {
        qDebug()<<"Control power is"<<reply;
        if (reply.contains("%")) {
            reply.replace("%","");
        }
        //FIXME: reply string probably needs to remove % sign
        return QString(reply);
    } else {
        qWarning()<<"Failed to read control power with command"<<getControlPowerCommand;
        return "-0.000";
    }
}
