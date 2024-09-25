#include "UrCodeScanner.h"
#include "UrTypes.h"
#include <QVideoProbe>
#include <QCamera>

UrCodeScanner::UrCodeScanner()
    : QObject()
{
    this->init();
}

void UrCodeScanner::init() {
    m_thread = new ScanThread(this);
    m_probe = new QVideoProbe(this);
    connect(m_thread, &ScanThread::decoded, this, &UrCodeScanner::onDecoded);
    connect(m_probe, &QVideoProbe::videoFrameProbed, this, &UrCodeScanner::onFrameCaptured);
}

void UrCodeScanner::setSource(QCamera *source)
{
    if( source != m_camera) {
        m_probe->setSource(source);
        emit sourceChanged();
    }
}

void UrCodeScanner::startCapture(bool scan_ur, const QString &ur_type) {
    if(!ur_type.isEmpty())
        m_ur_type = ur_type;
    if(scan_ur)
       emit urCaptureStarted(m_ur_type);
    else
       emit qrCaptureStarted();
	emit estimatedCompletedPercentage(0.0);
    if (!m_thread->isRunning()) {
        m_thread->start();
    }
}

void UrCodeScanner::reset() {
    m_done = false;
    m_decoder = ur::URDecoder();
    m_thread->start();
    m_handleFrames = true;
}

void UrCodeScanner::stop() {
    m_thread->stop();
    m_handleFrames = false;
}

void UrCodeScanner::onFrameCaptured(const QVideoFrame &frame) {
    if (!m_handleFrames || !m_thread->isRunning())
        return;
    QImage img = this->videoFrameToImage(frame);
    if (img.format() == QImage::Format_ARGB32)
        m_thread->addImage(img);
}

QImage UrCodeScanner::videoFrameToImage(const QVideoFrame &videoFrame)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QImage image = videoFrame.toImage();
#else
    QImage image = videoFrame.image();
#endif
    if (image.isNull())
        return {};
    if (image.format() != QImage::Format_ARGB32)
        image = image.convertToFormat(QImage::Format_ARGB32);
    return image.copy();
}

void UrCodeScanner::onImage(const QImage &image) {
    if (!m_handleFrames || !m_thread->isRunning())
        return;
    if (image.format() == QImage::Format_ARGB32) {
        m_thread->addImage(image);
		return;
	}
	m_thread->addImage(image.copy().convertToFormat(QImage::Format_ARGB32));
}

void UrCodeScanner::onDecoded(const QString &data) {
    qWarning() << "QR FRAME DATA: " << data;
    if (m_done)
        return;
    if (!m_scan_ur) { // scan only a QR code
        m_done = true;
        m_thread->stop();
        emit qrDataReceived(data);
        return;
    }
    if(!m_ur_type.isEmpty() && !data.startsWith("ur:" + m_ur_type)) { //check for type
        emit unexpectedUrType(extractUrType(data));
        return;
    }
    bool success = m_decoder.receive_part(data.toStdString());
    if (!success)
        return;
    emit receivedFrames(m_decoder.received_part_indexes().size());
    emit expectedFrames(m_decoder.expected_part_count());
    emit scannedFrames(m_decoder.received_part_indexes().size(), m_decoder.expected_part_count());
    emit estimatedCompletedPercentage(m_decoder.estimated_percent_complete());
    if (m_decoder.is_complete()) {
        m_done = true;
        m_thread->stop();
        if(m_decoder.is_success())
            emit urDataReceived(QString::fromStdString(getURType()).toLower(), getURData());
        else if(m_decoder.is_failure())
            emit urDataFailed(getURError());
    }
}

std::string UrCodeScanner::getURData() {
    if (!m_decoder.is_success())
        return "";
    ur::ByteVector cbor = m_decoder.result_ur().cbor();
    std::string data;
    auto i = cbor.begin();
    auto end = cbor.end();
    ur::CborLite::decodeBytes(i, end, data);
    return data;
}

std::string UrCodeScanner::getURType() {
    if (!m_decoder.is_success())
        return "";
    return m_decoder.expected_type().value_or("");
}

QString UrCodeScanner::getURError() {
    if (!m_decoder.is_failure())
        return {};
    return QString::fromStdString(m_decoder.result_error().what());
}

UrCodeScanner::~UrCodeScanner() {
    m_thread->stop();
    m_thread->quit();
    if (!m_thread->wait(5000))
    {
        m_thread->terminate();
        m_thread->wait();
    }
    delete m_probe;
}

QString extractUrType(const QString& qrFrame) {
    if (!qrFrame.startsWith("ur:") || !qrFrame.contains('/'))
        return "";
    return qrFrame.mid(3, qrFrame.indexOf('/') - 3);
}
