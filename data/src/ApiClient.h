#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonArray>

class ApiClient : public QObject
{
    Q_OBJECT

public:
    explicit ApiClient(QObject *parent = nullptr);

    void fetchEpisodes();
    void fetchCharacters(const QList<int> &characterIds);

signals:
    void episodesFetched(const QJsonArray &episodes);
    void charactersFetched(const QJsonArray &characters);
    void errorOccurred(const QString &error);

private:
    QNetworkAccessManager m_networkManager;
    static const QString BASE_URL;
};

#endif // APICLIENT_H
