#include "decoderprocess.h"
#include <QFile>
#include <QLoggingCategory>

DecoderProcess::DecoderProcess(QObject* parent) : QObject(parent)
{
    rtlTcp.setProgram("rtl_tcp.sh");
    udpgate.setProgram("udpgate4.sh");
    sdrtst.setProgram("sdrtst.sh");
    sondeudp.setProgram("sondeudp.sh");
    sondemod.setProgram("sondemod.sh");
    connect(&rtlTcp, &QProcess::errorOccurred, this, &DecoderProcess::onErrorOccured);
    //connect(&rtlTcp, &QProcess::readyReadStandardOutput, this, &DecoderProcess::onReadyRead);
    //connect(&rtlTcp, &QProcess::readyReadStandardError, this, &DecoderProcess::onReadyRead);
}

void DecoderProcess::startDecoding()
{
    createSdrtstConfig();
    qDebug() << "Start rtlTcp";
    rtlTcp.start();
    qDebug() << "rtlTcpStarted:" << rtlTcp.waitForStarted(5000);
    qDebug() << "Start sdrtst";
    sdrtst.start();
    qDebug() << "Start rtlTcp";
    udpgate.start();
    qDebug() << "Start rtlTcp";
    sondemod.start();
    qDebug() << "Start rtlTcp";
    sondeudp.start();
}

void DecoderProcess::stopDecoding()
{
    rtlTcp.kill();
    udpgate.terminate();
    sdrtst.kill();
    sondeudp.kill();
    sondemod.kill();
    QProcess::execute("killall", QStringList("rtl_tcp"));
    QProcess::execute("killall", QStringList("udpgate4"));
    QProcess::execute("killall", QStringList("sdrtst"));
    QProcess::execute("killall", QStringList("sondeudp"));
    QProcess::execute("killall", QStringList("sondemod"));
}

void DecoderProcess::createSdrtstConfig()
{
    qDebug() << "Creating new config file with frequency:" << rxFrequency;
    QFile file("/tmp/receiver.cfg");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Unable to open /tmp/receiver.cfg for writing";
        return;
    }

    QTextStream out(&file);
    out << "p 5 " << settings.value("RTL_PPM", 0).toString() << "\n";
    out << "p 8 1\n";
    out << "f " << rxFrequency << "  10 0 60\n";
    file.close();
}

void DecoderProcess::onNewFrequency(double mHz)
{
    this->rxFrequency = mHz;
    createSdrtstConfig();
}

void DecoderProcess::onErrorOccured(QProcess::ProcessError error)
{
    qDebug() << "Error occured:" << error;
}

void DecoderProcess::onReadyRead()
{
    qDebug() << rtlTcp.readAllStandardOutput();
    qDebug() << rtlTcp.readAllStandardError();
}
