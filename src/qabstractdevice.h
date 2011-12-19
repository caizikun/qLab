#ifndef QABSTRACTDEVICE_H
#define QABSTRACTDEVICE_H

#include <QObject>
#include <QByteArray>

/// Bus type enumertion. Gpib bus or some other.
enum deviceBusType {
    Gpib,
    Multi
};

class QAbstractDevice : public QObject
{
    Q_OBJECT
public:
    //
    explicit QAbstractDevice(enum deviceBusType busType=Gpib, QObject *parent = 0);
    ~QAbstractDevice();


    /** The MAIN function of the deivce :)
      */
    virtual bool readValue(int channel, QByteArray &returnValue)=0;

    /// Second main function :)
    virtual void resetDevice(void)=0;

    /// Amount of received data from physical device/driver
    //virtual long int rx(void)=0;

    /// Amount of data transmitted to physical device/driver
    //virtual long int tx(void)=0;

    /// Number of all devices (instances of this class)
    static int deviceCount(void);

    /// Device number among all the devices
    int deviceNum(void) const;

    /// Bus type of the device. GPIB or some other hw bus
    deviceBusType busType(void) const;

    /// Shortcut for the device. It's used in .ini file to set up device properties
    const char *shortName(void) const;

private:
    /// Number of the instances of this class
    static int numdevices;

    /** Number of the current instance.
      Each device then can know that it is n'th of total m devices
    */
    int numdevice;

    /// Factor - is the coefficient that us ysed for
    double factor;
    double min_value;
    double max_value;
    double scale_hint;
    QString unit;
    QString label;





protected:
    /// Device bus type
    int bus;

    /** Device short name. It's used to refer to device in config files.
      Ex. k2700, cc34
      */
    QByteArray shortname;

    /** Device identification string (if available).
      Ex. Device identification can be obtained by 'IDN?' command for GPIB interface
      */
    QByteArray idn;



signals:
    void rx(int);
    void tx(int);
    void errorMessage (QString);

public slots:

};

#endif // QABSTRACTDEVICE_H
