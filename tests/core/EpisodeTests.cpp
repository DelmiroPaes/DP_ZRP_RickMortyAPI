#include <gtest/gtest.h>
#include "core/entities/Episode.h"

using namespace rickmorty::core;

// Tests for Episode::formattedSeason()
TEST(EpisodeTest, FormattedSeason_ReturnsCorrectFormat) {
    Episode e{1, "Pilot", "", "S01E01", 1, 1, {}, ""};
    EXPECT_EQ(e.formattedSeason(), "Season 1, Episode 1");
}

TEST(EpisodeTest, FormattedSeason_HandlesTwoDigitNumbers) {
    Episode e{51, "Test", "", "S05E10", 5, 10, {}, ""};
    EXPECT_EQ(e.formattedSeason(), "Season 5, Episode 10");
}

TEST(EpisodeTest, FormattedSeason_HandlesZeroValues) {
    Episode e{1, "Test", "", "", 0, 0, {}, ""};
    EXPECT_EQ(e.formattedSeason(), "Season 0, Episode 0");
}

TEST(EpisodeTest, FormattedSeason_HandlesLargeNumbers) {
    Episode e{1, "Test", "", "", 100, 999, {}, ""};
    EXPECT_EQ(e.formattedSeason(), "Season 100, Episode 999");
}

// Tests for Episode::isValid()
TEST(EpisodeTest, IsValid_ReturnsTrueForPositiveId) {
    Episode e{1, "Pilot", "", "", 1, 1, {}, ""};
    EXPECT_TRUE(e.isValid());
}

TEST(EpisodeTest, IsValid_ReturnsTrueForLargeId) {
    Episode e{999999, "Pilot", "", "", 1, 1, {}, ""};
    EXPECT_TRUE(e.isValid());
}

TEST(EpisodeTest, IsValid_ReturnsFalseForZeroId) {
    Episode e{0, "Pilot", "", "", 1, 1, {}, ""};
    EXPECT_FALSE(e.isValid());
}

TEST(EpisodeTest, IsValid_ReturnsFalseForNegativeId) {
    Episode e{-1, "Pilot", "", "", 1, 1, {}, ""};
    EXPECT_FALSE(e.isValid());
}

// Tests for default constructed Episode
TEST(EpisodeTest, DefaultConstruction_HasZeroId) {
    Episode e;
    EXPECT_EQ(e.id, 0);
    EXPECT_FALSE(e.isValid());
}

TEST(EpisodeTest, DefaultConstruction_HasEmptyStrings) {
    Episode e;
    EXPECT_TRUE(e.name.empty());
    EXPECT_TRUE(e.airDate.empty());
    EXPECT_TRUE(e.episodeCode.empty());
}

TEST(EpisodeTest, DefaultConstruction_HasZeroSeasonAndEpisode) {
    Episode e;
    EXPECT_EQ(e.season, 0);
    EXPECT_EQ(e.episodeNumber, 0);
}

TEST(EpisodeTest, DefaultConstruction_HasEmptyCharacterIds) {
    Episode e;
    EXPECT_TRUE(e.characterIds.empty());
}

// Test character IDs
TEST(EpisodeTest, CharacterIds_CanStoreMultipleIds) {
    Episode e{1, "Pilot", "", "", 1, 1, {1, 2, 3, 42}, ""};
    ASSERT_EQ(e.characterIds.size(), 4);
    EXPECT_EQ(e.characterIds[0], 1);
    EXPECT_EQ(e.characterIds[3], 42);
}
