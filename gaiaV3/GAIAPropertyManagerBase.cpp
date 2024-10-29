#include "GAIAPropertyManagerBase.h"

GAIAPropertyBase *GAIAPropertyManagerBase::getProperty(const QByteArray &vendorCommand, bool notify) {
//    qDebug() << "vendorCommand" << vendorCommand;
    if (vendorCommand.length() < 4){
        return nullptr;
    }

    const auto mapKey = (static_cast<uint32_t>(vendorCommand.at(0)) << 24) |
        (static_cast<uint32_t>(vendorCommand.at(1)) << 16) |
        (static_cast<uint32_t>(vendorCommand.at(2)) << 8) |
        static_cast<uint32_t>(vendorCommand.at(3));

    if (m_properties.contains(mapKey)){
//        qDebug() << mapKey << "exists in the list";
        return m_properties[mapKey];
    }
    const auto vendorId = ((vendorCommand.at(0) << 8) & 0xff00) + (vendorCommand.at(1) & 0x00ff);
    const auto commandId = ((vendorCommand.at(2) << 8) & 0xff00) + (vendorCommand.at(3) & 0x00ff);

//    qDebug() << "vendorId" << vendorId << "commandId" << commandId;
    auto property = getPropertyFromVendorCommand(vendorId, commandId);

    if (property != nullptr){
        qDebug() << "getPropertyFromVendorCommand" << property;
        m_properties.insert(mapKey, property);
        if (notify)
            Q_EMIT propertyAdded(vendorCommand);
    }
    else {
        qDebug()
            << "getPropertyFromVendorCommand returns 0x0 for vendorId:"
            << vendorId
            << "commandId:"
            << commandId;
    }
    return property;
}
