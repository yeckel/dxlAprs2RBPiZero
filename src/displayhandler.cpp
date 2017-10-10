#include "displayhandler.h"
#include <QDebug>
#include <QTime>
#include "fonts.h"

static constexpr int UNCOLORED = 0;
static constexpr int COLORED = 1;

DisplayHandler::DisplayHandler(QObject* parent) : QObject(parent)
{
    lifeSign.setInterval(1000);
    lifeSign.setSingleShot(false);
    lifeSign.start();
    connect(&lifeSign, &QTimer::timeout, this, &DisplayHandler::onLifesign);
    if(epd.Init(lut_full_update) != 0)
    {
        qDebug() << "Display init failed";
    }
    frame_buffer = (unsigned char*)malloc(epd.width / 8 * epd.height);
    paint = new Paint{frame_buffer, epd.width, epd.height};
    paint->Clear(COLORED);
    paint->SetRotate(ROTATE_90);
    paint->DrawStringAt(0, 10, "ID:", &Font20, UNCOLORED);
    paint->DrawStringAt(0, 30, "Alt:", &Font24, UNCOLORED);
    paint->DrawStringAt(0, 54, "Coo:", &Font16, UNCOLORED);
    paint->DrawStringAt(0, 70, "Clmb:", &Font20, UNCOLORED);
    paint->DrawStringAt(0, 90, "Frame:", &Font20, UNCOLORED);
    paint->DrawStringAt(150, 90, "LS:", &Font16, UNCOLORED);
    paint->DrawStringAt(0, 112, "TD:", &Font16, UNCOLORED);
    epd.SetFrameMemory(paint->GetImage(), 0, 0, paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
    epd.SetFrameMemory(paint->GetImage(), 0, 0, paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
    if(epd.Init(lut_partial_update) != 0)
    {
        qDebug() << "e-Paper init failed";
    }
    //    onLatitudeChanged("5231.77");
    //    onLongitudeChanged("01311.66");
    //    onAltitudeChanged(38320);
    //    onClimbingRateChanged(-5.6);
    //    onSondeNameChanged("N1320115");
}

void DisplayHandler::onSondeNameChanged(QString sondeName)
{
    qDebug() << sondeName;
    paint->SetHeight(125);
    paint->SetWidth(22);
    paint->Clear(COLORED);
    paint->DrawStringAt(0, 10, qPrintable(sondeName), &Font20, UNCOLORED);
    epd.SetFrameMemory(paint->GetImage(), 111, 50, paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
    epd.SetFrameMemory(paint->GetImage(), 111, 50, paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
}

void DisplayHandler::onLatitudeChanged(QString latitude)
{
    qDebug() << latitude;
    paint->SetHeight(90);
    paint->SetWidth(16);
    paint->Clear(COLORED);
    paint->DrawStringAt(0, 4, qPrintable(QString("%1N").arg(latitude)), &Font16, UNCOLORED);
    epd.SetFrameMemory(paint->GetImage(), 122 - 54, 45, paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
    epd.SetFrameMemory(paint->GetImage(), 122 - 54, 45, paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
}

void DisplayHandler::onLongitudeChanged(QString longitude)
{
    qDebug() << longitude;
    paint->SetHeight(100);
    paint->SetWidth(16);
    paint->Clear(COLORED);
    paint->DrawStringAt(0, 4, qPrintable(QString("%1E").arg(longitude)), &Font16, UNCOLORED);
    epd.SetFrameMemory(paint->GetImage(), 122 - 54, 135, paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
    epd.SetFrameMemory(paint->GetImage(), 122 - 54, 135, paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
}

void DisplayHandler::onAltitudeChanged(double altitude)
{
    qDebug() << altitude;
    m_altitude = altitude;
    paint->SetHeight(140);
    paint->SetWidth(24);
    paint->Clear(COLORED);
    paint->DrawStringAt(0, 6, qPrintable(QString("%1m").arg(altitude)), &Font24, UNCOLORED);
    epd.SetFrameMemory(paint->GetImage(), 122 - 30 - 12, 60, paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
    epd.SetFrameMemory(paint->GetImage(), 122 - 30 - 12, 60, paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
    paitTouchDown();
}

void DisplayHandler::onClimbingRateChanged(double climbingRate)
{
    qDebug() << climbingRate;
    m_climbingRate = climbingRate;
    paint->SetHeight(150);
    paint->SetWidth(20);
    paint->Clear(COLORED);
    paint->DrawStringAt(0, 10, qPrintable(QString("%1 m/s").arg(climbingRate)), &Font20, UNCOLORED);
    epd.SetFrameMemory(paint->GetImage(), 122 - 10 - 70, 70, paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
    epd.SetFrameMemory(paint->GetImage(), 122 - 10 - 70, 70, paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
    paitTouchDown();
}

void DisplayHandler::onLifesign()
{
    paint->SetHeight(50);
    paint->SetWidth(16);
    paint->Clear(COLORED);
    paint->DrawStringAt(0, 4, qPrintable(QString("%1").arg(int(ls))), &Font16, UNCOLORED);
    epd.SetFrameMemory(paint->GetImage(), 122 - 8 - 90, 190,  paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
    ls++;
}

void DisplayHandler::paitTouchDown()
{
    QTime timeToTouchdown{0, 0, 0};
    if(m_climbingRate == 0.0)
    {
        timeToTouchdown.setHMS(99, 59, 59);
    }
    else
    {
        timeToTouchdown = timeToTouchdown.addSecs(abs(round(m_altitude / m_climbingRate)));
    }
    QString tttd = timeToTouchdown.toString("hh:mm:ss");
    qDebug() << "To touchdown:" << tttd;
    paint->SetHeight(150);
    paint->SetWidth(16);
    paint->Clear(COLORED);
    paint->DrawStringAt(0, 4, qPrintable(tttd), &Font16, UNCOLORED);
    epd.SetFrameMemory(paint->GetImage(), 122 - 112, 40, paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
    epd.SetFrameMemory(paint->GetImage(), 122 - 112, 40, paint->GetWidth(), paint->GetHeight());
    epd.DisplayFrame();
}
