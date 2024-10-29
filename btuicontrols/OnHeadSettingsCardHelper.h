//
// Created by Dmytrii Zavalnyi on 29.10.2024..
//

#ifndef SENNHEISERDESKTOPCLIENT_ONHEADSETTINGSCARDHELPER_H
#define SENNHEISERDESKTOPCLIENT_ONHEADSETTINGSCARDHELPER_H


#include <QObject>
#include <QtQmlIntegration>
#include "gaiaV3/GAIAPropertyBase.h"

class OnHeadSettingsCardHelper: public QObject{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool onHeadEnabled READ getOnHeadEnabled WRITE setOnHeadEnabled NOTIFY onHeadEnabledChanged)
    Q_PROPERTY(bool smartPauseEnabled READ getSmartPauseEnabled WRITE setSmartPauseEnabled NOTIFY smartPauseEnabledChanged)

public:
    explicit OnHeadSettingsCardHelper(QObject *parent = nullptr);

public slots:
    bool getOnHeadEnabled() const;
    bool getSmartPauseEnabled() const;
    void setOnHeadEnabled(const bool &enabled);
    void setSmartPauseEnabled(const bool &enabled);

private slots:

    void setOnHeadDetectionValue();
    void setSmartPauseValue();

private:
    bool m_onHeadEnabled;
    bool m_smartPauseEnabled;

    GAIAPropertyBase *setting_OnHeadDetection;
    GAIAPropertyBase *setting_SmartPause;

signals:
    void onHeadEnabledChanged();
    void smartPauseEnabledChanged();
};


#endif //SENNHEISERDESKTOPCLIENT_ONHEADSETTINGSCARDHELPER_H
