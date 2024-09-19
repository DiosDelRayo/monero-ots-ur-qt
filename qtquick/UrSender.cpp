#include <QBuffer>
#include <QSvgRenderer>
#include <QPainter>
#include <QDebug>

#include "UrSender.h"

UrSender::UrSender()
    : QObject()
{
    connect(&m_timer, &QTimer::timeout, this, &UrSender::nextQR);
}

void UrSender::setData(const QString &type, const std::string &data) {
    m_type = type;
    m_data = data;
    
    m_timer.stop();
    allParts.clear();
    
    if (m_data.empty())
        return;
    
    ur::ByteVector cbor;
    ur::CborLite::encodeBytes(cbor, ur::string_to_bytes(m_data));
    ur::UR h = ur::UR(m_type.toStdString(), cbor);

    delete m_urencoder;
    m_urencoder = new ur::UREncoder(h, m_fragmentLength);

    for (int i=0; i < m_urencoder->seq_len(); i++) {
        allParts.append(m_urencoder->next_part());
    }
    m_timer.setInterval(m_speed);
    m_timer.start();
}

void UrSender::nextQR() {
    currentIndex = currentIndex % m_urencoder->seq_len();

    std::string data;
    if (m_fountainCodeEnabled) {
        data = m_urencoder->next_part();
    } else {
        data = allParts[currentIndex];
    }
    emit updateCurrentFrameInfo((currentIndex % m_urencoder->seq_len() + 1), m_urencoder->seq_len());
	m_qrcode = new QrCode{QString::fromStdString(data), QrCode::Version::AUTO, QrCode::ErrorCorrectionLevel::MEDIUM};
    emit updateQrCode(*m_qrcode);
	m_currentFrameInfo = QString("%1/%2").arg((currentIndex % m_urencoder->seq_len() + 1)).arg(m_urencoder->seq_len());
	emit currentFrameInfoChanged();
    currentIndex += 1;
}

void UrSender::onSettingsChanged(int fragmentLength, int speed, bool fountainCodeEnabled) {
    m_fragmentLength = fragmentLength;
    m_speed = speed;
    m_fountainCodeEnabled = fountainCodeEnabled;
}

QImage UrSender::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    Q_UNUSED(id)

    QSize actualSize = requestedSize.isValid() ? requestedSize : QSize(300, 300);
    if (size)
        *size = actualSize;

    QImage image(actualSize, QImage::Format_ARGB32);
    image.fill(Qt::white);  // Fill with white background
    if(m_qrcode == nullptr)
        return image;
    if (!m_svg) { // render from Pixmap to image instead via SVG
        image = m_qrcode->toPixmap(4).toImage();
        *size = image.size();
        return image;
    }
    QByteArray currentSvgData;
    QBuffer buffer(&currentSvgData);
    buffer.open(QIODevice::WriteOnly);
    m_qrcode->writeSvg(&buffer, 1, 4);  // Using 1 as DPI, we'll scale in the ImageProvider
    buffer.close();
    QSvgRenderer renderer(currentSvgData);
    QPainter painter(&image);
    renderer.render(&painter);
    return image;
}

UrSender::~UrSender() {
	if(m_urencoder)
		delete m_urencoder;
}

void UrSender::test() {
	setData("test-test", "efwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegiooooooooooooooooooefwegubwuegewingeilwngmflqwekmnglkewmgkl gnewrjgnwgwejgiewngweigilgmweignweingiewngioewngiowengiwegiweogweiomgoweimgigmwegowegio");
}
