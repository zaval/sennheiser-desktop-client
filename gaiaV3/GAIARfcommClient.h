#ifndef QBT_GAIARFCOMMCLIENT_H
#define QBT_GAIARFCOMMCLIENT_H


#include <QObject>
#include <QBluetoothAddress>
#include <QtBluetooth/QBluetoothSocket>
#include <QtQmlIntegration>
#include <QJSEngine>
#include <QQmlApplicationEngine>
#include "GAIAPropertyBase.h"
#include "GAIAPropertyManager.h"
#include "BluetoothSocketWrapper.h"

class GAIARfcommClient: public QObject {

    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool isConnected READ getIsConnected NOTIFY isConnectedChanged)
    Q_PROPERTY(GAIAPropertyManagerBase* manager READ getManager NOTIFY managerChanged)
    Q_PROPERTY(QString deviceName READ getDeviceName WRITE setDeviceName NOTIFY deviceNameChanged)
    Q_PROPERTY(QString deviceAddress READ getDeviceAddress  NOTIFY deviceAddressChanged)


public:
    static GAIARfcommClient &getInstance() {
        static GAIARfcommClient instance;
        return instance;
    }

    static GAIARfcommClient *create(QQmlEngine *, QJSEngine *){

        auto instance = &GAIARfcommClient::getInstance();
        QJSEngine::setObjectOwnership(instance, QJSEngine::CppOwnership);
        return instance;
    }

//#ifdef Q_OS_APPLE
//    void setAddress(const QBluetoothUuid &deviceAddress);
//    void setServiceInfo(const QBluetoothServiceInfo &si);
//#else
//    void setAddress(const QBluetoothAddress &deviceAddress);
//#endif

    void setAddress(const QBluetoothAddress &deviceAddress);


protected:
    GAIARfcommClient();  // private constructor
    GAIARfcommClient(QBluetoothSocket *socket);  // private constructor
    ~GAIARfcommClient() = default;
//    QBluetoothSocket *socket;
    BluetoothSocketWrapper *socket;
    QList<QByteArray> packetSplitter(const QByteArray &data) const;
    bool m_isConnected;
    bool isSecondaryConnection;
    GAIAPropertyManagerBase *propertyManager;

private:
    GAIARfcommClient(const GAIARfcommClient &) = delete;  // delete copy constructor
    GAIARfcommClient &operator=(const GAIARfcommClient &) = delete;  // delete copy assignment operator
//#ifdef Q_OS_APPLE
//    QBluetoothUuid m_deviceAddress;
//    QBluetoothServiceInfo serviceInfo;
//#else
//    QBluetoothAddress m_deviceAddress;
//#endif

    QBluetoothAddress m_deviceAddress;
    QString deviceName;

protected slots:
    void socketConnected();
    void socketDisconnected();
    void socketReadyRead();

private slots:

    void propertyManagerAdded(const QByteArray &vendorCommand);

public slots:
    bool getIsConnected() const;
    void sendData(const QByteArray &data);

    GAIAPropertyManagerBase* getManager();
    QString getDeviceName() const;
    QString getDeviceAddress() const;

    void setDeviceName(const QString &name);

signals:
    void isConnectedChanged();
    void managerChanged();
    void deviceNameChanged();
    void deviceAddressChanged();
};


#endif //QBT_GAIARFCOMMCLIENT_H
