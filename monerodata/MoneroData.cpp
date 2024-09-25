#include "MoneroData.h"
#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonDocument>

MoneroData::MoneroData(QObject *parent) : QObject(parent) {}

MoneroData::~MoneroData() {}

MoneroTxData *MoneroData::parseTxData(const QString &data, bool fallbackToJson)
{
    MoneroTxData* out = parseTxDataUri(data);
    if(out == nullptr && fallbackToJson)
        out = parseTxDataJson(data);
    return out;
}

MoneroWalletData *MoneroData::parseWalletData(const QString &data, bool fallbackToJson)
{
    MoneroWalletData* out = parseWalletDataUri(data);
    if(out == nullptr && fallbackToJson)
        out = parseWalletDataJson(data);
    return out;
}

MoneroTxData* MoneroData::parseTxDataUri(const QString &uri) {
    QUrl url(uri);
    if (url.scheme()!= "monero")
        return nullptr; // invalid scheme
    MoneroTxData* data = new MoneroTxData(url.path());
    QUrlQuery query(url.query());
    data->setTxPaymentId(query.queryItemValue("tx_payment_id"));
    data->setRecipientName(query.queryItemValue("recipient_name"));
    data->setTxAmount(query.queryItemValue("tx_amount"));
    data->setTxDescription(query.queryItemValue("tx_description"));
    return data;
}

MoneroWalletData* MoneroData::parseWalletDataUri(const QString &uri) {
    QUrl url(uri);
    if (url.scheme()!= "monero_wallet")
        return nullptr; // invalid scheme
    MoneroWalletData* data = new MoneroWalletData(url.path());
    QUrlQuery query(url.query());
    data->setSpendKey(query.queryItemValue("spend_key"));
    data->setViewKey(query.queryItemValue("view_key"));
    data->setMnemonicSeed(query.queryItemValue("mnemonic_seed"));
    data->setHeight(query.queryItemValue("height").toInt());
    return data;
}

QString MoneroData::buildTxDataUri(MoneroTxData &data) {
    QUrl url;
    url.setScheme("monero");
    url.setPath(data.address());
    QUrlQuery query;
    if (!data.txPaymentId().isEmpty())
        query.addQueryItem("tx_payment_id", data.txPaymentId());
    if (!data.recipientName().isEmpty())
        query.addQueryItem("recipient_name", data.recipientName());
    if (!data.txAmount().isEmpty())
        query.addQueryItem("tx_amount", data.txAmount());
    if (!data.txDescription().isEmpty())
        url.setFragment(data.txDescription());
    url.setQuery(query);
    return url.toString();
}

QString MoneroData::buildWalletDataUri(MoneroWalletData &data) {
    QUrl url;
    url.setScheme("monero_wallet");
    url.setPath(data.address());
    QUrlQuery query;
    if (!data.spendKey().isEmpty())
        query.addQueryItem("spend_key", data.spendKey());
    if (!data.viewKey().isEmpty())
        query.addQueryItem("view_key", data.viewKey());
    if (!data.mnemonicSeed().isEmpty())
        query.addQueryItem("mnemonic_seed", data.mnemonicSeed());
    if (data.height()!= 0)
        query.addQueryItem("height", QString::number(data.height()));
    url.setQuery(query);
    return url.toString();
}

MoneroTxData* MoneroData::parseTxDataJson(const QString &json) {
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (doc.isNull() || !doc.isObject())
        return nullptr; // invalid JSON

    QJsonObject obj = doc.object();
    MoneroTxData* data = new MoneroTxData(obj["address"].toString());
    data->setTxPaymentId(obj["tx_payment_id"].toString());
    data->setRecipientName(obj["recipient_name"].toString());
    data->setTxAmount(obj["tx_amount"].toString());
    data->setTxDescription(obj["tx_description"].toString());
    return data;
}

MoneroWalletData* MoneroData::parseWalletDataJson(const QString &json) {
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (doc.isNull() || !doc.isObject())
        return nullptr; // invalid JSON

    QJsonObject obj = doc.object();
    MoneroWalletData* data = new MoneroWalletData(obj["address"].toString());
    data->setSpendKey(obj["spend_key"].toString());
    data->setViewKey(obj["view_key"].toString());
    data->setMnemonicSeed(obj["mnemonic_seed"].toString());
    data->setHeight(obj["height"].toInt());
    return data;
}

QString MoneroData::buildTxDataJson(MoneroTxData &data) {
    QJsonObject obj;
    obj["address"] = data.address();
    if (!data.txPaymentId().isEmpty())
        obj["tx_payment_id"] = data.txPaymentId();
    if (!data.recipientName().isEmpty())
        obj["recipient_name"] = data.recipientName();
    if (!data.txAmount().isEmpty())
        obj["tx_amount"] = data.txAmount();
    if (!data.txDescription().isEmpty())
        obj["tx_description"] = data.txDescription();

    QJsonDocument doc(obj);
    return QString(doc.toJson(QJsonDocument::Compact));
}

QString MoneroData::buildWalletDataJson(MoneroWalletData &data) {
    QJsonObject obj;
    obj["address"] = data.address();
    if (!data.spendKey().isEmpty())
        obj["spend_key"] = data.spendKey();
    if (!data.viewKey().isEmpty())
        obj["view_key"] = data.viewKey();
    if (!data.mnemonicSeed().isEmpty())
        obj["mnemonic_seed"] = data.mnemonicSeed();
    if (data.height() != 0)
        obj["height"] = data.height();

    QJsonDocument doc(obj);
    return QString(doc.toJson(QJsonDocument::Compact));
}
