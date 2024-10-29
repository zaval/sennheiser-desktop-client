//
// Created by Dmytrii Zavalnyi on 21.10.2024..
//

#ifndef SENNHEISERDESKTOPCLIENT_CONNECTEDDEVICESCARDHELPER_H
#define SENNHEISERDESKTOPCLIENT_CONNECTEDDEVICESCARDHELPER_H


#include <QObject>
#include <QtQmlIntegration>
#include "gaiaV3/GAIAPropertyBase.h"

struct PairedDeviceInfo {
    QString name;
    bool connected;
    int index;
};

class PairedDevicesModel: public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

public:

    enum DeviceRoles {
        NameRole = Qt::UserRole + 1,
        ConnectedRole,
        DeviceIndexRole
    };

    explicit PairedDevicesModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

    int rowCount(const QModelIndex &parent) const override;

    QHash<int, QByteArray> roleNames() const override;

    void addPairedDevices(const PairedDeviceInfo &device);
    void clear();

private:
    QList<PairedDeviceInfo> m_pairedDevices;

};

class ConnectedDevicesCardHelper: public QObject{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int totalPaired READ getTotalPaired NOTIFY totalPairedChanged)
    Q_PROPERTY(int maxConnections READ getMaxConnections NOTIFY maxConnectionsChanged)
    Q_PROPERTY(int ownIndex READ getOwnIndex NOTIFY ownIndexChanged)
    Q_PROPERTY(PairedDevicesModel* model READ getModel NOTIFY modelChanged)


public:
    explicit ConnectedDevicesCardHelper(QObject *parent = nullptr);

private:
    GAIAPropertyBase *pairedDevicesListSize;
    GAIAPropertyBase *pairedDevicesMaxBTConnections;
    GAIAPropertyBase *pairedDevicesOwnDeviceIndex;
    GAIAPropertyBase *pairedDevicesGetDeviceInfo;
    GAIAPropertyBase *pairedDevicesConnectDevice;
    GAIAPropertyBase *pairedDevicesDeleteEntry;
    GAIAPropertyBase *pairedDevicesDisconnectDevice;

    int m_totalPaired;
    int m_maxConnections;
    int m_ownIndex;

    PairedDevicesModel *m_model;

public slots:
    int getTotalPaired() const;
    int getMaxConnections() const;
    int getOwnIndex() const;
    PairedDevicesModel *getModel();

    void connectDevice(const int &index);
    void disconnectDevice(const int &index);
    void deleteDevice(const int &index);

signals:
    void totalPairedChanged();
    void maxConnectionsChanged();
    void ownIndexChanged();
    void modelChanged();

private slots:
    void changePairedDevicesListSizeValue();
    void changePairedDevicesMaxBTConnectionsValue();
    void changePairedDevicesOwnDeviceIndexValue();
    void changePairedDevicesGetDeviceInfoResult();
    void deviceConnectionUpdated();
};


#endif //SENNHEISERDESKTOPCLIENT_CONNECTEDDEVICESCARDHELPER_H
