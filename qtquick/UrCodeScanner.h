#ifndef URCODESCANNER_h
#define URCODESCANNER_h

#include <QImage>
#include <QVideoFrame>

#include "UrReceiver.h"

class QVideoProbe;
class QCamera;

class UrCodeScanner: public UrReceiver
{
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QCamera* source READ source WRITE setSource NOTIFY sourceChanged)

public:
    UrCodeScanner();
    ~UrCodeScanner();
    QCamera* source() { return m_camera; }
    void setSource(QCamera *source);

    bool enabled() const { return m_handleFrames; }
    void setEnabled(bool enabled);

signals:
    void enabledChanged();
    void sourceChanged();
    void decoded(const QString &data);
    // for backward compability
    void notifyError(const QString &error, bool warning = false);

protected:
    QVideoProbe *m_probe;
    QCamera *m_camera = nullptr;
    void init();
};
#endif // URCODESCANNER_h
