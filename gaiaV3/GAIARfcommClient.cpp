#include "GAIARfcommClient.h"
#include "GAIAPropertyClasses.h"
#include <QBluetoothDeviceInfo>
//#ifdef Q_OS_APPLE
//void GAIARfcommClient::setAddress(const QBluetoothUuid &deviceAddress) {
//#else
//void GAIARfcommClient::setAddress(const QBluetoothAddress &deviceAddress) {
//#endif

void GAIARfcommClient::setAddress(const QBluetoothAddress &deviceAddress) {
    m_deviceAddress = deviceAddress;
    Q_EMIT deviceAddressChanged();
    if (socket->isOpen()){
        if (!m_deviceAddress.isNull())
            isSecondaryConnection = true;
        socket->close();
        return;
    }
//#ifdef Q_OS_APPLE
////    QBluetoothDeviceInfo deviceInfo;
////    deviceInfo.setDeviceUuid(m_deviceAddress);
////    QBluetoothServiceInfo serviceInfo;
////
////    QBluetoothUuid sppUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
////    serviceInfo.setServiceUuid(sppUuid);
////
////
////    serviceInfo.setServiceUuid(QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));
////
////    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
////                             QVariant::fromValue(QVariantList() << QVariant::fromValue(QBluetoothUuid::ProtocolUuid::Rfcomm)));
////
////    serviceInfo.setDevice(deviceInfo);
//
////    socket->connectToService(serviceInfo);
//#else
//    socket->connectToService(m_deviceAddress, QBluetoothServiceInfo::RfcommProtocol);
//#endif

    socket->connectToService(m_deviceAddress, QBluetoothServiceInfo::RfcommProtocol);


}

GAIARfcommClient::GAIARfcommClient():
    QObject(nullptr),
//    socket(new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this)),
    socket(new BluetoothSocketWrapper(this)),
    m_isConnected(false),
    isSecondaryConnection(false),
    m_deviceAddress{},
    propertyManager(new GAIAPropertyManager(this)),
    deviceName("")
{
//    propertyManager = new GAIAPropertyManager(this);
    connect(socket, &BluetoothSocketWrapper::connected, this, &GAIARfcommClient::socketConnected);
    connect(socket, &BluetoothSocketWrapper::readyRead, this, &GAIARfcommClient::socketReadyRead);
    connect(socket, &BluetoothSocketWrapper::disconnected, this, &GAIARfcommClient::socketDisconnected);
    connect(propertyManager, &GAIAPropertyManagerBase::propertyAdded, this, &GAIARfcommClient::propertyManagerAdded);
}

void GAIARfcommClient::socketConnected() {
//    qDebug() << "GAIARfcommClient connected";
    m_isConnected = true;
    Q_EMIT isConnectedChanged();
}

void GAIARfcommClient::socketDisconnected() {
//    qDebug() << "GAIARfcommClient disconnected";
    m_isConnected = false;
    Q_EMIT isConnectedChanged();
    if (isSecondaryConnection){
//#ifdef Q_OS_APPLE
////        QBluetoothDeviceInfo deviceInfo;
////        deviceInfo.setDeviceUuid(m_deviceAddress);
////        QBluetoothServiceInfo serviceInfo;
////        serviceInfo.setDevice(deviceInfo);
//        socket->connectToService(serviceInfo);
//#else
//        socket->connectToService(m_deviceAddress, QBluetoothServiceInfo::RfcommProtocol);
//#endif
        socket->connectToService(m_deviceAddress, QBluetoothServiceInfo::RfcommProtocol);
        isSecondaryConnection = false;
    }
}

QList<QByteArray> GAIARfcommClient::packetSplitter(const QByteArray &data) const {
    QList<QByteArray> packets = {};
    auto tmpData = data;
    while (true){
        if (tmpData.isEmpty() || tmpData.length() < 6) {
            break;
        }
        const auto dataSize = ((tmpData.at(2) << 8) & 0xff00) + (tmpData.at(3) & 0x00ff);
        packets << tmpData.mid(0, dataSize + 8);
        tmpData = tmpData.mid(dataSize + 8);
    }
    return packets;
}

void GAIARfcommClient::socketReadyRead() {
    const auto &dataAll = socket->readAll();
    qDebug() << "[SOCKET] << " << dataAll;
    if (dataAll.length() < 6){
        return;
    }

    const auto packets = packetSplitter(dataAll);

    std::for_each(packets.begin(), packets.end(), [this](const QByteArray &data){
        const auto vendorCommand = data.mid(4, 4);
        auto property = propertyManager->getProperty(vendorCommand);

//        qDebug() << property;
        if (property != nullptr){
            property->parse(data);
        }
    });

}

bool GAIARfcommClient::getIsConnected() const {
    return m_isConnected;
}

void GAIARfcommClient::sendData(const QByteArray &data) {
    qDebug() << "[SOCKET] >>" << data;
    if (!socket->isOpen()){
        return;
    }
    socket->write(data);
}

GAIAPropertyManagerBase *GAIARfcommClient::getManager() {
    return propertyManager;
}

void GAIARfcommClient::propertyManagerAdded(const QByteArray &vendorCommand) {
    auto property = propertyManager->getProperty(vendorCommand, false);
    connect(property, &GAIAPropertyBase::sendData, this, &GAIARfcommClient::sendData);
}

QString GAIARfcommClient::getDeviceName() const {
    return deviceName;
}

void GAIARfcommClient::setDeviceName(const QString &name) {
    deviceName = name;
    Q_EMIT deviceNameChanged();
}

QString GAIARfcommClient::getDeviceAddress() const {
    return m_deviceAddress.toString();
}

GAIARfcommClient::GAIARfcommClient(QBluetoothSocket *socket) : QObject(nullptr),
//   socket(socket),
   socket(new BluetoothSocketWrapper(nullptr)),
   m_isConnected(false),
   isSecondaryConnection(false),
   m_deviceAddress{},
   propertyManager(new GAIAPropertyManager(this)),
   deviceName("")
{
//    connect(propertyManager, &GAIAPropertyManagerBase::propertyAdded, this, &GAIARfcommClient::propertyManagerAdded);
}

//#ifdef Q_OS_APPLE
//void GAIARfcommClient::setServiceInfo(const QBluetoothServiceInfo &si) {
//    serviceInfo = si;
//    socket->connectToService(serviceInfo);
//}
//#endif

