#ifndef URSENDER_H
#define URSENDER_H

#include <QTimer>
#include <QrCode.h>
#include <bc-ur.hpp>

class UrSender : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString currentFrameInfo READ currentFrameInfo NOTIFY currentFrameInfoChanged)

public:
	explicit UrSender();
	~UrSender();
    QString currentFrameInfo() const { return m_currentFrameInfo; }
	QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

signals:
    void updateQrCode(const QrCode &qrCode);
    void updateCurrentFrameInfo(int current, int total);
    void currentFrameInfoChanged();

public slots:
    void onSettingsChanged(int fragmentLength, int speed, bool fountainCodeEnabled);
    void setData(const QString &type, const std::string &data);
	void test();

private slots:
    void nextQR();

private:
    bool m_svg = true; // use SVG for reandering TODO: need to profile both methods
    QTimer m_timer;
    ur::UREncoder *m_urencoder = nullptr;
    QList<std::string> allParts;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    qsizetype currentIndex = 0;
#else
    int currentIndex = 0;
#endif
    
    QrCode *m_qrcode = nullptr;
    std::string m_data;
    QString m_type;
    int m_fragmentLength = 150;
    int m_speed = 80;
    bool m_fountainCodeEnabled = false;
	QString m_currentFrameInfo;
};
#endif // URSENDER_H
