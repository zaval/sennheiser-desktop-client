#ifndef QBT_GAIAPROPERTYMANAGERBASE_H
#define QBT_GAIAPROPERTYMANAGERBASE_H


#include <QtCore/QObject>
#include "GAIAPropertyBase.h"

class GAIAPropertyManagerBase: public QObject {
    Q_OBJECT

public:
    virtual ~GAIAPropertyManagerBase() = default;

public slots:
    GAIAPropertyBase *getProperty(const QByteArray &vendorCommand, bool notify = true);
    virtual GAIAPropertyBase *getPropertyFromVendorCommand(const quint16 &vendorId, const quint16 &commandId) = 0;


protected:
    QMap<quint32, GAIAPropertyBase*> m_properties;

signals:
    void propertyAdded(const QByteArray &vendorCommand);

};


#endif //QBT_GAIAPROPERTYMANAGERBASE_H
