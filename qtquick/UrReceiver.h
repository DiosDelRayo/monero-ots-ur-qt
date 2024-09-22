#ifndef URRECEIVER_H
#define URRECEIVER_H

#include <QImage>
#include <QVideoFrame>
#include <string>

#include <ScanThread.h>
#include <bc-ur.hpp>
#include <ur-decoder.hpp>

class UrReceiver : public QObject
{
	Q_OBJECT

public:
    UrReceiver();
	~UrReceiver() override;
    
    Q_INVOKABLE void reset();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void pause();

signals:
    void qrDataReceived(const QString &data);
    void urDataReceived(const QString &type, const std::string &data);
    void urDataFailed(const QString &errorMsg);
    void receivedFrames(int count);
    void expectedFrames(int total);
    void scannedFrames(int count, int total);
    void estimatedCompletedPercentage(float complete);

    void urCaptureStarted();
    void qrCaptureStarted();

public slots:
    void onFrameCaptured(const QVideoFrame &videoFrame);
    void onImage(const QImage &image);
    void startCapture(bool scan_ur = false);

private slots:
    void onDecoded(const QString &data);

private:
    bool m_scan_ur = false;
    bool m_done = false;
    ScanThread *m_thread;
    ur::URDecoder m_decoder;

    QImage videoFrameToImage(const QVideoFrame &videoFrame);
    std::string getURData();
    std::string getURType();
    QString getURError();
protected:
    bool m_handleFrames = true;
    void init();
};
#endif // URRECEIVER_H
