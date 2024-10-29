#ifndef SENNHEISERDESKTOPCLIENT_DEVICEINFOCARDHELPER_H
#define SENNHEISERDESKTOPCLIENT_DEVICEINFOCARDHELPER_H


#include <QObject>
#include <QtQmlIntegration>
#include "gaiaV3/GAIAPropertyBase.h"

class DeviceInfoCardHelper: public QObject{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString releaseVersion READ getReleaseVersion NOTIFY releaseVersionChanged)
    Q_PROPERTY(int batteryPercent READ getBatteryPercent NOTIFY batteryPercentChanged)
    Q_PROPERTY(QString deviceAddress READ getDeviceAddress NOTIFY deviceAddressChanged)
    Q_PROPERTY(QString modelId READ getModelId NOTIFY modelIdChanged)
    Q_PROPERTY(QString deviceName READ getDeviceName NOTIFY deviceNameChanged)
    Q_PROPERTY(QString serialNumber READ getSerialNumber NOTIFY serialNumberChanged)

private:
    QString m_releaseVersion;
    int m_batteryPercent;
    QString m_modelId;
    QString m_serialNumber;

    GAIAPropertyBase *service_SystemReleaseVersion;
    GAIAPropertyBase *battery_Percent;
    GAIAPropertyBase *versions_ModelId;
    GAIAPropertyBase *core_SerialNumber;
    GAIAPropertyBase *sound_CodecUsed;

public:
    explicit DeviceInfoCardHelper(QObject *parent = nullptr);

private slots:
    void setBatteryPercentValue();
    void setSystemReleaseVersion();
    void setModelId();
    void setSerialNumber();

public slots:
    QString getReleaseVersion() const;
    int getBatteryPercent() const;
    QString getDeviceAddress()const;
    QString getModelId()const;
    QString getDeviceName()const;
    QString getSerialNumber()const;

signals:
    void releaseVersionChanged();
    void batteryPercentChanged();
    void deviceAddressChanged();
    void modelIdChanged();
    void deviceNameChanged();
    void serialNumberChanged();
};


#endif //SENNHEISERDESKTOPCLIENT_DEVICEINFOCARDHELPER_H
