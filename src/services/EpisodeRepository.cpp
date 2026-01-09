#include "EpisodeRepository.h"
#include "ApiService.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>

namespace rickmorty::services {

EpisodeRepository::EpisodeRepository(ApiService* apiService, QObject* parent)
    : QObject(parent)
    , m_apiService(apiService)
{
}

void EpisodeRepository::fetchAllEpisodes(core::EpisodeCallback callback)
{
    m_apiService->get("/episode", [callback](ApiService::JsonResult result) {
        if (auto* error = std::get_if<QString>(&result)) {
            callback(error->toStdString());
            return;
        }

        const auto& jsonArray = std::get<QJsonArray>(result);
        std::vector<core::Episode> episodes;
        episodes.reserve(static_cast<size_t>(jsonArray.size()));

        for (const auto& value : jsonArray) {
            episodes.push_back(parseEpisode(value.toObject()));
        }

        callback(std::move(episodes));
    });
}

core::Episode EpisodeRepository::parseEpisode(const QJsonObject& json)
{
    core::Episode episode;
    episode.id = json["id"].toInt();
    episode.name = json["name"].toString().toStdString();
    episode.airDate = json["air_date"].toString().toStdString();
    episode.episodeCode = json["episode"].toString().toStdString();
    parseEpisodeCode(QString::fromStdString(episode.episodeCode),
                     episode.season, episode.episodeNumber);
    episode.characterIds = parseCharacterIds(json["characters"].toArray());
    return episode;
}

void EpisodeRepository::parseEpisodeCode(const QString& code, int& season, int& episode)
{
    static const QRegularExpression re("S(\\d+)E(\\d+)");
    const auto match = re.match(code);
    if (match.hasMatch()) {
        season = match.captured(1).toInt();
        episode = match.captured(2).toInt();
    }
}

std::vector<int> EpisodeRepository::parseCharacterIds(const QJsonArray& characters)
{
    std::vector<int> ids;
    ids.reserve(static_cast<size_t>(characters.size()));

    static const QRegularExpression re("/(\\d+)$");
    for (const auto& value : characters) {
        const auto match = re.match(value.toString());
        if (match.hasMatch()) {
            ids.push_back(match.captured(1).toInt());
        }
    }
    return ids;
}

}  // namespace rickmorty::services
