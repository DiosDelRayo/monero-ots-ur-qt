#include "UrRegister.h"

#include <QQmlApplicationEngine>
#include <QCamera>
#include "UrReceiver.h"
#include "UrCodeScanner.h"
#include "UrSender.h"
#include "UrImageProvider.h"
#include "qqmlcontext.h"

Q_DECLARE_METATYPE(UrCodeScanner)

namespace OtsUr {

    static UrSender* _urSender = nullptr;

	void registerTypes()
	{
		qmlRegisterType<UrReceiver>("OtsUr", 0, 1, "UrReceiver");
		qmlRegisterType<UrSender>("OtsUr", 0, 1, "UrSender");
    }

    void setupContext(QQmlApplicationEngine &engine) {
        UrImageProvider *urcodeImageProvider = new UrImageProvider();
        UrSender *urSender = new UrSender();
        urcodeImageProvider->setSender(urSender);
        engine.addImageProvider("urcode", urcodeImageProvider);
        engine.rootContext()->setContextProperty("urSender", urSender);
    }

    void setupCamera(QQmlApplicationEngine &engine)
    {
        QObject *urCamera = engine.rootObjects().first()->findChild<QObject*>("urCamera");
        if (!urCamera)
        {
            qCritical() << "QrCodeScanner : something went wrong !";
            return;
        }
        qWarning() << "QrCodeScanner : object found";
        QCamera *camera_ = qvariant_cast<QCamera*>(urCamera->property("mediaObject"));
        QObject *urFinder = engine.rootObjects().first()->findChild<QObject*>("QrFinder");
        qobject_cast<UrCodeScanner*>(urFinder)->setSource(camera_);
        engine.rootContext()->setContextProperty("urScanner", urFinder);
    }
}

