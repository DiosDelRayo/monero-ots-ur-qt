#include "UrReceiver.h"

UrReceiver::UrReceiver()
	: QObject()
	, m_thread(new ScanThread(this))
{
    connect(m_thread, &ScanThread::decoded, this, &UrReceiver::onDecoded);
}

void UrReceiver::startCapture(bool scan_ur) {
    if(scan_ur)
       emit urCaptureStarted();
    else
       emit qrCaptureStarted();
	emit estimatedCompletedPercentage(0.0);
    if (!m_thread->isRunning()) {
        m_thread->start();
    }
}

void UrReceiver::reset() {
    m_done = false;
    m_decoder = ur::URDecoder();
    m_thread->start();
    m_handleFrames = true;
}

void UrReceiver::stop() {
    m_thread->stop();
}

void UrReceiver::pause() {
    m_handleFrames = false;
}

void UrReceiver::onFrameCaptured(const QVideoFrame &frame) {
    if (!m_handleFrames || !m_thread->isRunning())
        return;
    QImage img = this->videoFrameToImage(frame);
    if (img.format() == QImage::Format_ARGB32)
        m_thread->addImage(img);
}

QImage UrReceiver::videoFrameToImage(const QVideoFrame &videoFrame)
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

void UrReceiver::onImage(const QImage &image) {
    if (!m_handleFrames || !m_thread->isRunning())
        return;
    if (image.format() == QImage::Format_ARGB32) {
        m_thread->addImage(image);
		return;
	}
	m_thread->addImage(image.copy().convertToFormat(QImage::Format_ARGB32));
}

void UrReceiver::onDecoded(const QString &data) {
    if (m_done)
        return;
    if (m_scan_ur) {
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
                emit urDataReceived(QString::fromStdString(getURType()), getURData());
            else if(m_decoder.is_failure())
                emit urDataFailed(getURError());
        }
        return;
    }
    m_done = true;
    m_thread->stop();
    emit qrDataReceived(data);
}

std::string UrReceiver::getURData() {
    if (!m_decoder.is_success())
        return "";
    ur::ByteVector cbor = m_decoder.result_ur().cbor();
    std::string data;
    auto i = cbor.begin();
    auto end = cbor.end();
    ur::CborLite::decodeBytes(i, end, data);
    return data;
}

std::string UrReceiver::getURType() {
    if (!m_decoder.is_success())
        return "";
    return m_decoder.expected_type().value_or("");
}

QString UrReceiver::getURError() {
    if (!m_decoder.is_failure())
        return {};
    return QString::fromStdString(m_decoder.result_error().what());
}

UrReceiver::~UrReceiver()
{
    m_thread->stop();
    m_thread->quit();
    if (!m_thread->wait(5000))
    {
        m_thread->terminate();
        m_thread->wait();
    }
}
