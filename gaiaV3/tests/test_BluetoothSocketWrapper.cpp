#include <QObject>
#include <QSignalSpy>
#include <QTest>
#include "BluetoothSocketWrapper.h"
#include "mock_classes.h"

using namespace Qt::Literals::StringLiterals;

class BluetoothSocketWrapperTests: public QObject {
    Q_OBJECT

private slots:

    void testConnectToServiceReal();
    void testConnectToServiceDemo();
    void testReadRealData();
    void testReadDemoData();
    void testReadDemoDataTwice();
    void testReadDemoEmptyData();
    void testCloseReal();
    void testCloseDemo();
    void testIsOpenReal();
    void testIsOpenDemo();
};

void BluetoothSocketWrapperTests::testConnectToServiceReal() {
    auto s = new MockBluetoothSocket(this);
    auto wrapper = new MockBluetoothSocketWrapper(s, this);
    wrapper->connectToService(QBluetoothAddress("11:22:33:44:55:55"), QBluetoothServiceInfo::RfcommProtocol);
    QCOMPARE(s->openMode(), QIODevice::ReadWrite);
}

void BluetoothSocketWrapperTests::testConnectToServiceDemo() {
    auto s = new MockBluetoothSocket(this);
    auto wrapper = new MockBluetoothSocketWrapper(s, this);
    QSignalSpy spy(wrapper, SIGNAL(connected()));
    wrapper->connectToService(QBluetoothAddress("11:11:11:11:11:11"), QBluetoothServiceInfo::RfcommProtocol);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(s->openMode(), QIODevice::NotOpen);
}

void BluetoothSocketWrapperTests::testReadRealData() {
    auto s = new MockBluetoothSocket(this);
    const auto testData = "\x00\x11\x22"_ba;
    s->setTestData(testData);
    auto wrapper = new MockBluetoothSocketWrapper(s, this);
    wrapper->connectToService(QBluetoothAddress("11:22:33:44:55:55"), QBluetoothServiceInfo::RfcommProtocol);
    const auto &data = wrapper->readAll();
    QCOMPARE(data, testData);
}

void BluetoothSocketWrapperTests::testReadDemoData() {
    auto s = new MockBluetoothSocket(this);
    auto wrapper = new MockBluetoothSocketWrapper(s, this);
    wrapper->connectToService(QBluetoothAddress("11:11:11:11:11:11"), QBluetoothServiceInfo::RfcommProtocol);
    QSignalSpy spy(wrapper, SIGNAL(readyRead()));
    wrapper->write("\xFF\x03\x00\x00\x04\x95\x06\x03"_ba);
    spy.wait(600);
    const auto &data = wrapper->readAll();
    QCOMPARE(data, "\xFF\x03\x00\x01\x04\x95\x07\x03\x2a"_ba);
}

void BluetoothSocketWrapperTests::testReadDemoDataTwice() {
    auto s = new MockBluetoothSocket(this);
    auto wrapper = new MockBluetoothSocketWrapper(s, this);
    wrapper->connectToService(QBluetoothAddress("11:11:11:11:11:11"), QBluetoothServiceInfo::RfcommProtocol);
    QSignalSpy spy(wrapper, SIGNAL(readyRead()));
    wrapper->write("\xFF\x03\x00\x00\x04\x95\x06\x03"_ba);
    wrapper->write("\xFF\x03\x00\x00\x04\x95\x06\x03"_ba);
    spy.wait(600);
    const auto &data = wrapper->readAll();
    QCOMPARE(data, "\xFF\x03\x00\x01\x04\x95\x07\x03\x2a"_ba);
}

void BluetoothSocketWrapperTests::testReadDemoEmptyData() {
    auto s = new MockBluetoothSocket(this);
    auto wrapper = new MockBluetoothSocketWrapper(s, this);
    wrapper->connectToService(QBluetoothAddress("11:11:11:11:11:11"), QBluetoothServiceInfo::RfcommProtocol);

    const auto &data = wrapper->readAll();
    QVERIFY(data.isEmpty());
}

void BluetoothSocketWrapperTests::testCloseReal() {
    auto s = new MockBluetoothSocket(this);
    auto wrapper = new MockBluetoothSocketWrapper(s, this);
    QSignalSpy spy(wrapper, SIGNAL(disconnectedReal()));
    wrapper->connectToService(QBluetoothAddress("11:22:33:44:55:55"), QBluetoothServiceInfo::RfcommProtocol);
    wrapper->close();
    QCOMPARE(spy.count(), 1);
}

void BluetoothSocketWrapperTests::testCloseDemo() {
    auto s = new MockBluetoothSocket(this);
    auto wrapper = new MockBluetoothSocketWrapper(s, this);
    QSignalSpy spy(wrapper, SIGNAL(disconnected()));
    wrapper->connectToService(QBluetoothAddress("11:11:11:11:11:11"), QBluetoothServiceInfo::RfcommProtocol);
    wrapper->close();
    QCOMPARE(spy.count(), 1);
}

void BluetoothSocketWrapperTests::testIsOpenReal() {
    auto s = new MockBluetoothSocket(this);
    auto wrapper = new MockBluetoothSocketWrapper(s, this);
    QVERIFY(!wrapper->isOpen());
}

void BluetoothSocketWrapperTests::testIsOpenDemo() {
    auto s = new MockBluetoothSocket(this);
    auto wrapper = new MockBluetoothSocketWrapper(s, this);
    wrapper->setIsDemo(true);
    QVERIFY(wrapper->isOpen());
}




QTEST_MAIN(BluetoothSocketWrapperTests)
#include "test_BluetoothSocketWrapper.moc"
