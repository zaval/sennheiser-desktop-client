//
// Created by Dmytrii Zavalnyi on 21.10.2024..
//

#include "DeviceInfoCardHelper.h"
#include "gaiaV3/GAIARfcommClient.h"

using namespace Qt::Literals::StringLiterals;

DeviceInfoCardHelper::DeviceInfoCardHelper(QObject *parent) :
    QObject(parent),
    service_SystemReleaseVersion(GAIARfcommClient::getInstance().getManager()->getProperty("\x04\x95\x13\x01"_ba)),
    battery_Percent(GAIARfcommClient::getInstance().getManager()->getProperty("\x04\x95\x07\x03"_ba)),
    versions_ModelId(GAIARfcommClient::getInstance().getManager()->getProperty("\x04\x95\x13\x06"_ba)),
    core_SerialNumber(GAIARfcommClient::getInstance().getManager()->getProperty("\x00\x1D\x01\x03"_ba)),
    sound_CodecUsed(GAIARfcommClient::getInstance().getManager()->getProperty("\x04\x95\x09\x00"_ba)),
    m_batteryPercent(0),
    m_releaseVersion(""),
    m_modelId("")
{
    if (battery_Percent != nullptr){
        connect(battery_Percent, &GAIAPropertyBase::valueChanged, this, &DeviceInfoCardHelper::setBatteryPercentValue);
        battery_Percent->sendGet();
    }
    if (service_SystemReleaseVersion != nullptr){
        connect(service_SystemReleaseVersion, &GAIAPropertyBase::valueChanged, this, &DeviceInfoCardHelper::setSystemReleaseVersion);
        service_SystemReleaseVersion->sendGet();
    }
    if (versions_ModelId != nullptr){
        connect(versions_ModelId, &GAIAPropertyBase::valueChanged, this, &DeviceInfoCardHelper::setModelId);
        versions_ModelId->sendGet();
    }
    if (core_SerialNumber != nullptr){
        connect(core_SerialNumber, &GAIAPropertyBase::valueChanged, this, &DeviceInfoCardHelper::setSerialNumber);
        core_SerialNumber->sendGet();
    }
    sound_CodecUsed->sendGet();
}

QString DeviceInfoCardHelper::getReleaseVersion() const {
    return m_releaseVersion;
}

int DeviceInfoCardHelper::getBatteryPercent() const {
    return m_batteryPercent;
}

QString DeviceInfoCardHelper::getDeviceAddress() const {
    return GAIARfcommClient::getInstance().getDeviceAddress();
}

void DeviceInfoCardHelper::setBatteryPercentValue() {
    m_batteryPercent = battery_Percent->getValue().at(0).toInt();
    Q_EMIT batteryPercentChanged();
}

void DeviceInfoCardHelper::setSystemReleaseVersion() {
    const auto releaseValue = service_SystemReleaseVersion->getValue();
    if (releaseValue.length() < 3){
        return;
    }
    m_releaseVersion = u"%1.%2.%3"_s
            .arg(QString::number(releaseValue[0].toUInt()))
            .arg(QString::number(releaseValue[1].toUInt()))
            .arg(QString::number(releaseValue[2].toUInt()));
    Q_EMIT releaseVersionChanged();
}

void DeviceInfoCardHelper::setModelId() {
    m_modelId = versions_ModelId->getValue().at(0).toString().trimmed().replace("\u0000"_ba, "");
    Q_EMIT modelIdChanged();
}

QString DeviceInfoCardHelper::getModelId() const {
    return m_modelId;
}

QString DeviceInfoCardHelper::getDeviceName() const {
    return GAIARfcommClient::getInstance().getDeviceName();
}

QString DeviceInfoCardHelper::getSerialNumber() const {
    return m_serialNumber;
}

void DeviceInfoCardHelper::setSerialNumber() {
    const auto &sn = core_SerialNumber->getValue();
    if (sn.length() < 1){
        return;
    }
    m_serialNumber = sn.at(0).toString().trimmed().replace("\u0000"_ba, "");
    Q_EMIT serialNumberChanged();
}
