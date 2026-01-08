#include "ApiService.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace rickmorty::services {

ApiService::ApiService(QObject* parent)
    : QObject(parent)
{
}

void ApiService::get(const QString& endpoint, JsonCallback callback)
{
    const QString url = QString(BASE_URL) + endpoint;
    QNetworkRequest request{QUrl(url)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = m_networkManager.get(request);

    connect(reply, &QNetworkReply::finished, this, [callback, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            callback(reply->errorString());
            return;
        }

        const QByteArray data = reply->readAll();
        const QJsonDocument doc = QJsonDocument::fromJson(data);

        if (doc.isArray()) {
            callback(doc.array());
        } else if (doc.isObject()) {
            const QJsonObject obj = doc.object();
            if (obj.contains("results")) {
                callback(obj["results"].toArray());
            } else {
                // Single object response - wrap in array
                QJsonArray arr;
                arr.append(obj);
                callback(arr);
            }
        } else {
            callback(QString("Invalid JSON response"));
        }
    });
}

}  // namespace rickmorty::services
