#pragma once

#include <gmock/gmock.h>
#include "core/repositories/ICharacterRepository.h"

namespace rickmorty::tests {

class MockCharacterRepository : public core::ICharacterRepository {
public:
    MOCK_METHOD(void, fetchCharacters,
                (const std::vector<int>& ids, core::CharacterCallback callback),
                (override));
};

}  // namespace rickmorty::tests
