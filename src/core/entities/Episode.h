#ifndef RICKMORTY_CORE_EPISODE_H
#define RICKMORTY_CORE_EPISODE_H

#include <string>
#include <vector>

namespace rickmorty::core {

struct Episode {
    int id = 0;
    std::string name;
    std::string airDate;
    std::string episodeCode;  // e.g., "S01E01"
    int season = 0;
    int episodeNumber = 0;
    std::vector<int> characterIds;
    std::string imageUrl;

    [[nodiscard]] std::string formattedSeason() const noexcept {
        return "Season " + std::to_string(season) + ", Episode " + std::to_string(episodeNumber);
    }

    [[nodiscard]] bool isValid() const noexcept {
        return id > 0;
    }
};

}  // namespace rickmorty::core

#endif  // RICKMORTY_CORE_EPISODE_H
