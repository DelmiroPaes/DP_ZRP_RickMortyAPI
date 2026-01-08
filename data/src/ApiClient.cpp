#include "ApiClient.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

const QString ApiClient::BASE_URL = "https://rickandmortyapi.com/api";

ApiClient::ApiClient(QObject *parent)
    : QObject(parent)
{
}

void ApiClient::fetchEpisodes()
{
    QNetworkRequest request(QUrl(BASE_URL + "/episode"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = m_networkManager.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();
        QJsonArray results = obj["results"].toArray();

        emit episodesFetched(results);
    });
}

void ApiClient::fetchCharacters(const QList<int> &characterIds)
{
    if (characterIds.isEmpty()) {
        emit charactersFetched(QJsonArray());
        return;
    }

    QStringList idStrings;
    for (int id : characterIds) {
        idStrings.append(QString::number(id));
    }

    QString url = BASE_URL + "/character/" + idStrings.join(",");
    QNetworkRequest request{QUrl(url)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = m_networkManager.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply, characterIds]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonArray results;

        if (characterIds.size() == 1) {
            results.append(doc.object());
        } else {
            results = doc.array();
        }

        emit charactersFetched(results);
    });
}
