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
	explicit UrReceiver();
	~UrReceiver() override;
    
    void startCapture(bool scan_ur = false);
    void reset();
    void stop();
    void pause();

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

private slots:
    void onDecoded(const QString &data);
    QImage videoFrameToImage(const QVideoFrame &videoFrame);
    std::string getURData();
    std::string getURType();
    QString getURError();

private:
    bool m_scan_ur = false;
    bool m_done = false;
    bool m_handleFrames = true;
    ScanThread *m_thread;
    ur::URDecoder m_decoder;
};
#endif // URRECEIVER_H
