#ifndef QGPIBDEVICE_H
#define QGPIBDEVICE_H

#ifdef WIN32
#include "gpib-32.h"
#endif

#include "qabstractdevice.h"
#include <QDebug>
#include <QByteArray>
#include <QStringList>
#include <QSettings>
#include <QApplication>

class QGpibDevice : public QAbstractDevice
{
    Q_OBJECT
public:
    explicit QGpibDevice(QByteArray &DeviceShortName, QObject *parent = 0);
    ~QGpibDevice();
    int Id(void) const;
    int Handle(void) const;
    /** Implementation of QAbstractDevice method. Reads value from specified
      channel. If channel is not specified or if channel==0 then default
    or last used channel is used.
    */
    bool readValue(QByteArray &returnValue,int channel=0);

    /** Checks if device is online */
    bool isOnline(void);

    /** Resets the device. It should be used if some errors occur with device */
    void resetDevice();

private:

    /// gpib external variables allocation
    int ibsta,iberr,ibcnt;
    long int ibcntl;

    /// gpib id
    short int id;

    /// current gpib channel
    int channel;

    /// handle of the device
    int handle;

    /// device-specific settings object
    QSettings *deviceSettings;
    /// experiment specific settings object
    QSettings *experimentSettings;

    /** Command that will cause gpib device to read channel
      This string can contain %n that will be replaced with channel number (integer),
      so it's possible to specify one string to read data from many channels
      */
    QString readCommand;

    /** Some devices (k2700 for example) needs separate command to switch to channel.
      If channelCommand is not empty than it will be triggered on device init and on every request for data from non-last channel
      */
    QString channelCommand;


    bool set (QString command);
    bool get (QByteArray &reply);
    bool ask (QString command, QByteArray &reply);
    bool getIdn(void);

signals:

public slots:

};

#endif // QGPIBDEVICE_H
