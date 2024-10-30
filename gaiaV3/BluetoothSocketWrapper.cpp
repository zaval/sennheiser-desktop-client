#include <QTimer>
#include "BluetoothSocketWrapper.h"

using namespace Qt::Literals::StringLiterals;

BluetoothSocketWrapper::BluetoothSocketWrapper(QObject *parent) :
    QObject(parent),
    isDemo{false},
    m_socket(new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this)),
    m_dataMap(),
    m_queue()
{
    connect(m_socket, &QBluetoothSocket::connected, this, [this](){Q_EMIT connected();});
    connect(m_socket, &QBluetoothSocket::readyRead, this, [this](){Q_EMIT readyRead();});
    connect(m_socket, &QBluetoothSocket::disconnected, this, [this](){Q_EMIT disconnected();});
    prepareData();

}

bool BluetoothSocketWrapper::isOpen() const {
    if (isDemo){
        return true;
    }
    return m_socket->isOpen();
}

void BluetoothSocketWrapper::close() {
    if (isDemo){
        Q_EMIT disconnected();
    }
    m_socket->close();
}

QByteArray BluetoothSocketWrapper::readAll() {
    if (isDemo){
        if (m_queue.isEmpty()){
            return QByteArray();
        }

        const auto data = m_queue.dequeue();
        if (!m_queue.isEmpty()){
            Q_EMIT readyRead();
        }
        return data;

    }
    return m_socket->readAll();
}

void
BluetoothSocketWrapper::connectToService(const QBluetoothAddress &address, quint16 port, QIODevice::OpenMode openMode) {
    if (address.toString() == "11:11:11:11:11:11") {
        isDemo = true;
        Q_EMIT connected();
    } else {
        isDemo = false;
        m_socket->connectToService(address, port, openMode);
    }
}

qint64 BluetoothSocketWrapper::write(const QByteArray &data) {
    if (isDemo){
        if (m_dataMap.contains(data)){
            QTimer::singleShot(500, [this, data](){
                m_queue.enqueue(m_dataMap[data]);
                Q_EMIT readyRead();
            });
        }
        return data.size();
    }
    return m_socket->write(data);
}

BluetoothSocketWrapper::BluetoothSocketWrapper(QBluetoothSocket *socket)
    : QObject(nullptr),
      isDemo{false},
      m_socket(socket),
      m_dataMap(),
      m_queue()
{
    prepareData();
}

void BluetoothSocketWrapper::prepareData() {
    m_dataMap = {
            {"\xFF\x03\x00\x00\x04\x95\x06\x03"_ba, "\xFF\x03\x00\x01\x04\x95\x07\x03\x2a"_ba}, // Battery_Percent
            {"\xFF\x03\x00\x00\x04\x95\x12\x01"_ba, "\xFF\x03\x00\x06\x04\x95\x13\x01\x00\x2a\x00\x18\x00\x2a"_ba}, // Service_SystemReleaseVersion
            {"\xFF\x03\x00\x00\x04\x95\x12\x06"_ba, "\xFF\x03\x00\r\x04\x95\x13\x06MMMMBT Black\x00"_ba}, // Versions_ModelId
            {"\xFF\x03\x00\x00\x00\x1d\x00\x03"_ba, "\xFF\x03\x00\x0c\x00\x1D\x01\x03"_ba + "123456789012"_ba}, // Core_SerialNumber
            {"\xFF\x03\x00\x00\x04\x95\x08\x00"_ba, "\xFF\x03\x00\x01\x04\x95\x09\x00\x05"_ba}, // Sound_CodecUsed

            {"\xFF\x03\x00\x00\x04\x95\x1a\x01"_ba, "\xFF\x03\x00\x06\x04\x95\x1B\x01\x01\x02\x02\x00\x03\x01"_ba}, // ANC
            {"\xFF\x03\x00\x00\x04\x95\x1a\x03"_ba, "\xFF\x03\x00\x01\x04\x95\x1B\x03\x00"_ba}, // ANC_Transparency
            {"\xFF\x03\x00\x00\x04\x95\x1a\x05"_ba, "\xFF\x03\x00\x01\x04\x95\x1B\x05\x01"_ba}, // ANC_Status

            {"\xFF\x03\x00\x00\x04\x95\x08\x0B"_ba, "\xFF\x03\x00\x01\x04\x95\x09\x0B\x01"_ba}, // Setting_AutoCall
            {"\xFF\x03\x00\x00\x04\x95\x08\x15"_ba, "\xFF\x03\x00\x01\x04\x95\x09\x15\x01"_ba}, // Setting_ComfortCall

            {"\xFF\x03\x00\x00\x04\x95\x14\x00"_ba, "\xFF\x03\x00\x02\x04\x95\x15\x00\x00\x04"_ba}, // PairedDevicesListSize
            {"\xFF\x03\x00\x00\x04\x95\x14\x09"_ba, "\xFF\x03\x00\x01\x04\x95\x15\x09\x02"_ba}, // PairedDevicesMaxBTConnections
            {"\xFF\x03\x00\x00\x04\x95\x14\x07"_ba, "\xFF\x03\x00\x01\x04\x95\x15\x07\x00"_ba}, // PairedDevicesOwnDeviceIndex


            {"\xFF\x03\x00\x01\x04\x95\x14\x01\x00"_ba, "\xFF\x03\x00\x12\x04\x95\x15\x01\x00\x01\x01Motorola T2288\x00"_ba}, // PairedDevicesGetDeviceInfo
            {"\xFF\x03\x00\x01\x04\x95\x14\x01\x01"_ba, "\xFF\x03\x00\x12\x04\x95\x15\x01\x01\x00\x00Sega Mega Drive\x00"_ba}, // PairedDevicesGetDeviceInfo
            {"\xFF\x03\x00\x01\x04\x95\x14\x01\x02"_ba, "\xFF\x03\x00\x17\x04\x95\x15\x01\x02\x02\x00Sony Walkman TPS-L2\x00"_ba}, // PairedDevicesGetDeviceInfo
            {"\xFF\x03\x00\x01\x04\x95\x14\x01\x03"_ba, "\xFF\x03\x00\x0f\x04\x95\x15\x01\x03\x02\x01ZX Spectrum\x00"_ba}, // PairedDevicesGetDeviceInfo

            {"\xFF\x03\x00\x00\x04\x95\x08\x0d"_ba, "\xFF\x03\x00\x01\x04\x95\x09\x0d\x01"_ba}, // Setting_SmartPause
            {"\xFF\x03\x00\x00\x04\x95\x04\x01"_ba, "\xFF\x03\x00\x01\x04\x95\x05\x01\x01"_ba}, // Setting_OnHeadDetection
    };
}
