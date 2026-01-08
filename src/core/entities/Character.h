#ifndef RICKMORTY_CORE_CHARACTER_H
#define RICKMORTY_CORE_CHARACTER_H

#include <string>
#include <cctype>

namespace rickmorty::core {

struct Character {
    int id = 0;
    std::string name;
    std::string status;    // "Alive", "Dead", "unknown"
    std::string species;
    std::string type;      // Sub-type (can be empty)
    std::string gender;    // "Male", "Female", "Genderless", "unknown"
    std::string origin;    // Origin location name
    std::string location;  // Current location name
    std::string imageUrl;
    int episodeCount = 0;  // Number of episodes the character appears in

    [[nodiscard]] std::string initial() const noexcept {
        if (name.empty()) {
            return "";
        }
        return std::string(1, static_cast<char>(std::toupper(static_cast<unsigned char>(name[0]))));
    }

    [[nodiscard]] bool isValid() const noexcept {
        return id > 0;
    }
};

}  // namespace rickmorty::core

#endif  // RICKMORTY_CORE_CHARACTER_H
