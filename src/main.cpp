#include <QCoreApplication>
#include "server.h"
#include "decoderprocess.h"
#include "aprsframedecoder.h"
#include "displayhandler.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setOrganizationDomain("tomsik.eu");
    QCoreApplication::setApplicationName("DxlAprsStarter");
    Server server{&a};
    DecoderProcess decoderProcess{&a};
    QCoreApplication::connect(&server, &Server::startReceiving, &decoderProcess, &DecoderProcess::startDecoding);
    QCoreApplication::connect(&server, &Server::stopReceiving, &decoderProcess, &DecoderProcess::stopDecoding);
    AprsFrameDecoder aprsFrameDecoder{&a};
    DisplayHandler displayHandler{&a};
    QCoreApplication::connect(&aprsFrameDecoder, &AprsFrameDecoder::sondeIdChanged, &displayHandler, &DisplayHandler::onSondeNameChanged);
    QCoreApplication::connect(&aprsFrameDecoder, &AprsFrameDecoder::latitudeChanged, &displayHandler, &DisplayHandler::onLatitudeChanged);
    QCoreApplication::connect(&aprsFrameDecoder, &AprsFrameDecoder::longitudeChanged, &displayHandler, &DisplayHandler::onLongitudeChanged);
    QCoreApplication::connect(&aprsFrameDecoder, &AprsFrameDecoder::altitudeChanged, &displayHandler, &DisplayHandler::onAltitudeChanged);
    QCoreApplication::connect(&aprsFrameDecoder, &AprsFrameDecoder::climbingRateChanged, &displayHandler, &DisplayHandler::onClimbingRateChanged);
    return a.exec();
}
