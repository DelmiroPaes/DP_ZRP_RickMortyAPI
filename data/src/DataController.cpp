#include "DataController.h"
#include <QDebug>

DataController::DataController(QObject *parent)
    : QObject(parent)
{
    connect(&m_apiClient, &ApiClient::episodesFetched,
            this, &DataController::onEpisodesFetched);
    connect(&m_apiClient, &ApiClient::charactersFetched,
            this, &DataController::onCharactersFetched);
    connect(&m_apiClient, &ApiClient::errorOccurred,
            this, &DataController::onError);
}

EpisodeModel* DataController::episodes()
{
    return &m_episodeModel;
}

CharacterModel* DataController::characters()
{
    return &m_characterModel;
}

QVariantMap DataController::selectedEpisode() const
{
    if (m_selectedIndex < 0 || m_selectedIndex >= m_episodeModel.rowCount()) {
        return QVariantMap();
    }

    Episode episode = m_episodeModel.getEpisode(m_selectedIndex);
    QVariantMap map;
    map["index"] = m_selectedIndex;
    map["id"] = episode.id;
    map["name"] = episode.name;
    map["airDate"] = episode.airDate;
    map["episodeCode"] = episode.episodeCode;
    map["season"] = episode.season;
    map["episodeNumber"] = episode.episodeNumber;
    map["image"] = episode.image;

    return map;
}

bool DataController::isLoadingEpisodes() const
{
    return m_isLoadingEpisodes;
}

bool DataController::isLoadingCharacters() const
{
    return m_isLoadingCharacters;
}

bool DataController::hasError() const
{
    return m_hasError;
}

QString DataController::errorMessage() const
{
    return m_errorMessage;
}

void DataController::loadEpisodes()
{
    qDebug() << "DataController: Loading episodes...";
    m_isLoadingEpisodes = true;
    emit isLoadingEpisodesChanged();

    m_hasError = false;
    emit hasErrorChanged();

    m_apiClient.fetchEpisodes();
}

void DataController::selectEpisode(int index)
{
    if (index == m_selectedIndex) {
        return;
    }

    m_selectedIndex = index;
    emit selectedEpisodeChanged();

    if (index < 0 || index >= m_episodeModel.rowCount()) {
        m_characterModel.clear();
        return;
    }

    Episode episode = m_episodeModel.getEpisode(index);

    m_isLoadingCharacters = true;
    emit isLoadingCharactersChanged();

    m_apiClient.fetchCharacters(episode.characterIds);
}

void DataController::onEpisodesFetched(const QJsonArray &episodes)
{
    qDebug() << "DataController: Episodes fetched, count:" << episodes.size();
    m_episodeModel.setEpisodes(episodes);

    m_isLoadingEpisodes = false;
    emit isLoadingEpisodesChanged();
}

void DataController::onCharactersFetched(const QJsonArray &characters)
{
    m_characterModel.setCharacters(characters);

    m_isLoadingCharacters = false;
    emit isLoadingCharactersChanged();
}

void DataController::onError(const QString &error)
{
    qDebug() << "DataController: Error occurred:" << error;
    m_errorMessage = error;
    m_hasError = true;
    m_isLoadingEpisodes = false;
    m_isLoadingCharacters = false;

    emit errorMessageChanged();
    emit hasErrorChanged();
    emit isLoadingEpisodesChanged();
    emit isLoadingCharactersChanged();
}
