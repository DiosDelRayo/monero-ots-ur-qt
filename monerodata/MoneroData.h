#ifndef MONERO_DATA_H
#define MONERO_DATA_H

#include <QObject>
#include "MoneroTxData.h"
#include "MoneroWalletData.h"

class MoneroData : public QObject {
    Q_OBJECT

public:
    MoneroData(QObject *parent = nullptr);
    ~MoneroData();
    Q_INVOKABLE static MoneroTxData* parseTxData(const QString &data, bool fallbackToJson = true);
    Q_INVOKABLE static MoneroWalletData* parseWalletData(const QString &data, bool fallbackToJson = true);
    Q_INVOKABLE static MoneroTxData* parseTxDataUri(const QString &uri);
    Q_INVOKABLE static MoneroWalletData* parseWalletDataUri(const QString &uri);
    Q_INVOKABLE static MoneroTxData *parseTxDataJson(const QString &json);
    Q_INVOKABLE static MoneroWalletData *parseWalletDataJson(const QString &json);
    Q_INVOKABLE static QString buildTxDataUri(MoneroTxData &data);
    Q_INVOKABLE static QString buildWalletDataUri(MoneroWalletData &data);
    Q_INVOKABLE static QString buildWalletDataJson(MoneroWalletData &data);
    Q_INVOKABLE static QString buildTxDataJson(MoneroTxData &data);
};
#endif // MONERO_DATA_H
