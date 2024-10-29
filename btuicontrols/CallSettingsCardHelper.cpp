//
// Created by Dmytrii Zavalnyi on 29.10.2024..
//

#include "CallSettingsCardHelper.h"
#include "gaiaV3/GAIARfcommClient.h"

using namespace Qt::Literals::StringLiterals;

CallSettingsCardHelper::CallSettingsCardHelper(QObject *parent) :
    QObject(parent),
    m_comfortCallEnabled{false},
    m_autoCallEnabled{false},
    setting_AutoCall(GAIARfcommClient::getInstance().getManager()->getProperty("\x04\x95\x09\x0B"_ba)),
    setting_ComfortCall(GAIARfcommClient::getInstance().getManager()->getProperty("\x04\x95\x09\x15"_ba))
{
    if (setting_AutoCall != nullptr){
        connect(setting_AutoCall, &GAIAPropertyBase::valueChanged, this, &CallSettingsCardHelper::setAutoCallValue);
        setting_AutoCall->sendGet();
    }
    if (setting_ComfortCall != nullptr){
        connect(setting_ComfortCall, &GAIAPropertyBase::valueChanged, this, &CallSettingsCardHelper::setComfortCallValue);
        setting_ComfortCall->sendGet();
    }
}

bool CallSettingsCardHelper::getAutoCallEnabled() const {
    return m_autoCallEnabled;
}

bool CallSettingsCardHelper::getComfortCallEnabled() const {
    return m_comfortCallEnabled;
}

void CallSettingsCardHelper::setAutoCallValue() {
    const auto &acv = setting_AutoCall->getValue();
    if (acv.length() < 1){
        return;
    }
    m_autoCallEnabled = acv.at(0).toInt() == 0x01;
    Q_EMIT autoCallEnabledChanged();
}

void CallSettingsCardHelper::setComfortCallValue() {
    const auto &ccv = setting_ComfortCall->getValue();
    if (ccv.length() < 1){
        return;
    }
    m_comfortCallEnabled = ccv.at(0).toInt() == 0x01;
    Q_EMIT comfortCallEnabledChanged();
}

void CallSettingsCardHelper::setAutoCallEnabled(const bool &enabled) {
    setting_AutoCall->sendSet({
         {static_cast<uint8_t>(enabled ? 0x01 : 0x00)}
    });
}

void CallSettingsCardHelper::setComfortCallEnabled(const bool &enabled) {
    setting_ComfortCall->sendSet({
          {static_cast<uint8_t>(enabled ? 0x01 : 0x00)}
    });
}
