#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

#include <QObject>
#include <QTimer>
#include "epd2in13.h"
#include "epdpaint.h"
#include "imagedata.h"

class DisplayHandler : public QObject
{
    Q_OBJECT
public:
    explicit DisplayHandler(QObject* parent = nullptr);

signals:

public slots:
    void onSondeNameChanged(QString sondeName);
    void onLatitudeChanged(QString latitude);
    void onLongitudeChanged(QString longitude);
    void onAltitudeChanged(double altitude);
    void onClimbingRateChanged(double climbingRate);
    void onLifesign();
private:
    double m_climbingRate{0.0};
    double m_altitude{0.0};
    QTimer lifeSign{this};
    uint8_t ls{0};
    Epd epd;
    unsigned char* frame_buffer;
    Paint* paint;
    void paitTouchDown();
};

#endif // DISPLAYHANDLER_H
