#include "UrRegister.h"

#include <QQmlApplicationEngine>
#include <QCamera>
#include "UrReceiver.h"
#include "UrCodeScanner.h"
#include "UrSender.h"
#include "UrImageProvider.h"
#include "../uri/MoneroUri.h"
#include "qqmlcontext.h"

namespace OtsUr {

    static UrSender* _urSender = nullptr;

	void registerTypes()
	{
		qmlRegisterType<UrReceiver>("OtsUr", 0, 1, "UrReceiver");
		qmlRegisterType<UrSender>("OtsUr", 0, 1, "UrSender");
        qmlRegisterType<UrCodeScanner>("OtsUr", 0, 1, "UrCodeScannerImpl");
        qmlRegisterType<MoneroUri>("OtsUr", 0, 1, "MoneroUri");
        qmlRegisterType<MoneroTxData>("OtsUr", 0, 1, "MoneroTxData");
        qmlRegisterType<MoneroWalletData>("OtsUr", 0, 1, "MoneroWalletData");
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
            qCritical() << "UrCodeScanner : couldn't get camera !";
            return;
        }
        qWarning() << "UrCodeScanner : object found";
        QCamera *camera = qvariant_cast<QCamera*>(urCamera->property("mediaObject"));
        if(!camera) {
            qCritical() << "UrCodeScanner : couldn't cast camera !";
            return;
        }
        qWarning() << "UrCodeScanner : got Camera";
        QObject *urScanner = engine.rootObjects().first()->findChild<QObject*>("urScanner");
        if(!urScanner) {
            qCritical() << "UrCodeScanner : couldn't get scanner !";
            return;
        }
        UrCodeScanner *scanner = qobject_cast<UrCodeScanner*>(urScanner);
        if(!scanner) {
            qCritical() << "UrCodeScanner : couldn't cast scanner !";
            return;
        }
        qWarning() << "UrCodeScanner : got Scanner" << scanner;
        scanner->init();
        // UrCodeScanner *scanner = new UrCodeScanner();
        qWarning() << "UrCodeScanner : initialized Scanner";
        scanner->setSource(camera);
        qWarning() << "UrCodeScanner : connected";
        engine.rootContext()->setContextProperty("urScannerObj", scanner);
        qWarning() << "UrCodeScanner : set urScanner";
    }
}
