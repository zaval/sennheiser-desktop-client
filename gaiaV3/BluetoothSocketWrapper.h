#ifndef SENNHEISERDESKTOPCLIENT_BLUETOOTHSOCKETWRAPPER_H
#define SENNHEISERDESKTOPCLIENT_BLUETOOTHSOCKETWRAPPER_H


#include <QObject>
#include <QBluetoothSocket>
#include <QQueue>

class BluetoothSocketWrapper: public QObject {

    Q_OBJECT

public:
    explicit BluetoothSocketWrapper(QObject *parent = nullptr);
    bool isOpen() const;
    void close();
    QByteArray readAll();
    void connectToService(const QBluetoothAddress &address, quint16 port, QIODevice::OpenMode openMode = QIODevice::ReadWrite);
    qint64 write(const QByteArray &data);


private:
    QBluetoothSocket *m_socket;
    bool isDemo;

    QMap<QByteArray, QByteArray> m_dataMap;
    QQueue<QByteArray> m_queue;

signals:
    void connected();
    void disconnected();
    void readyRead();
};


#endif //SENNHEISERDESKTOPCLIENT_BLUETOOTHSOCKETWRAPPER_H
