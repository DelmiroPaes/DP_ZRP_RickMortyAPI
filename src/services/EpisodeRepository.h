#ifndef RICKMORTY_SERVICES_EPISODE_REPOSITORY_H
#define RICKMORTY_SERVICES_EPISODE_REPOSITORY_H

#include <QObject>
#include "core/repositories/IEpisodeRepository.h"

namespace rickmorty::services {

class ApiService;

class EpisodeRepository : public QObject, public core::IEpisodeRepository {
    Q_OBJECT

public:
    explicit EpisodeRepository(ApiService* apiService, QObject* parent = nullptr);

    void fetchAllEpisodes(core::EpisodeCallback callback) override;

private:
    ApiService* m_apiService;

    static core::Episode parseEpisode(const QJsonObject& json);
    static void parseEpisodeCode(const QString& code, int& season, int& episode);
    static std::vector<int> parseCharacterIds(const QJsonArray& characters);
};

}  // namespace rickmorty::services

#endif  // RICKMORTY_SERVICES_EPISODE_REPOSITORY_H
