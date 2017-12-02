#include "aprsframedecoder.h"
#include <QDebug>

AprsFrameDecoder::AprsFrameDecoder(QObject* parent): QObject(parent)
{
    reconnectTimer.setSingleShot(true);
    reconnectTimer.setInterval(1000);
    connect(&tcpSocket, &QTcpSocket::readyRead, this, &AprsFrameDecoder::readPendingDatagrams);
    connect(&tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(&reconnectTimer, &QTimer::timeout, this, &AprsFrameDecoder::connectToudpGate);
    connectToudpGate();
}

QString AprsFrameDecoder::sondeId() const
{
    return m_sondeId;
}

QString AprsFrameDecoder::latitude() const
{
    return m_latitude;
}

QString AprsFrameDecoder::longitude() const
{
    return m_longitude;
}

double AprsFrameDecoder::altitude() const
{
    return m_altitude;
}

double AprsFrameDecoder::climbingRate() const
{
    return m_climbingRate;
}

void AprsFrameDecoder::readPendingDatagrams()
{
    QByteArray localReadAll = tcpSocket.readAll();
    QString datagram(localReadAll);

    qDebug() << "Read:" << datagram;
    QString readableData = QString(datagram);
    QRegExp rx(":;(.+)\\s\\*(\\d+)h(\\d+.\\d+\\w)\\/(\\d+.\\d+\\w)O(\\d{3})\\/(\\d{3})\\/A=(\\d{6})!.+Clb=(.+)m\\/s");
    rx.indexIn(readableData);
    QStringList split = rx.capturedTexts();
    if(split.size() != 9)
    {
        qWarning() << split;
    }
    setSondeId(split[1]);
    setLatitude(split[3]);
    setLongitude(split[4]);
    setAltitude(QString(split[7]).toInt() / FT2M);
    setClimbingRate(QString(split[8]).toDouble());
}

void AprsFrameDecoder::setSondeId(QString sondeId)
{
    if(m_sondeId == sondeId)
    {
        return;
    }

    m_sondeId = sondeId;
    emit sondeIdChanged(m_sondeId);
}

void AprsFrameDecoder::setLatitude(QString latitude)
{
    if(m_latitude == latitude)
    {
        return;
    }

    m_latitude = latitude;
    emit latitudeChanged(m_latitude);
}

void AprsFrameDecoder::setLongitude(QString longitude)
{
    if(m_longitude == longitude)
    {
        return;
    }

    m_longitude = longitude;
    emit longitudeChanged(m_longitude);
}

void AprsFrameDecoder::setAltitude(double altitude)
{
    if(qFuzzyCompare(m_altitude, altitude))
    {
        return;
    }

    m_altitude = altitude;
    emit altitudeChanged(m_altitude);
}

void AprsFrameDecoder::setClimbingRate(double climbingRate)
{
    if(qFuzzyCompare(m_climbingRate, climbingRate))
    {
        return;
    }

    m_climbingRate = climbingRate;
    emit climbingRateChanged(m_climbingRate);
}

void AprsFrameDecoder::displayError(QAbstractSocket::SocketError socketError)
{
    switch(socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qWarning() << "The host was not found. Please check the host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qWarning() << "The connection was refused by the peer. "
                   "Make sure the fortune server is running, "
                   "and check that the host name and port "
                   "settings are correct.";
        break;
    default:
        qWarning() << QString("The following error occurred: %1.").arg(tcpSocket.errorString());
    }
    reconnectTimer.start();
}

void AprsFrameDecoder::connectToudpGate()
{
    tcpSocket.abort();
    tcpSocket.connectToHost(settings.value("UdpGateIP", UDP_GATE_IP).toString(), settings.value("UdpGatePort", UDP_GATE_PORT).toInt());
}
