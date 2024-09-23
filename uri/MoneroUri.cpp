#include "MoneroUri.h"
#include <QUrl>
#include <QUrlQuery>

MoneroUri::MoneroUri(QObject *parent) : QObject(parent) {}

MoneroUri::~MoneroUri() {}

MoneroTxData* MoneroUri::parseTxDataUri(const QString &uri) {
    QUrl url(uri);
    if (url.scheme()!= "monero") {
        // invalid scheme
        return nullptr;
    }
    MoneroTxData* data = new MoneroTxData(url.path());
    QUrlQuery query(url.query());
    data->setTxPaymentId(query.queryItemValue("tx_payment_id"));
    data->setRecipientName(query.queryItemValue("recipient_name"));
    data->setTxAmount(query.queryItemValue("tx_amount"));
    data->setTxDescription(query.queryItemValue("tx_description"));
    return data;
}

MoneroWalletData* MoneroUri::parseWalletDataUri(const QString &uri) {
    QUrl url(uri);
    if (url.scheme()!= "monero_wallet") {
        // invalid scheme
        return nullptr;
    }
    MoneroWalletData* data = new MoneroWalletData(url.path());
    QUrlQuery query(url.query());
    data->setSpendKey(query.queryItemValue("spend_key"));
    data->setViewKey(query.queryItemValue("view_key"));
    data->setMnemonicSeed(query.queryItemValue("mnemonic_seed"));
    data->setHeight(query.queryItemValue("height").toInt());
    return data;
}

QString MoneroUri::buildTxDataUri(MoneroTxData &data) {
    QUrl url;
    url.setScheme("monero");
    url.setPath(data.address());

    QUrlQuery query;
    if (!data.txPaymentId().isEmpty()) {
        query.addQueryItem("tx_payment_id", data.txPaymentId());
    }
    if (!data.recipientName().isEmpty()) {
        query.addQueryItem("recipient_name", data.recipientName());
    }
    if (!data.txAmount().isEmpty()) {
        query.addQueryItem("tx_amount", data.txAmount());
    }
    if (!data.txDescription().isEmpty()) {
        url.setFragment(data.txDescription());
    }

    url.setQuery(query);
    return url.toString();
}

QString MoneroUri::buildWalletDataUri(MoneroWalletData &data) {
    QUrl url;
    url.setScheme("monero_wallet");
    url.setPath(data.address());

    QUrlQuery query;
    if (!data.spendKey().isEmpty()) {
        query.addQueryItem("spend_key", data.spendKey());
    }
    if (!data.viewKey().isEmpty()) {
        query.addQueryItem("view_key", data.viewKey());
    }
    if (!data.mnemonicSeed().isEmpty()) {
        query.addQueryItem("mnemonic_seed", data.mnemonicSeed());
    }
    if (data.height()!= 0) {
        query.addQueryItem("height", QString::number(data.height()));
    }

    url.setQuery(query);
    return url.toString();
}
