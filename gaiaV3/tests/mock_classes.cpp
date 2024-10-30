//
// Created by Dmytrii Zavalnyi on 30.10.2024..
//

#include "mock_classes.h"

GAIAPropertyBase *MockGAIAPropertyManager::getPropertyFromVendorCommand(const quint16 &vendorId, const quint16 &commandId)  {
    vendorCommands << QPair<quint16, quint16>{vendorId, commandId};
    return new GAIAPropertyBase();
}

QList<QPair<quint16, quint16>> MockGAIAPropertyManager::getVendorCommands() const {
    return vendorCommands;
}

MockBluetoothSocket::MockBluetoothSocket(QObject* parent)
    : QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, parent)
{
//    setOpenMode(QIODevice::ReadWrite | QIODevice::Unbuffered);
}

void MockBluetoothSocket::close() {
//        QBluetoothSocket::close();
    setOpenMode(QIODevice::NotOpen);
    Q_EMIT disconnected();
}

void MockBluetoothSocket::setTestData(const QByteArray &data){
    m_data = data;
}

QByteArray MockBluetoothSocket::getTestData() const {
    return m_data;
}

qint64 MockBluetoothSocket::bytesAvailable() const {
    return m_data.size();
}

qint64 MockBluetoothSocket::readData(char *data, qint64 maxSize) {

    qDebug() << "readdata" << data << maxSize;

    if (maxSize == 0 || m_data.isEmpty()){
        return 0;
    }
    memcpy(data, m_data.data(), m_data.size());
    const auto sz = m_data.length();
    m_data.clear();

    return sz;
}

qint64 MockBluetoothSocket::writeData(const char *data, qint64 maxSize){
    m_data.clear();
    m_data.append(data, maxSize);
    return maxSize;
}

MockBluetoothSocketWrapper::MockBluetoothSocketWrapper(MockBluetoothSocket *socket, QObject *parent)
        : BluetoothSocketWrapper(socket)

{
    connect(m_socket, &QBluetoothSocket::connected, this, [this](){Q_EMIT connectedReal();});
    connect(m_socket, &QBluetoothSocket::readyRead, this, [this](){Q_EMIT readyReadReal();});
    connect(m_socket, &QBluetoothSocket::disconnected, this, [this](){Q_EMIT disconnectedReal();});
}

void MockBluetoothSocketWrapper::setIsDemo(const bool val){
    isDemo = val;
}