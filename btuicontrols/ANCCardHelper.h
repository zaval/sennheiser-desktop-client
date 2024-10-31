#ifndef SENNHEISERDESKTOPCLIENT_ANCCARDHELPER_H
#define SENNHEISERDESKTOPCLIENT_ANCCARDHELPER_H


#include <QObject>
#include <QtQmlIntegration>
#include "gaiaV3/GAIAPropertyBase.h"

class ANCCardHelper: public QObject{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int ancTransparency READ getAncTransparency WRITE setAncTransparency NOTIFY ancTransparencyChanged)
    Q_PROPERTY(bool antiWindEnabled READ getAntiWindEnabled WRITE setAntiWindEnabled NOTIFY antiWindEnabledChanged)
    Q_PROPERTY(int antiWindValue READ getAntiWindValue WRITE setAntiWindValue NOTIFY antiWindValueChanged)
    Q_PROPERTY(bool comfortEnabled READ getComfortEnabled WRITE setComfortEnabled NOTIFY comfortEnabledChanged)
    Q_PROPERTY(bool adaptiveEnabled READ getAdaptiveEnabled WRITE setAdaptiveEnabled NOTIFY adaptiveEnabledChanged)
    Q_PROPERTY(bool ancEnabled READ getAncEnabled WRITE setAncEnabled NOTIFY ancEnabledChanged)

public:

    explicit ANCCardHelper(QObject *parent = nullptr);

private:
    GAIAPropertyBase *anc;
    GAIAPropertyBase *anc_transparency;
    GAIAPropertyBase *anc_status;

    int m_ancTransparency;
    bool m_antiWindEnabled;
    bool m_comfortEnabled;
    bool m_adaptiveEnabled;
    bool m_ancEnabled;
    int m_antiWindValue;

    QTimer *transparencyTimer;

public slots:
    int getAncTransparency() const;
    bool getAntiWindEnabled() const;
    bool getComfortEnabled() const;
    bool getAdaptiveEnabled() const;
    bool getAncEnabled() const;
    int getAntiWindValue() const;
    void setAncEnabled(const bool &value);
    void setAntiWindEnabled(const bool &value);
    void setComfortEnabled(const bool &value);
    void setAdaptiveEnabled(const bool &value);
    void setAntiWindValue(const int &value);
    void setAncTransparency(const int &value);

signals:
    void ancTransparencyChanged();
    void antiWindEnabledChanged();
    void comfortEnabledChanged();
    void adaptiveEnabledChanged();
    void ancEnabledChanged();
    void antiWindValueChanged();

private slots:
    void ancValueChanged();
    void ancTransparencyValueChanged();
    void ancStatusValueChanged();
    void transparencyTimerTimedOut();

};


#endif //SENNHEISERDESKTOPCLIENT_ANCCARDHELPER_H
