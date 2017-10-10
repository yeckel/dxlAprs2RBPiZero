#ifndef DECODERPROCESS_H
#define DECODERPROCESS_H

#include <QObject>
#include <QProcess>

class DecoderProcess : public QObject
{
    Q_OBJECT
public:
    explicit DecoderProcess(QObject* parent = nullptr);

signals:

public slots:
    void startDecoding();
    void stopDecoding();
private slots:
    void onErrorOccured(QProcess::ProcessError error);
    void onReadyRead();
private:
    QProcess rtlTcp{this};
    QProcess udpgate{this};
    QProcess sdrtst{this};
    QProcess sondeudp{this};
    QProcess sondemod{this};
};

#endif // DECODERPROCESS_H
