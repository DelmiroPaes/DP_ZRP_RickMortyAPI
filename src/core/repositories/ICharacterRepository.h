#ifndef RICKMORTY_CORE_ICHARACTER_REPOSITORY_H
#define RICKMORTY_CORE_ICHARACTER_REPOSITORY_H

#include <vector>
#include <functional>
#include <variant>
#include <string>
#include "../entities/Character.h"

namespace rickmorty::core {

using CharacterResult = std::variant<std::vector<Character>, std::string>;
using CharacterCallback = std::function<void(CharacterResult)>;

class ICharacterRepository {
public:
    virtual ~ICharacterRepository() = default;

    virtual void fetchCharacters(const std::vector<int>& ids, CharacterCallback callback) = 0;

protected:
    ICharacterRepository() = default;
    ICharacterRepository(const ICharacterRepository&) = default;
    ICharacterRepository(ICharacterRepository&&) = default;
    ICharacterRepository& operator=(const ICharacterRepository&) = default;
    ICharacterRepository& operator=(ICharacterRepository&&) = default;
};

}  // namespace rickmorty::core

#endif  // RICKMORTY_CORE_ICHARACTER_REPOSITORY_H
