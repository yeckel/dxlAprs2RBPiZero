#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QSettings>
#include <QUdpSocket>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject* parent = nullptr);
    static constexpr int CONTROL_PORT = 7755;
signals:
    void startReceiving();
    void stopReceiving();
    void newFreq(double mHz);
public slots:
    void readPendingDatagrams();
private:
    QUdpSocket udpSocket{this};
    QSettings settings{this};
};

#endif // SERVER_H
