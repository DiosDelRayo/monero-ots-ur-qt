#ifndef URCODESCANNER_h
#define URCODESCANNER_h

#include "UrTypes.h"
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
    Q_PROPERTY(QCamera* source READ source WRITE setSource NOTIFY sourceChanged)

public:
    UrCodeScanner();
    ~UrCodeScanner() override;
    void init();

    QCamera* source() { return m_camera; }
    
    Q_INVOKABLE void reset();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void startCapture(bool scan_ur = false, const QString &ur_type = "");
    Q_INVOKABLE void outputs() { startCapture(true, XMR_OUTPUT); }
    Q_INVOKABLE void keyImages() { startCapture(true, XMR_KEY_IMAGE); }
    Q_INVOKABLE void unsignedTx() { startCapture(true, XMR_TX_UNSIGNED); }
    Q_INVOKABLE void signedTx() { startCapture(true, XMR_TX_SIGNED); }
    Q_INVOKABLE void qr() { startCapture(false, ""); }
    void setSource(QCamera *source);

signals:
    void qrDataReceived(const QString &data);
    void urDataReceived(const QString &type, const std::string &data);
    void urDataFailed(const QString &errorMsg);
    void receivedFrames(int count);
    void expectedFrames(int total);
    void scannedFrames(int count, int total);
    void estimatedCompletedPercentage(float complete);
    void unexpectedUrType(const QString &ur_type);

    void urCaptureStarted(const QString &type);
    void qrCaptureStarted();

    void sourceChanged();
    void notifyError(const QString &error, bool warning = false);

public slots:
    void onFrameCaptured(const QVideoFrame &videoFrame);
    void onImage(const QImage &image);

private slots:
    void onDecoded(const QString &data);

private:
    bool m_scan_ur = false;
    QString m_ur_type = ""; // expected UR type
    bool m_done = false;
    ScanThread *m_thread;
    ur::URDecoder m_decoder;

    QImage videoFrameToImage(const QVideoFrame &videoFrame);
    std::string getURData();
    std::string getURType();
    QString getURError();

protected:
    static QString extractUrType(const QString& qrFrame);
    bool m_handleFrames = true;
    QVideoProbe *m_probe;
    QCamera *m_camera = nullptr;
};
#endif // URCODESCANNER_h
