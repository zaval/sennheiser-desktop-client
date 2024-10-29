#include <QtTest/QtTest>
#include "GAIAPropertyBase.h"

using namespace Qt::Literals::StringLiterals;

// Mock class inheriting from GAIAPropertyBase for testing protected methods and data

class GAIAStringTest: public GAIAPropertyBase {
    Q_OBJECT

public:
    explicit GAIAStringTest(QObject *parent = nullptr){
        getVendorId = 0x001D;
        getCommandId = 0x0003;
        getResponseVendorId = 0x001D;
        getResponseCommandId = 0x0103;

        setVendorId = 0x001A;
        setCommandId = 0x0013;
        setResponseVendorId = 0x001A;
        setResponseCommandId = 0x0103;
        valueTypes = {ParameterType::STRING};

        invocationVendorId = 0x1111;
        invocationCommandId = 0x2222;
        invocationResponseVendorId = 0x1122;
        invocationResponseCommandId = 0x2211;

        resultTypes = {ParameterType::STRING};
    }

public:
    void setValueType(const ParameterType &type){
        valueTypes = {type};
    }

    void setResultType(const ParameterType &type){
        resultTypes = {type};
    }
    void setParameterTypes(const QList<ParameterType> &type){
        parameterTypes = type;
    }

    void setSetTypes(const QList<ParameterType> &type){
        setTypes = type;
    }
};

class GAIAPropertyBaseTest : public QObject {
Q_OBJECT

private slots:
    void testCommand_data();
    void testCommand();
    void testParse_data();
    void testParse();
    void testSend_data();
    void testSend();

private:
    GAIAStringTest gaia;
    void prepareCommandData();
};

void GAIAPropertyBaseTest::prepareCommandData() {
    QTest::addColumn<GAIAPropertyBase::CommandType>("type");
    QTest::addColumn<QVariantList>("params");
    QTest::addColumn<QList<GAIAPropertyBase::ParameterType>>("typesToCheck");
    QTest::addColumn<QByteArray>("expectedResult");

    QTest::newRow("Command GET, no params") << GAIAPropertyBase::CommandType::GET << QVariantList{} << QList<GAIAPropertyBase::ParameterType>{} << "\xff\x03\x00\x00\x00\x1d\x00\x03"_ba;
    QTest::newRow("Command SET with UINT8 param") << GAIAPropertyBase::CommandType::SET << (QVariantList{QChar(0x42)}) << QList<GAIAPropertyBase::ParameterType>{} << "\xff\x03\x00\x01\x00\x1a\x00\x13\x42"_ba;
    QTest::newRow("Command SET with UINT16 param") << GAIAPropertyBase::CommandType::SET << (QVariantList{QVariant::fromValue<uint16_t>(0x1242)}) << QList<GAIAPropertyBase::ParameterType>{} << "\xff\x03\x00\x01\x00\x1a\x00\x13\x12\x42"_ba;
    QTest::newRow("Command SET with UINT32 param") << GAIAPropertyBase::CommandType::SET << (QVariantList{0x12345678}) << QList<GAIAPropertyBase::ParameterType>{} << "\xff\x03\x00\x01\x00\x1a\x00\x13\x12\x34\x56\x78"_ba;
    QTest::newRow("Command INVOCATION with UINT8 param") << GAIAPropertyBase::CommandType::INVOCATION << (QVariantList{QChar(0x42)}) << QList<GAIAPropertyBase::ParameterType>{} << "\xff\x03\x00\x01\x11\x11\x22\x22\x42"_ba;
    QTest::newRow("Command INVOCATION with UINT16 param") << GAIAPropertyBase::CommandType::INVOCATION << (QVariantList{QVariant::fromValue<uint16_t>(0x1242)}) << QList<GAIAPropertyBase::ParameterType>{} << "\xff\x03\x00\x01\x11\x11\x22\x22\x12\x42"_ba;
    QTest::newRow("Command INVOCATION with UINT32 param") << GAIAPropertyBase::CommandType::INVOCATION << (QVariantList{0x12345678}) << QList<GAIAPropertyBase::ParameterType>{} << "\xff\x03\x00\x01\x11\x11\x22\x22\x12\x34\x56\x78"_ba;

    QTest::newRow("Command SET with UINT8 predefined param") << GAIAPropertyBase::CommandType::SET << (QVariantList{QChar(0x42)}) << QList<GAIAPropertyBase::ParameterType>{GAIAPropertyBase::ParameterType::UINT8} << "\xff\x03\x00\x01\x00\x1a\x00\x13\x42"_ba;
    QTest::newRow("Command SET with UINT16 predefined param") << GAIAPropertyBase::CommandType::SET << (QVariantList{QVariant::fromValue<uint16_t>(0x1242)}) << QList<GAIAPropertyBase::ParameterType>{GAIAPropertyBase::ParameterType::UINT16} << "\xff\x03\x00\x01\x00\x1a\x00\x13\x12\x42"_ba;
    QTest::newRow("Command SET with UINT32 predefined param") << GAIAPropertyBase::CommandType::SET << (QVariantList{0x12345678}) << QList<GAIAPropertyBase::ParameterType>{GAIAPropertyBase::ParameterType::UINT32} << "\xff\x03\x00\x01\x00\x1a\x00\x13\x12\x34\x56\x78"_ba;
    QTest::newRow("Command INVOCATION with predefined UINT8 param") << GAIAPropertyBase::CommandType::INVOCATION << (QVariantList{QChar(0x42)}) << QList<GAIAPropertyBase::ParameterType>{GAIAPropertyBase::ParameterType::UINT8} << "\xff\x03\x00\x01\x11\x11\x22\x22\x42"_ba;
    QTest::newRow("Command INVOCATION with predefined UINT16 param") << GAIAPropertyBase::CommandType::INVOCATION << (QVariantList{QVariant::fromValue<uint16_t>(0x1242)}) << QList<GAIAPropertyBase::ParameterType>{GAIAPropertyBase::ParameterType::UINT16} << "\xff\x03\x00\x01\x11\x11\x22\x22\x12\x42"_ba;
    QTest::newRow("Command INVOCATION with predefined UINT32 param") << GAIAPropertyBase::CommandType::INVOCATION << (QVariantList{0x12345678}) << QList<GAIAPropertyBase::ParameterType>{GAIAPropertyBase::ParameterType::UINT32} << "\xff\x03\x00\x01\x11\x11\x22\x22\x12\x34\x56\x78"_ba;
}

void GAIAPropertyBaseTest::testCommand_data() {
    prepareCommandData();
}

void GAIAPropertyBaseTest::testCommand() {
    QFETCH(GAIAPropertyBase::CommandType, type);
    QFETCH(QVariantList, params);
    QFETCH(QList<GAIAPropertyBase::ParameterType>, typesToCheck);
    QFETCH(QByteArray, expectedResult);

    auto result = QByteArray{};

    switch (type) {
        case GAIAPropertyBase::CommandType::GET:
            result = gaia.commandGet();
            break;
        case GAIAPropertyBase::CommandType::SET:
            gaia.setSetTypes(typesToCheck);
            result = gaia.commandSet(params);
            break;
        case GAIAPropertyBase::CommandType::INVOCATION:
            gaia.setParameterTypes(typesToCheck);
            result = gaia.commandInvocation(params);
            break;
    }
    QCOMPARE(result, expectedResult);
}

void GAIAPropertyBaseTest::testParse_data(){
    QTest::addColumn<GAIAPropertyBase::ParameterType>("valueType");
    QTest::addColumn<GAIAPropertyBase::ParameterType>("resultType");
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<QVariantList>("expectedResult");

    QTest::newRow("Value of UINT8")
        << GAIAPropertyBase::ParameterType::UINT8
        << GAIAPropertyBase::ParameterType::NONE
        << "\xff\x03\x00\x01\x00\x1d\x01\x03\x12"_ba
        << QVariantList{0x12};
    QTest::newRow("Value of UINT16")
        << GAIAPropertyBase::ParameterType::UINT16
        << GAIAPropertyBase::ParameterType::NONE
        << "\xff\x03\x00\x02\x00\x1d\x01\x03\x12\x42"_ba
        << QVariantList{QVariant::fromValue<uint16_t>(0x1242)};
    QTest::newRow("Value of UINT32")
        << GAIAPropertyBase::ParameterType::UINT32
        << GAIAPropertyBase::ParameterType::NONE
        << "\xff\x03\x00\x04\x00\x1d\x01\x03\x12\x34\x56\x78"_ba
        << QVariantList{0x12345678};
    QTest::newRow("Result of UINT8")
        << GAIAPropertyBase::ParameterType::NONE
        << GAIAPropertyBase::ParameterType::UINT8
        << "\xff\x03\x00\x01\x11\x22\x22\x11\x12"_ba
        << QVariantList{0x12};
    QTest::newRow("Result of UINT16")
        << GAIAPropertyBase::ParameterType::NONE
        << GAIAPropertyBase::ParameterType::UINT16
        << "\xff\x03\x00\x02\x11\x22\x22\x11\x12\x42"_ba
        << QVariantList{QVariant::fromValue<uint16_t>(0x1242)};
    QTest::newRow("Result of UINT32")
        << GAIAPropertyBase::ParameterType::NONE
        << GAIAPropertyBase::ParameterType::UINT32
        << "\xff\x03\x00\x04\x11\x22\x22\x11\x12\x34\x56\x78"_ba
        << QVariantList{0x12345678};

}

void GAIAPropertyBaseTest::testParse(){
    QFETCH(GAIAPropertyBase::ParameterType, valueType);
    QFETCH(GAIAPropertyBase::ParameterType, resultType);
    QFETCH(QByteArray, data);
    QFETCH(QVariantList, expectedResult);

    gaia.setValueType(valueType);
    gaia.setResultType(resultType);
    gaia.parse(data);

    if (valueType != GAIAPropertyBase::ParameterType::NONE){
        QCOMPARE(gaia.getValue(), expectedResult);
    }
    if (resultType != GAIAPropertyBase::ParameterType::NONE){
        QCOMPARE(gaia.getResult(), expectedResult);
    }
}

void GAIAPropertyBaseTest::testSend_data() {
    prepareCommandData();
}

void GAIAPropertyBaseTest::testSend() {
    QFETCH(GAIAPropertyBase::CommandType, type);
    QFETCH(QVariantList, params);
    QFETCH(QList<GAIAPropertyBase::ParameterType>, typesToCheck);
    QFETCH(QByteArray, expectedResult);

    QSignalSpy spy(&gaia, SIGNAL(sendData(const QByteArray&)));
    switch (type) {
        case GAIAPropertyBase::CommandType::GET:
            gaia.sendGet();
            break;
        case GAIAPropertyBase::CommandType::SET:
            gaia.setSetTypes(typesToCheck);
            gaia.sendSet(params);
            break;
        case GAIAPropertyBase::CommandType::INVOCATION:
            gaia.setParameterTypes(typesToCheck);
            gaia.sendInvocation(params);
            break;
    }

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();

    QVERIFY(arguments.at(0).toByteArray() == expectedResult);
}


QTEST_MAIN(GAIAPropertyBaseTest)
#include "test_GAIAPropertyBase.moc"