//
// Created by Dmytrii Zavalnyi on 29.10.2024..
//

#ifndef SENNHEISERDESKTOPCLIENT_CALLSETTINGSCARDHELPER_H
#define SENNHEISERDESKTOPCLIENT_CALLSETTINGSCARDHELPER_H


#include <QObject>
#include <QtQmlIntegration>
#include "gaiaV3/GAIAPropertyBase.h"

class CallSettingsCardHelper: public QObject{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool autoCallEnabled READ getAutoCallEnabled WRITE setAutoCallEnabled NOTIFY autoCallEnabledChanged)
    Q_PROPERTY(bool comfortCallEnabled READ getComfortCallEnabled WRITE setComfortCallEnabled NOTIFY comfortCallEnabledChanged)

public:
    explicit CallSettingsCardHelper(QObject *parent = nullptr);

public slots:
    bool getAutoCallEnabled() const;
    bool getComfortCallEnabled() const;
    void setAutoCallEnabled(const bool &enabled);
    void setComfortCallEnabled(const bool &enabled);

private slots:

    void setAutoCallValue();
    void setComfortCallValue();

private:
    bool m_autoCallEnabled;
    bool m_comfortCallEnabled;

    GAIAPropertyBase *setting_AutoCall;
    GAIAPropertyBase *setting_ComfortCall;

signals:
    void autoCallEnabledChanged();
    void comfortCallEnabledChanged();

};


#endif //SENNHEISERDESKTOPCLIENT_CALLSETTINGSCARDHELPER_H
