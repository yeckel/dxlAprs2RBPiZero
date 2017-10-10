#ifndef APRSFRAMEDECODER_H
#define APRSFRAMEDECODER_H

#include <QObject>
#include <QUdpSocket>

class AprsFrameDecoder: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString sondeId READ sondeId WRITE setSondeId NOTIFY sondeIdChanged)
    Q_PROPERTY(QString latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
    Q_PROPERTY(QString longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
    Q_PROPERTY(double altitude READ altitude WRITE setAltitude NOTIFY altitudeChanged)
    Q_PROPERTY(double climbingRate READ climbingRate WRITE setClimbingRate NOTIFY climbingRateChanged)
public:
    explicit AprsFrameDecoder(QObject* parent = nullptr);
    QString sondeId() const;
    QString latitude() const;
    QString  longitude() const;
    double altitude() const;
    double climbingRate() const;

signals:
    void sondeIdChanged(QString sondeId);
    void latitudeChanged(QString latitude);
    void longitudeChanged(QString longitude);
    void altitudeChanged(double altitude);
    void climbingRateChanged(double climbingRate);

public slots:
    void readPendingDatagrams();
    void setSondeId(QString sondeId);
    void setLatitude(QString latitude);
    void setLongitude(QString longitude);
    void setAltitude(double altitude);
    void setClimbingRate(double climbingRate);

private:
    QUdpSocket udpSocket{this};
    QString m_sondeId;
    QString m_latitude;
    QString m_longitude;
    double m_altitude;
    double m_climbingRate;
};

#endif // APRSFRAMEDECODER_H
