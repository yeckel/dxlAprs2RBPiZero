#include "aprsframedecoder.h"
#include <QDebug>

AprsFrameDecoder::AprsFrameDecoder(QObject* parent): QObject(parent)
{
    udpSocket.bind(QHostAddress::LocalHost, 4010);
    connect(&udpSocket, &QUdpSocket::readyRead, this, &AprsFrameDecoder::readPendingDatagrams);
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
    QByteArray datagram;
    do
    {
        datagram.resize(udpSocket.pendingDatagramSize());
        udpSocket.readDatagram(datagram.data(), datagram.size());
    }
    while(udpSocket.hasPendingDatagrams());

    qDebug() << "Read:" << datagram;
    QList<QByteArray> localSplit = datagram.split(';');
    if(localSplit.size() != 2)
    {
        return;
    }
    QString readableData = QString(localSplit[1]);
    QRegExp rx("(\\D\\d+)\\s\\*(\\d+)h(.*)N\\/(.*)E.*A=(\\d+).*Clb=(.+)m\\/s");
    rx.indexIn(readableData);
    QStringList split = rx.capturedTexts();
    if(split.size() != 7)
    {
        qWarning() << split;
    }
    setSondeId(split[1]);
    setLatitude(split[3]);
    setLongitude(split[4]);
    setAltitude(QString(split[5]).toInt());
    setClimbingRate(QString(split[6]).toDouble());
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
