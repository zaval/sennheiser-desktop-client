#ifndef QBT_GAIAPROPERTYBASE_H
#define QBT_GAIAPROPERTYBASE_H


#include <QObject>
#include <QVariantList>

class GAIAPropertyBase: public QObject {


    Q_OBJECT

    Q_PROPERTY(QVariantList value READ getValue NOTIFY valueChanged)
    Q_PROPERTY(QVariantList result READ getResult NOTIFY resultChanged)

public:

    explicit GAIAPropertyBase(QObject *parent = nullptr);

    enum CommandType {
        GET,
        SET,
        INVOCATION
    };

    enum ParameterType {
        UINT8,
        UINT16,
        UINT32,
        STRING,
        BOOL,
        NONE
    };

//    template <typename T, typename... Types>
//    QByteArray unifiedCommandSet(T var1, Types... var2) const;


private:
    [[nodiscard]] QByteArray prepareCommand(CommandType type) const;
    [[nodiscard]] QByteArray prepareCommand(CommandType type, const QVariantList &parameters) const;
    static const QMap<ParameterType, int> typeSizes;

protected:
    uint16_t getVendorId{};
    uint16_t getCommandId{};
    uint16_t getResponseVendorId{};
    uint16_t getResponseCommandId{};

    uint16_t setVendorId{};
    uint16_t setCommandId{};
    uint16_t setResponseVendorId{};
    uint16_t setResponseCommandId{};

    uint16_t invocationVendorId{};
    uint16_t invocationCommandId{};
    uint16_t invocationResponseVendorId{};
    uint16_t invocationResponseCommandId{};

    QList<ParameterType> valueTypes = {};
    QList<ParameterType> resultTypes = {};
    QList<ParameterType> parameterTypes = {};
    QList<ParameterType> setTypes = {};

    QVariantList value;
    QVariantList result;

public slots:
    [[nodiscard]] QByteArray commandGet() const;


    [[nodiscard]] QByteArray commandSet(const QVariantList &parameters) const;
    [[nodiscard]] QByteArray commandInvocation(const QVariantList &parameters) const;
    void parse(const QByteArray &data);

    void sendGet();
    void sendSet(const QVariantList &parameters);
    void sendInvocation(const QVariantList &parameters);

    [[nodiscard]] QVariantList getValue();
    [[nodiscard]] QVariantList getResult() const;

signals:
    void valueChanged();
    void resultChanged();
    void sendData(const QByteArray &data);
};



#endif //QBT_GAIAPROPERTYBASE_H
