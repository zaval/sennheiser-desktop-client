//
// Created by Dmytrii Zavalnyi on 30.10.2024..
//

#ifndef SENNHEISERDESKTOPCLIENT_MOCK_CLASSES_H
#define SENNHEISERDESKTOPCLIENT_MOCK_CLASSES_H


#include <QBluetoothSocket>
#include "GAIAPropertyManagerBase.h"
#include "BluetoothSocketWrapper.h"

class MockGAIAPropertyManager : public GAIAPropertyManagerBase {
    Q_OBJECT

public:
    GAIAPropertyBase *getPropertyFromVendorCommand(const quint16 &vendorId, const quint16 &commandId) override;

    QList<QPair<quint16, quint16>> getVendorCommands() const;

private:

    QList<QPair<quint16, quint16>> vendorCommands{};
};

class MockBluetoothSocket : public QBluetoothSocket {
    Q_OBJECT

public:
    MockBluetoothSocket(QObject* parent = nullptr);

    void close() override;

    void setTestData(const QByteArray &data);

    QByteArray getTestData() const;

    qint64 bytesAvailable() const override;

protected:
    qint64 readData(char *data, qint64 maxSize) override;

    qint64 writeData(const char *data, qint64 maxSize) override;
private:
    QByteArray m_data;

};

class MockBluetoothSocketWrapper: public BluetoothSocketWrapper {
    Q_OBJECT
public:
    explicit MockBluetoothSocketWrapper(MockBluetoothSocket *socket, QObject *parent = nullptr);

    void setIsDemo(const bool val);

    signals:
    void connectedReal();
    void disconnectedReal();
    void readyReadReal();
};



#endif //SENNHEISERDESKTOPCLIENT_MOCK_CLASSES_H
