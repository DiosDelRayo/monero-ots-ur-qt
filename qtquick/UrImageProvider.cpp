#include <QImage>
#include "UrImageProvider.h"
#include "UrSender.h"
#include <QDebug>

UrImageProvider::UrImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image), m_sender(nullptr)
{
}

QImage UrImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    qDebug() << "UrImageProvider::requestImage: id:" << id << "size:" << size << "requestedSize:" << requestedSize;
    if (m_sender)
        return m_sender->requestImage(id, size, requestedSize);
    
    qDebug() << "UrImageProvider::requestImage: fallback (no UrSender set)";
    QSize actualSize = requestedSize.isValid() ? requestedSize : QSize(300, 300);
    if (size)
        *size = actualSize;

    QImage image(actualSize, QImage::Format_ARGB32);
    image.fill(Qt::red);  // Fill with red background as a fallback
    return image;
}

void UrImageProvider::setSender(UrSender *sender) {
    m_sender = sender;
}

UrImageProvider::~UrImageProvider() {
    // UrSender is not owned by UrImageProvider, so we don't delete it here
}
