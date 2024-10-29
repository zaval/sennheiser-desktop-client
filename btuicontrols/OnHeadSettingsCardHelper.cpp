//
// Created by Dmytrii Zavalnyi on 29.10.2024..
//

#include "OnHeadSettingsCardHelper.h"
#include "gaiaV3/GAIARfcommClient.h"

using namespace Qt::Literals::StringLiterals;

OnHeadSettingsCardHelper::OnHeadSettingsCardHelper(QObject *parent) :
    QObject(parent),
    m_smartPauseEnabled{false},
    m_onHeadEnabled{false},
    setting_SmartPause(GAIARfcommClient::getInstance().getManager()->getProperty("\x04\x95\x09\x0D"_ba)),
    setting_OnHeadDetection(GAIARfcommClient::getInstance().getManager()->getProperty("\x04\x95\x05\x01"_ba))
{
    if (setting_SmartPause != nullptr){
        connect(setting_SmartPause, &GAIAPropertyBase::valueChanged, this, &OnHeadSettingsCardHelper::setSmartPauseValue);
        setting_SmartPause->sendGet();
    }
    if (setting_OnHeadDetection != nullptr){
        connect(setting_OnHeadDetection, &GAIAPropertyBase::valueChanged, this, &OnHeadSettingsCardHelper::setOnHeadDetectionValue);
        setting_OnHeadDetection->sendGet();
    }
}

bool OnHeadSettingsCardHelper::getOnHeadEnabled() const {
    return m_onHeadEnabled;
}

bool OnHeadSettingsCardHelper::getSmartPauseEnabled() const {
    return m_smartPauseEnabled;
}

void OnHeadSettingsCardHelper::setOnHeadEnabled(const bool &enabled) {
    setting_OnHeadDetection->sendSet({
        {static_cast<uint8_t>(enabled ? 0x01 : 0x00)}
    });
}

void OnHeadSettingsCardHelper::setSmartPauseEnabled(const bool &enabled) {
    setting_SmartPause->sendSet({
        {static_cast<uint8_t>(enabled ? 0x01 : 0x00)}
    });
}

void OnHeadSettingsCardHelper::setOnHeadDetectionValue() {
    const auto &ohdv = setting_OnHeadDetection->getValue();
    if (ohdv.length() < 1){
        return;
    }
    m_onHeadEnabled = ohdv.at(0).toInt() == 0x01;
    Q_EMIT onHeadEnabledChanged();

}

void OnHeadSettingsCardHelper::setSmartPauseValue() {
    const auto &sspv = setting_SmartPause->getValue();
    if (sspv.length() < 1){
        return;
    }
    m_smartPauseEnabled = sspv.at(0).toInt() == 0x01;
    Q_EMIT smartPauseEnabledChanged();
}
