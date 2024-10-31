#include <QtTest/QtTest>
#include "GAIARfcommClient.h"
#include "mock_classes.h"

using namespace Qt::Literals::StringLiterals;

typedef QList<QPair<quint16, quint16>> ListOfPairsOfUints;

class MockRfcommClient : public GAIARfcommClient {
    Q_OBJECT

public:
    MockRfcommClient(MockBluetoothSocket *s = nullptr)
    {
        socket = new MockBluetoothSocketWrapper(s, this);

        propertyManager = new MockGAIAPropertyManager();
        connect(socket, &MockBluetoothSocketWrapper::connected, this, &MockRfcommClient::socketConnected);
        connect(socket, &MockBluetoothSocketWrapper::readyRead, this, &MockRfcommClient::socketReadyRead);
        connect(socket, &MockBluetoothSocketWrapper::disconnected, this, &MockRfcommClient::socketDisconnected);
    }

    MockGAIAPropertyManager *getPropertyManager() const{
        return qobject_cast<MockGAIAPropertyManager*>(propertyManager);
    }

    QList<QByteArray> testPacketSplitter(const QByteArray &data) const{
        return packetSplitter(data);
    }

    void testSocketConnected() {
        socketConnected();
    }
    void testSocketDisconnected() {
        isSecondaryConnection = false;
        socketDisconnected();
    }
    void testSocketDisconnectedAfterConnected() {
        isSecondaryConnection = true;
        socketDisconnected();
    }

    bool getIsSecondaryConnection() const {
        return isSecondaryConnection;
    }

    void testSocketReadyRead(){
        socketReadyRead();
    }
};


class GAIARfcommClientTest: public QObject {
    Q_OBJECT

private slots:
    void testSetAddress();
    void testConnectionStateConnected();
    void testConnectionStateDisconnected();
    void testConnectionStateDisconnectedAfterConnected();
    void testPacketSplitter_data();
    void testPacketSplitter();
    void testSetDeviceName();
    void testReadSocket_data();
    void testReadSocket();
    void testSendData();
    void testSendDataOnClosedSocket();
    void testGetManager();
//    void testSendData();
};

void GAIARfcommClientTest::testSetAddress()
{
    auto *socket = new MockBluetoothSocket();
    socket->open(QIODevice::ReadWrite | QIODevice::Unbuffered);
    MockRfcommClient client{socket};
    QBluetoothAddress address("00:11:22:33:44:55");
    client.setAddress(address);
    QCOMPARE(client.getDeviceAddress(), QString("00:11:22:33:44:55"));
}

void GAIARfcommClientTest::testConnectionStateConnected()
{
    auto *socket = new MockBluetoothSocket();
    MockRfcommClient client{socket};
    QSignalSpy spy(&client, SIGNAL(isConnectedChanged()));
    client.testSocketConnected();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(client.getIsConnected(), true);

}

void GAIARfcommClientTest::testConnectionStateDisconnected() {
    auto *socket = new MockBluetoothSocket();
    MockRfcommClient client{socket};
    QSignalSpy spy(&client, SIGNAL(isConnectedChanged()));
    client.testSocketDisconnected();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(client.getIsConnected(), false);
}

void GAIARfcommClientTest::testConnectionStateDisconnectedAfterConnected() {
    auto *socket = new MockBluetoothSocket();
    socket->close();

    MockRfcommClient client{socket};
    QSignalSpy spy(&client, SIGNAL(isConnectedChanged()));
    QBluetoothAddress address("00:11:22:33:44:55");
    client.setAddress(address);
    client.testSocketDisconnectedAfterConnected();
    QVERIFY(spy.count() > 0);
    QCOMPARE(client.getIsConnected(), false);
    QCOMPARE(client.getIsSecondaryConnection(), false);

}

void GAIARfcommClientTest::testPacketSplitter_data() {
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<int>("expectedPacketsSize");
    QTest::addColumn<QByteArray>("expectedFirstPacket");

    QTest::newRow("Single packet")  << "\xff\x03\x00\x00\x00\x1d\x00\x03"_ba << 1 << "\xff\x03\x00\x00\x00\x1d\x00\x03"_ba;
    QTest::newRow("Two packets packet")  << "\xff\x03\x00\x00\x00\x1d\x00\x03\xff\x03\x00\x00\x00\x1d\x00\x03"_ba << 2 << "\xff\x03\x00\x00\x00\x1d\x00\x03"_ba;
    QTest::newRow("Broken packet")  << "\xff\x03\x00"_ba << 0 << ""_ba;
    QTest::newRow("Single packet and broken")  << "\xff\x03\x00\x00\x00\x1d\x00\x03\xff\x03\x00"_ba << 1 << "\xff\x03\x00\x00\x00\x1d\x00\x03"_ba;
}

void GAIARfcommClientTest::testPacketSplitter()
{

    QFETCH(QByteArray, data);
    QFETCH(int, expectedPacketsSize);
    QFETCH(QByteArray, expectedFirstPacket);

    auto *socket = new MockBluetoothSocket();
    MockRfcommClient client{socket};
    auto packets = client.testPacketSplitter(data);

    QCOMPARE(packets.count(), expectedPacketsSize);
    if (expectedPacketsSize > 0)
        QCOMPARE(packets.first(), expectedFirstPacket);
}

void GAIARfcommClientTest::testSetDeviceName()
{
    auto *socket = new MockBluetoothSocket();
    MockRfcommClient client{socket};
    QSignalSpy spy(&client, SIGNAL(deviceNameChanged()));

    client.setDeviceName("TestDevice");
    QCOMPARE(client.getDeviceName(), QString("TestDevice"));
    QCOMPARE(spy.count(), 1);
}

void GAIARfcommClientTest::testReadSocket_data() {
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<ListOfPairsOfUints>("expectedResult");

    ListOfPairsOfUints twoPacketsResult;
    twoPacketsResult << QPair<quint16, quint16>{0x0102, 0x0103} << QPair<quint16, quint16>{0x0202, 0x0203};

    QTest::newRow("Single packet")  << "\xff\x03\x00\x00\x01\x02\x01\x03"_ba << QList<QPair<quint16, quint16>>{{0x0102, 0x0103}};
    QTest::newRow("Two packets")  << "\xff\x03\x00\x00\x01\x02\x01\x03\xff\x03\x00\x00\x02\x02\x02\x03"_ba << twoPacketsResult;
    QTest::newRow("Bad packet")  << "\xff\x03"_ba << ListOfPairsOfUints{};
}

void GAIARfcommClientTest::testReadSocket() {

    QFETCH(QByteArray, data);
    QFETCH(ListOfPairsOfUints, expectedResult);

    auto *socket = new MockBluetoothSocket();
    socket->setTestData(data);
    socket->open(QIODevice::ReadWrite | QIODevice::Unbuffered);
    MockRfcommClient client{socket};
    client.testSocketReadyRead();
    const auto &actualResult = client.getPropertyManager()->getVendorCommands();
    QCOMPARE(actualResult.size(), expectedResult.size());
}

void GAIARfcommClientTest::testGetManager() {
    auto *socket = new MockBluetoothSocket();
    MockRfcommClient client{socket};
    QCOMPARE(client.getManager(), client.getPropertyManager());
}

void GAIARfcommClientTest::testSendData() {
    auto *socket = new MockBluetoothSocket();
    socket->open(QIODevice::ReadWrite | QIODevice::Unbuffered);
    MockRfcommClient client{socket};
    client.sendData("\xff\x03"_ba);
    QCOMPARE(socket->getTestData(), "\xff\x03"_ba);
}

void GAIARfcommClientTest::testSendDataOnClosedSocket() {
    auto *socket = new MockBluetoothSocket();
    socket->close();
    MockRfcommClient client{socket};
    client.sendData("\xff\x03"_ba);
    QCOMPARE(socket->getTestData(), ""_ba);
}


QTEST_MAIN(GAIARfcommClientTest)
#include "test_GAIARfcommClient.moc"
