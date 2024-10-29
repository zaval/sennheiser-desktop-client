#include "GAIAPropertyBase.h"

using namespace Qt::Literals::StringLiterals;

 const QMap<GAIAPropertyBase::ParameterType, int> GAIAPropertyBase::typeSizes = {
        {ParameterType::NONE, 0},
        {ParameterType::UINT32, 4},
        {ParameterType::UINT16, 2},
        {ParameterType::UINT8, 1},
        {ParameterType::STRING, -1},
        {ParameterType::BOOL, 1},
};

GAIAPropertyBase::GAIAPropertyBase(QObject *parent) : QObject(parent) {}

QByteArray GAIAPropertyBase::prepareCommand(CommandType type, const QVariantList &parameters) const {
    auto res = "\xff\x03"_ba; // header
    const auto paramSize = static_cast<uint16_t>(parameters.length());

    // length
    res.append(static_cast<char>((paramSize & 0xff00) >> 8));
    res.append(static_cast<char>(paramSize & 0x00ff));

    QList<ParameterType> typesToCheck = {};

    switch (type) {
        case CommandType::GET:
            res.append(static_cast<char>((getVendorId & 0xff00) >> 8));
            res.append(static_cast<char>(getVendorId & 0x00ff));

            res.append(static_cast<char>((getCommandId & 0xff00) >> 8));
            res.append(static_cast<char>(getCommandId & 0x00ff));
            break;
        case CommandType::SET:
            res.append(static_cast<char>((setVendorId & 0xff00) >> 8));
            res.append(static_cast<char>(setVendorId & 0x00ff));

            res.append(static_cast<char>((setCommandId & 0xff00) >> 8));
            res.append(static_cast<char>(setCommandId & 0x00ff));
            typesToCheck = setTypes;
            break;
        case CommandType::INVOCATION:
            res.append(static_cast<char>((invocationVendorId & 0xff00) >> 8));
            res.append(static_cast<char>(invocationVendorId & 0x00ff));

            res.append(static_cast<char>((invocationCommandId & 0xff00) >> 8));
            res.append(static_cast<char>(invocationCommandId & 0x00ff));
            typesToCheck = parameterTypes;
            break;
    }
    
    for (auto i = 0; i < parameters.length(); i++) {
        const auto &param = parameters[i];

        auto value16 = 0;

        if (typesToCheck.length() > i){
            switch (typesToCheck[i]) {
                case ParameterType::UINT8:
                    res.append(static_cast<char >(param.toChar().toLatin1()));
                    break;
                case ParameterType::UINT16:
                    value16 = static_cast<uint16_t>(param.toUInt());
                    res.append(static_cast<char>((value16 >> 8) & 0xff));
                    res.append(static_cast<char>(value16 & 0xff));
                    break;

                case ParameterType::UINT32:
                    res.append(static_cast<char>((param.toUInt() >> 24) & 0xff));
                    res.append(static_cast<char>((param.toUInt() >> 16) & 0xff));
                    res.append(static_cast<char>((param.toUInt() >> 8) & 0xff));
                    res.append(static_cast<char>(param.toUInt() & 0xff));
                    break;
                case ParameterType::BOOL:
                        param.toBool() ? res.append("\x01"_ba) : res.append("\x00"_ba);
                    break;
                case ParameterType::STRING:
                    res.append(param.toString().toLocal8Bit());
                    break;
                default:
                    break;
            }
            continue;
        }

        switch (param.typeId()) {
            case QMetaType::Type::Char:
            case QMetaType::Type::UChar:
            case QMetaType::Type::QChar:
                res.append(static_cast<char >(param.toChar().toLatin1()));
                break;
            case QMetaType::Type::Short:
            case QMetaType::Type::UShort:
            case QMetaType::Type::Char16:

                value16 = static_cast<uint16_t>(param.toUInt());
                res.append(static_cast<char>((value16 >> 8) & 0xff));
                res.append(static_cast<char>(value16 & 0xff));
                break;
            case QMetaType::Type::Int:
            case QMetaType::Type::UInt:
            case QMetaType::Type::Char32:

                res.append(static_cast<char>((param.toUInt() >> 24) & 0xff));
                res.append(static_cast<char>((param.toUInt() >> 16) & 0xff));
                res.append(static_cast<char>((param.toUInt() >> 8) & 0xff));
                res.append(static_cast<char>(param.toUInt() & 0xff));

                break;
            case QMetaType::Type::QString:
                res.append(param.toString().toLocal8Bit());
                break;
            case QMetaType::Type::QByteArray:
                res.append(param.toByteArray());
                break;
            default:
                break;
        }
    }

    return res;
}

QByteArray GAIAPropertyBase::prepareCommand(CommandType type) const {
    return prepareCommand(type, QVariantList{});
}

QByteArray GAIAPropertyBase::commandGet() const {
    return prepareCommand(CommandType::GET);
}

QByteArray GAIAPropertyBase::commandSet(const QVariantList &parameters) const {
    return prepareCommand(CommandType::SET, parameters);
}

QByteArray GAIAPropertyBase::commandInvocation(const QVariantList &parameters) const {
    return prepareCommand(CommandType::INVOCATION, parameters);
}

QVariantList GAIAPropertyBase::getValue() {
    if (value.isEmpty()){
        sendGet();
    }
    return value;
}

QVariantList GAIAPropertyBase::getResult() const {
    return result;
}

void GAIAPropertyBase::parse(const QByteArray &data) {

    if (!data.startsWith("\xff\x03"_ba)){
        return;
    }

    const auto paramSizeSlice = data.mid(2, 2);
    const auto paramSize = ((paramSizeSlice.at(0) << 8) & 0xff00) + (paramSizeSlice.at(1) & 0x00ff);
    const auto vendorId = data.mid(4, 2);
    const auto commandId = data.mid(6, 2);

    const auto vendorIdInt =  ((vendorId.at(0) << 8) & 0xff00) + (vendorId.at(1) & 0x00ff);
    const auto commandIdInt =  ((commandId.at(0) << 8) & 0xff00) + (commandId.at(1) & 0x00ff);

//    qDebug()
//        << "vendorId" << vendorId
//        << "commandId" << commandId
//        << "vendorIdInt" << vendorId
//        << "commandIdInt" << commandId
//        << "paramSizeSlice" << paramSizeSlice
//        << "paramSize" << paramSize;


    auto valueParser = [](const QByteArray& valueData, ParameterType parameterType, QVariant *res){
        switch (parameterType) {
            case ParameterType::UINT8:
                *res = static_cast<quint8>(valueData.at(0));
                break;
            case ParameterType::UINT16:
                *res = static_cast<quint16>(
                        ((valueData.at(0) << 8) & 0xff00) +
                        (valueData.at(1) & 0x00ff)
                        );
                break;
            case ParameterType::UINT32:
                *res = static_cast<quint32>(
                        ((valueData.at(0) << 24) & 0xff000000) +
                        ((valueData.at(1) << 16) & 0x00ff0000) +
                        ((valueData.at(2) << 8) & 0x0000ff00) +
                        (valueData.at(3) & 0x000000ff)
                );
                break;
            case ParameterType::STRING:
                *res = QString::fromUtf8(valueData);
                break;
            case ParameterType::BOOL:
                *res = valueData.at(0) != 0x00;
                break;
            case ParameterType::NONE:
                *res = QVariant();
        }
    };

    if (
            (vendorIdInt == getResponseVendorId && commandIdInt == getResponseCommandId) ||
            (vendorIdInt == setResponseVendorId && commandIdInt == setResponseCommandId)
    ){

        value.clear();

        auto startIndex = 8;
        for (const auto &valueType: valueTypes){
            QVariant v;
            valueParser(data.mid(startIndex, typeSizes[valueType]), valueType, &v);
            value << v;
            startIndex += typeSizes[valueType];
            if (data.length() < (startIndex+1))
                break;
        }
//        qDebug() << "updated value";
//        std::for_each(value.begin(), value.end(), [](const auto v){
//            qDebug() << v;
//        });
        Q_EMIT valueChanged();
    }

    if (vendorIdInt == invocationResponseVendorId && commandIdInt == invocationResponseCommandId){
        result.clear();
        auto startIndex = 8;
        for (const auto & resultType: resultTypes){
            QVariant v;
            valueParser(data.mid(startIndex, typeSizes[resultType]), resultType, &v);
            result << v;
            startIndex += typeSizes[resultType];
            if (data.length() < (startIndex+1))
                break;
        }

        Q_EMIT resultChanged();
    }

}

void GAIAPropertyBase::sendGet() {
    const auto &data = commandGet();
    Q_EMIT sendData(data);
}

void GAIAPropertyBase::sendSet(const QVariantList &parameters) {
    const auto &data = commandSet(parameters);
    Q_EMIT sendData(data);
}

void GAIAPropertyBase::sendInvocation(const QVariantList &parameters) {
    const auto &data = commandInvocation(parameters);
    Q_EMIT sendData(data);
}

//template<typename T, typename... Types>
//QByteArray GAIAPropertyBase::unifiedCommandSet(T var1, Types... var2) const {
//    QByteArray res;
//
//    if (
//        (typeid(T) == typeid(uint8_t)) ||
//        (typeid(T) == typeid(int8_t)) ||
//        (typeid(T) == typeid(char)) ||
//        (typeid(T) == typeid(QChar))
//    ){
//        res.append(static_cast<char>(var1));
//    }
//    else if (
//        (typeid(T) == typeid(uint16_t)) ||
//        (typeid(T) == typeid(int16_t)) ||
//        (typeid(T) == typeid(short))
//    ){
//        res.append(static_cast<char>((var1 >> 8) & 0xff));
//        res.append(static_cast<char>(var1 & 0xff));
//    }
//
//    else if (
//        (typeid(T) == typeid(uint32_t)) ||
//        (typeid(T) == typeid(int32_t)) ||
//        (typeid(T) == typeid(int)) ||
//        (typeid(T) == typeid(unsigned int))
//    ){
//        res.append(static_cast<char>((var1 >> 24) & 0xff));
//        res.append(static_cast<char>((var1 >> 16) & 0xff));
//        res.append(static_cast<char>((var1 >> 8) & 0xff));
//        res.append(static_cast<char>(var1 & 0xff));
//    }
//
//    else if (
//        (typeid(T) == typeid(QString)) ||
//        (typeid(T) == typeid(QByteArray))
//    ){
//        res.append(var1);
//    }
//
//    res.append(unifiedCommandGet(var2...));
//    return res;
//}


