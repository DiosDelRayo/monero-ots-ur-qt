#include "UrRegister.h"

#include <QQmlApplicationEngine>
#include "UrReceiver.h"
#include "UrSender.h"
#include "UrImageProvider.h"
#include "qqmlcontext.h"

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
}

