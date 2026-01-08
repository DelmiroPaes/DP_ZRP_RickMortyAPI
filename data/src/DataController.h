#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <QObject>
#include <QVariantMap>
#include "ApiClient.h"
#include "EpisodeModel.h"
#include "CharacterModel.h"

class DataController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(EpisodeModel* episodes READ episodes CONSTANT)
    Q_PROPERTY(CharacterModel* characters READ characters CONSTANT)
    Q_PROPERTY(QVariantMap selectedEpisode READ selectedEpisode NOTIFY selectedEpisodeChanged)
    Q_PROPERTY(bool isLoadingEpisodes READ isLoadingEpisodes NOTIFY isLoadingEpisodesChanged)
    Q_PROPERTY(bool isLoadingCharacters READ isLoadingCharacters NOTIFY isLoadingCharactersChanged)
    Q_PROPERTY(bool hasError READ hasError NOTIFY hasErrorChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

public:
    explicit DataController(QObject *parent = nullptr);

    EpisodeModel* episodes();
    CharacterModel* characters();
    QVariantMap selectedEpisode() const;
    bool isLoadingEpisodes() const;
    bool isLoadingCharacters() const;
    bool hasError() const;
    QString errorMessage() const;

    Q_INVOKABLE void loadEpisodes();
    Q_INVOKABLE void selectEpisode(int index);

signals:
    void selectedEpisodeChanged();
    void isLoadingEpisodesChanged();
    void isLoadingCharactersChanged();
    void hasErrorChanged();
    void errorMessageChanged();

private slots:
    void onEpisodesFetched(const QJsonArray &episodes);
    void onCharactersFetched(const QJsonArray &characters);
    void onError(const QString &error);

private:
    ApiClient m_apiClient;
    EpisodeModel m_episodeModel;
    CharacterModel m_characterModel;
    int m_selectedIndex = -1;
    bool m_isLoadingEpisodes = false;
    bool m_isLoadingCharacters = false;
    bool m_hasError = false;
    QString m_errorMessage;
};

#endif // DATACONTROLLER_H
