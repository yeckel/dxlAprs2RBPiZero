#include "decoderprocess.h"
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

void DecoderProcess::onErrorOccured(QProcess::ProcessError error)
{
    qDebug() << "Error occured:" << error;
}

void DecoderProcess::onReadyRead()
{
    qDebug() << rtlTcp.readAllStandardOutput();
    qDebug() << rtlTcp.readAllStandardError();
}
