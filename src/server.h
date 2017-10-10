#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject* parent = nullptr);

signals:
    void startReceiving();
    void stopReceiving();
    void newFreq(double mHz);
public slots:
    void readPendingDatagrams();
private:
    QUdpSocket udpSocket{this};
};

#endif // SERVER_H
