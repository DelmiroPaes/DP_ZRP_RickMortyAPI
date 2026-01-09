#ifndef RICKMORTY_CORE_IEPISODE_REPOSITORY_H
#define RICKMORTY_CORE_IEPISODE_REPOSITORY_H

#include <vector>
#include <functional>
#include <variant>
#include <string>
#include "../entities/Episode.h"

namespace rickmorty::core {

using EpisodeResult = std::variant<std::vector<Episode>, std::string>;
using EpisodeCallback = std::function<void(EpisodeResult)>;

class IEpisodeRepository {
public:
    virtual ~IEpisodeRepository() = default;

    virtual void fetchAllEpisodes(EpisodeCallback callback) = 0;

protected:
    IEpisodeRepository() = default;
    IEpisodeRepository(const IEpisodeRepository&) = default;
    IEpisodeRepository(IEpisodeRepository&&) = default;
    IEpisodeRepository& operator=(const IEpisodeRepository&) = default;
    IEpisodeRepository& operator=(IEpisodeRepository&&) = default;
};

}  // namespace rickmorty::core

#endif  // RICKMORTY_CORE_IEPISODE_REPOSITORY_H
