#ifndef DECODERPROCESS_H
#define DECODERPROCESS_H

#include <QObject>
#include <QProcess>
#include <QSettings>

class DecoderProcess : public QObject
{
    Q_OBJECT
public:
    explicit DecoderProcess(QObject* parent = nullptr);

signals:

public slots:
    void startDecoding();
    void stopDecoding();
    void onNewFrequency(double rxFrequency);
private slots:
    void onErrorOccured(QProcess::ProcessError error);
    void onReadyRead();
private:
    QSettings settings{this};
    QProcess rtlTcp{this};
    QProcess udpgate{this};
    QProcess sdrtst{this};
    QProcess sondeudp{this};
    QProcess sondemod{this};
    void createSdrtstConfig();
    double rxFrequency{405.1};
};

#endif // DECODERPROCESS_H
