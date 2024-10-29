
#include "BluetoothHandler.h"
#include "gaiaV3/GAIARfcommClient.h"

BluetoothHandler::BluetoothHandler(QObject *parent) :
    QObject(parent),
    discoveryAgent(new QBluetoothDeviceDiscoveryAgent(this)),
    serviceUuid(QStringLiteral("0000fdce-0000-1000-8000-00805f9b34fb")),
    m_isSearching(false)
//#ifdef Q_OS_APPLE
//    , serviceInfos{}
//#endif
{
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,this, &BluetoothHandler::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,this, &BluetoothHandler::discoveryFinished);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled, this, &BluetoothHandler::discoveryFinished);
}

void BluetoothHandler::startDeviceDiscovery() {
//    qDebug() << "Start discovery";
    m_isSearching = true;
    Q_EMIT isSearchingChanged();
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
//#ifdef Q_OS_APPLE
//    serviceInfos.clear();
//#endif
}

void BluetoothHandler::deviceDiscovered(const QBluetoothDeviceInfo &device) {
    if (device.serviceUuids().contains(serviceUuid)) {
        const auto uids = device.serviceUuids();
        qDebug() << "Found device with required service:" << device.name() << device.address() << device.deviceUuid();
//#ifdef Q_OS_APPLE
//        Q_EMIT newDeviceAdded(device.name(), device.deviceUuid().toString(QUuid::WithoutBraces));
//        discoverServices(device);
//#else
//        Q_EMIT newDeviceAdded(device.name(), device.address().toString());
//#endif
        Q_EMIT newDeviceAdded(device.name(), device.address().toString(), device.deviceUuid().toString(QUuid::WithoutBraces));


//        selectedDeviceAddress = device.address();
//        Q_EMIT deviceAddressChanged();
        GAIARfcommClient::getInstance().setDeviceName(device.name());
//        discoveryAgent->stop();
    }
}

void BluetoothHandler::discoveryFinished() {
    m_isSearching = false;
    Q_EMIT isSearchingChanged();

    if (!selectedDeviceAddress.isNull()) {
        qDebug() << "device found, connecting";
        GAIARfcommClient::getInstance().setAddress(selectedDeviceAddress);

    }

//#ifdef Q_OS_APPLE
//    if (serviceInfos.contains(selectedDeviceAddress.toString(QUuid::WithoutBraces))){
//        GAIARfcommClient::getInstance().setServiceInfo(serviceInfos[selectedDeviceAddress.toString(QUuid::WithoutBraces)]);
//    }
//
//#endif
}


bool BluetoothHandler::getIsSearching() const {
    return m_isSearching;
}

QString BluetoothHandler::getDeviceAddress() const {
    return selectedDeviceAddress.toString();
}

void BluetoothHandler::setDeviceAddress(const QString &address) {
//    qDebug() << "BluetoothHandler::setDeviceAddress" << address;
    if (m_isSearching){
        discoveryAgent->stop();
    }
//#ifdef Q_OS_APPLE
//    selectedDeviceAddress = QBluetoothUuid(address);
//#else
//    selectedDeviceAddress = QBluetoothAddress(address);
//#endif


    selectedDeviceAddress = QBluetoothAddress(address);
    if (!m_isSearching){
        GAIARfcommClient::getInstance().setAddress(selectedDeviceAddress);
    }
}

//#ifdef Q_OS_APPLE
//void BluetoothHandler::discoverServices(const QBluetoothDeviceInfo &device) {
//    if (serviceDiscoveryAgent != nullptr){
//        serviceDiscoveryAgent->deleteLater();
//    }
//    serviceDiscoveryAgent = new QBluetoothServiceDiscoveryAgent(device.address(), this);
//    connect(serviceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
//            this, &BluetoothHandler::serviceDiscovered);
////    serviceDiscoveryAgent->setUuidFilter(QBluetoothUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"))); // SPP UUID
//    serviceDiscoveryAgent->setUuidFilter(serviceUuid); // SPP UUID
//    serviceDiscoveryAgent->start(QBluetoothServiceDiscoveryAgent::MinimalDiscovery);
//}
//
//void BluetoothHandler::serviceDiscovered(const QBluetoothServiceInfo &service) {
//    qDebug() << "Discovered service:" << service.serviceName() << "at" << service.device().name() << ", UUID:" << service.serviceUuid().toString();
//    if (service.isValid()) {
//        serviceInfos.insert(service.serviceUuid().toString(QUuid::WithoutBraces), service);
//    }
//}
//
//#endif
