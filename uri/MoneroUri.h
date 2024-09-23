#ifndef MONEROURI_H
#define MONEROURI_H

#include <QObject>
#include "MoneroTxData.h"
#include "MoneroWalletData.h"

class MoneroUri : public QObject {
    Q_OBJECT

public:
    MoneroUri(QObject *parent = nullptr);
    ~MoneroUri();
    Q_INVOKABLE static MoneroTxData* parseTxDataUri(const QString &uri);
    Q_INVOKABLE static MoneroWalletData* parseWalletDataUri(const QString &uri);
    Q_INVOKABLE static QString buildTxDataUri(MoneroTxData &data);
    Q_INVOKABLE static QString buildWalletDataUri(MoneroWalletData &data);
};
#endif // MONEROURI_H
