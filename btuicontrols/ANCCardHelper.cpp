#include "ANCCardHelper.h"
#include "gaiaV3/GAIARfcommClient.h"

using namespace Qt::Literals::StringLiterals;

ANCCardHelper::ANCCardHelper(QObject *parent) :
        QObject(parent),
        anc(GAIARfcommClient::getInstance().getManager()->getProperty("\x04\x95\x1B\x01"_ba)),
        anc_transparency(GAIARfcommClient::getInstance().getManager()->getProperty("\x04\x95\x1B\x03"_ba)),
        anc_status(GAIARfcommClient::getInstance().getManager()->getProperty("\x04\x95\x1B\x05"_ba)),
        m_adaptiveEnabled(false),
        m_ancTransparency(0),
        m_antiWindEnabled(false),
        m_comfortEnabled(false),
        m_ancEnabled(false),
        m_antiWindValue{0},
        transparencyTimer(new QTimer(this))
{
    if (anc != nullptr){
        connect(anc, &GAIAPropertyBase::valueChanged, this, &ANCCardHelper::ancValueChanged);
        anc->sendGet();
    }
    if (anc_transparency != nullptr){
        connect(anc_transparency, &GAIAPropertyBase::valueChanged, this, &ANCCardHelper::ancTransparencyValueChanged);
        anc_transparency->sendGet();
    }
    if (anc_status != nullptr){
        connect(anc_status, &GAIAPropertyBase::valueChanged, this, &ANCCardHelper::ancStatusValueChanged);
        anc_status->sendGet();
    }

    connect(transparencyTimer, &QTimer::timeout, this, &ANCCardHelper::transparencyTimerTimedOut);
}

void ANCCardHelper::ancValueChanged() {
    const auto &ancv = anc->getValue();
    if (ancv.length() < 6){
        return;
    }
    m_antiWindEnabled = ancv[1] == 1 || ancv[1] == 2;
    m_antiWindValue = ancv[1].toInt();
    m_comfortEnabled = ancv[3] == 1;
    m_adaptiveEnabled = ancv[5] == 1;
    Q_EMIT antiWindEnabledChanged();
    Q_EMIT comfortEnabledChanged();
    Q_EMIT adaptiveEnabledChanged();
    Q_EMIT antiWindValueChanged();
}

void ANCCardHelper::ancTransparencyValueChanged() {
    const auto &anctv = anc_transparency->getValue();
    if (anctv.length() < 1){
        return;
    }
    m_ancTransparency = anctv.at(0).toInt();
    Q_EMIT ancTransparencyChanged();
}

int ANCCardHelper::getAncTransparency() const {
    return m_ancTransparency;
}

bool ANCCardHelper::getAntiWindEnabled() const {
    return m_antiWindEnabled;
}

bool ANCCardHelper::getComfortEnabled() const {
    return m_comfortEnabled;
}

bool ANCCardHelper::getAdaptiveEnabled() const {
    return m_adaptiveEnabled;
}

void ANCCardHelper::ancStatusValueChanged() {
    const auto &aev = anc_status->getValue();
    if (aev.length() < 1){
        return;
    }
    m_ancEnabled = aev.at(0).toInt() == 0x01;
    Q_EMIT ancEnabledChanged();
}

void ANCCardHelper::setAncEnabled(const bool &value) {
    qDebug() << "ANCCardHelper::setAncEnabled" << value;
    anc_status->sendSet(QVariantList{static_cast<uint8_t>(value ? 0x01 : 0x00)});
}

bool ANCCardHelper::getAncEnabled() const {
    return m_ancEnabled;
}

void ANCCardHelper::setAntiWindEnabled(const bool &value) {
    qDebug() << "ANCCardHelper::setAntiWindEnabled" << value;
    anc->sendSet({
        {static_cast<uint8_t >(0x01)},
        {static_cast<uint8_t>(value ? 0x01 : 0x00)}
    });
}

void ANCCardHelper::setComfortEnabled(const bool &value) {
    qDebug() << "ANCCardHelper::setComfortEnabled" << value;
    anc->sendSet({
         {static_cast<uint8_t >(0x02)},
         {static_cast<uint8_t>(value ? 0x01 : 0x00)}
    });
}

void ANCCardHelper::setAdaptiveEnabled(const bool &value) {
    qDebug() << "ANCCardHelper::setAdaptiveEnabled" << value;
    anc->sendSet({
         {static_cast<uint8_t >(0x03)},
         {static_cast<uint8_t>(value ? 0x01 : 0x00)}
    });

}

int ANCCardHelper::getAntiWindValue() const {
    return m_antiWindValue;
}

void ANCCardHelper::setAntiWindValue(const int &value) {
    qDebug() << "ANCCardHelper::setAntiWindValue" << value;
    anc->sendSet({
         {static_cast<uint8_t >(0x01)},
         {static_cast<uint8_t>(value)}
    });
}

void ANCCardHelper::setAncTransparency(const int &value) {

    if (transparencyTimer->isActive()){
        transparencyTimer->stop();
    }
    m_ancTransparency = value;
    transparencyTimer->start(1000);
}

void ANCCardHelper::transparencyTimerTimedOut() {
    qDebug() << "&QTimer::timeout" << m_ancTransparency;
    transparencyTimer->stop();
    anc_transparency->sendSet({
        {static_cast<uint8_t>(m_ancTransparency)}
    });
}
