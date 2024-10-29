#ifndef SENNHEISERDESKTOPCLIENT_BLUETOOTHHANDLER_H
#define SENNHEISERDESKTOPCLIENT_BLUETOOTHHANDLER_H


#include <QObject>
#include <QtQmlIntegration>
#include <QtBluetooth/QBluetoothDeviceInfo>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothServiceInfo>
#include <QBluetoothServiceDiscoveryAgent>

class BluetoothHandler: public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool isSearching READ getIsSearching NOTIFY isSearchingChanged)
    Q_PROPERTY(QString deviceAddress READ getDeviceAddress WRITE setDeviceAddress NOTIFY deviceAddressChanged)

public:
    explicit BluetoothHandler(QObject *parent = nullptr);

public slots:
    void startDeviceDiscovery();
    bool getIsSearching() const;
    QString getDeviceAddress() const;
    void setDeviceAddress(const QString &address);


private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void discoveryFinished();

//#ifdef Q_OS_APPLE
//    void serviceDiscovered(const QBluetoothServiceInfo &service);
//#endif


private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
//#ifdef Q_OS_APPLE
//    QBluetoothUuid selectedDeviceAddress;
//    QMap<QString, QBluetoothServiceInfo> serviceInfos;
//    void discoverServices(const QBluetoothDeviceInfo &device);
//    QBluetoothServiceDiscoveryAgent *serviceDiscoveryAgent;
//#else
//    QBluetoothAddress selectedDeviceAddress;
//#endif

    QBluetoothAddress selectedDeviceAddress;

    QBluetoothUuid serviceUuid;
    bool m_isSearching;

signals:
    void isSearchingChanged();
    void deviceAddressChanged();
    void newDeviceAdded(const QString &name, const QString &address, const QString &uuid);
};


#endif //SENNHEISERDESKTOPCLIENT_BLUETOOTHHANDLER_H
