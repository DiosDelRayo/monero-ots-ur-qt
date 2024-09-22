#ifndef URCODESCANNER_h
#define URCODESCANNER_h

#include <QImage>
#include <QVideoFrame>
#include <string>

#include <ScanThread.h>
#include <bc-ur.hpp>
#include <ur-decoder.hpp>

class QVideoProbe;
class QCamera;

class UrCodeScanner: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QCamera* source READ source WRITE setSource NOTIFY sourceChanged)

public:
    UrCodeScanner();
    ~UrCodeScanner() override;
    void init();

    QCamera* source() { return m_camera; }
    
    Q_INVOKABLE void reset();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void pause();
    void setSource(QCamera *source);

    Q_INVOKABLE bool enabled() const { return m_handleFrames; }
    Q_INVOKABLE void setEnabled(bool enabled);

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

    void enabledChanged();
    void sourceChanged();
    // void decoded(const QString &data);
    // for backward compability
    void notifyError(const QString &error, bool warning = false);

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
    QVideoProbe *m_probe;
    QCamera *m_camera = nullptr;
};
#endif // URCODESCANNER_h
