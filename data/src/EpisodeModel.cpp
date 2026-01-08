#include "EpisodeModel.h"
#include <QJsonObject>
#include <QRegularExpression>

EpisodeModel::EpisodeModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int EpisodeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_episodes.count();
}

QVariant EpisodeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_episodes.count())
        return QVariant();

    const Episode &episode = m_episodes.at(index.row());

    switch (role) {
    case IdRole:
        return episode.id;
    case NameRole:
        return episode.name;
    case AirDateRole:
        return episode.airDate;
    case EpisodeCodeRole:
        return episode.episodeCode;
    case SeasonRole:
        return episode.season;
    case EpisodeNumberRole:
        return episode.episodeNumber;
    case CharacterIdsRole:
        return QVariant::fromValue(episode.characterIds);
    case ImageRole:
        return episode.image;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> EpisodeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[AirDateRole] = "airDate";
    roles[EpisodeCodeRole] = "episode";
    roles[SeasonRole] = "season";
    roles[EpisodeNumberRole] = "episodeNumber";
    roles[CharacterIdsRole] = "characterIds";
    roles[ImageRole] = "image";
    return roles;
}

void EpisodeModel::setEpisodes(const QJsonArray &episodes)
{
    beginResetModel();
    m_episodes.clear();

    for (const QJsonValue &value : episodes) {
        QJsonObject obj = value.toObject();
        Episode episode;
        episode.id = obj["id"].toInt();
        episode.name = obj["name"].toString();
        episode.airDate = obj["air_date"].toString();
        episode.episodeCode = obj["episode"].toString();
        parseEpisodeCode(episode.episodeCode, episode.season, episode.episodeNumber);
        episode.characterIds = parseCharacterIds(obj["characters"].toArray());
        episode.image = "";

        m_episodes.append(episode);
    }

    endResetModel();
    emit countChanged();
}

Episode EpisodeModel::getEpisode(int index) const
{
    if (index >= 0 && index < m_episodes.count()) {
        return m_episodes.at(index);
    }
    return Episode();
}

void EpisodeModel::parseEpisodeCode(const QString &code, int &season, int &episode)
{
    QRegularExpression re("S(\\d+)E(\\d+)");
    QRegularExpressionMatch match = re.match(code);

    if (match.hasMatch()) {
        season = match.captured(1).toInt();
        episode = match.captured(2).toInt();
    } else {
        season = 0;
        episode = 0;
    }
}

QList<int> EpisodeModel::parseCharacterIds(const QJsonArray &characters)
{
    QList<int> ids;
    QRegularExpression re("/(\\d+)$");

    for (const QJsonValue &value : characters) {
        QString url = value.toString();
        QRegularExpressionMatch match = re.match(url);
        if (match.hasMatch()) {
            ids.append(match.captured(1).toInt());
        }
    }

    return ids;
}
