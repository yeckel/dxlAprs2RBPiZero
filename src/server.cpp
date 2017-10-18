#include "server.h"

Server::Server(QObject* parent) : QObject(parent)
{
    udpSocket.bind(QHostAddress::Any, settings.value("ControlPort", CONTROL_PORT).toInt());
    connect(&udpSocket, &QUdpSocket::readyRead, this, &Server::readPendingDatagrams);
}

void Server::readPendingDatagrams()
{
    QByteArray datagram;

    do
    {
        datagram.resize(udpSocket.pendingDatagramSize());
        udpSocket.readDatagram(datagram.data(), datagram.size());
    }
    while(udpSocket.hasPendingDatagrams());

    qDebug() << "Received:" << datagram;
    if(datagram.startsWith("Frequency:"))
    {
        QList<QByteArray> localSplit = datagram.split(':');
        if(localSplit.size() != 2)
        {
            qWarning() << "Wrong command size:" << localSplit.size();
            return;
        }
        bool ok;
        double mHz = localSplit[1].trimmed().toDouble(&ok);
        if(!ok)
        {
            qWarning() << "Cound not read frequency:" << localSplit[1].trimmed();
        }
        emit newFreq(mHz);
        return;
    }
    if(datagram.startsWith("Start"))
    {
        emit startReceiving();
        return;
    }
    if(datagram.startsWith("Stop"))
    {
        emit stopReceiving();
        return;
    }
}
