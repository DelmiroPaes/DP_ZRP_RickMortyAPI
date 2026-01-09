#pragma once

#include <gmock/gmock.h>
#include "core/repositories/IEpisodeRepository.h"

namespace rickmorty::tests {

class MockEpisodeRepository : public core::IEpisodeRepository {
public:
    MOCK_METHOD(void, fetchAllEpisodes, (core::EpisodeCallback callback), (override));
};

}  // namespace rickmorty::tests
