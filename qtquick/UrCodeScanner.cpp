#include "UrCodeScanner.h"
#include <QVideoProbe>
#include <QCamera>

UrCodeScanner::UrCodeScanner()
    : UrReceiver()
{
    this->init();
}

void UrCodeScanner::init() {
    UrReceiver::init();
    m_probe = new QVideoProbe(this);
    connect(m_probe, &QVideoProbe::videoFrameProbed, this, &UrReceiver::onFrameCaptured);
    // for backward compability
    connect(this, &UrCodeScanner::qrDataReceived, this, [this](const QString &data){
        emit this->decoded(data);
    });
}

void UrCodeScanner::setSource(QCamera *source)
{
    if( source != m_camera) {
        m_probe->setSource(source);
        emit sourceChanged();
    }
}

// only Qr Scanning for backwards compability
void UrCodeScanner::setEnabled(bool enabled)
{
    if(m_handleFrames)
        this->stop();
    else
        this->startCapture(false);
    emit enabledChanged();
}

UrCodeScanner::~UrCodeScanner() {
    delete m_probe;
}
