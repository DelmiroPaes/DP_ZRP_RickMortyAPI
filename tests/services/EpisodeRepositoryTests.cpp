#include <gtest/gtest.h>
#include <QJsonObject>
#include <QJsonArray>
#include "services/EpisodeRepository.h"

using namespace rickmorty::services;

// Tests for parseEpisodeCode()
TEST(EpisodeRepositoryTest, ParseEpisodeCode_ParsesValidCode) {
    int season = 0, episode = 0;
    EpisodeRepository::parseEpisodeCode("S01E05", season, episode);
    EXPECT_EQ(season, 1);
    EXPECT_EQ(episode, 5);
}

TEST(EpisodeRepositoryTest, ParseEpisodeCode_HandlesTwoDigitSeason) {
    int season = 0, episode = 0;
    EpisodeRepository::parseEpisodeCode("S10E05", season, episode);
    EXPECT_EQ(season, 10);
    EXPECT_EQ(episode, 5);
}

TEST(EpisodeRepositoryTest, ParseEpisodeCode_HandlesTwoDigitEpisode) {
    int season = 0, episode = 0;
    EpisodeRepository::parseEpisodeCode("S01E25", season, episode);
    EXPECT_EQ(season, 1);
    EXPECT_EQ(episode, 25);
}

TEST(EpisodeRepositoryTest, ParseEpisodeCode_HandlesBothTwoDigits) {
    int season = 0, episode = 0;
    EpisodeRepository::parseEpisodeCode("S10E25", season, episode);
    EXPECT_EQ(season, 10);
    EXPECT_EQ(episode, 25);
}

TEST(EpisodeRepositoryTest, ParseEpisodeCode_InvalidFormat_DoesNotModify) {
    int season = 99, episode = 99;
    EpisodeRepository::parseEpisodeCode("INVALID", season, episode);
    // Values remain unchanged when format doesn't match
    EXPECT_EQ(season, 99);
    EXPECT_EQ(episode, 99);
}

TEST(EpisodeRepositoryTest, ParseEpisodeCode_EmptyString_DoesNotModify) {
    int season = 99, episode = 99;
    EpisodeRepository::parseEpisodeCode("", season, episode);
    EXPECT_EQ(season, 99);
    EXPECT_EQ(episode, 99);
}

// Tests for parseCharacterIds()
TEST(EpisodeRepositoryTest, ParseCharacterIds_ExtractsSingleId) {
    QJsonArray chars;
    chars.append("https://rickandmortyapi.com/api/character/1");

    auto ids = EpisodeRepository::parseCharacterIds(chars);
    ASSERT_EQ(ids.size(), 1);
    EXPECT_EQ(ids[0], 1);
}

TEST(EpisodeRepositoryTest, ParseCharacterIds_ExtractsMultipleIds) {
    QJsonArray chars;
    chars.append("https://rickandmortyapi.com/api/character/1");
    chars.append("https://rickandmortyapi.com/api/character/42");
    chars.append("https://rickandmortyapi.com/api/character/183");

    auto ids = EpisodeRepository::parseCharacterIds(chars);
    ASSERT_EQ(ids.size(), 3);
    EXPECT_EQ(ids[0], 1);
    EXPECT_EQ(ids[1], 42);
    EXPECT_EQ(ids[2], 183);
}

TEST(EpisodeRepositoryTest, ParseCharacterIds_EmptyArray_ReturnsEmpty) {
    QJsonArray chars;
    auto ids = EpisodeRepository::parseCharacterIds(chars);
    EXPECT_TRUE(ids.empty());
}

TEST(EpisodeRepositoryTest, ParseCharacterIds_InvalidUrl_Skipped) {
    QJsonArray chars;
    chars.append("not-a-valid-url");
    chars.append("https://rickandmortyapi.com/api/character/5");

    auto ids = EpisodeRepository::parseCharacterIds(chars);
    ASSERT_EQ(ids.size(), 1);
    EXPECT_EQ(ids[0], 5);
}

// Tests for parseEpisode()
TEST(EpisodeRepositoryTest, ParseEpisode_CreatesValidEpisode) {
    QJsonObject json;
    json["id"] = 1;
    json["name"] = "Pilot";
    json["air_date"] = "December 2, 2013";
    json["episode"] = "S01E01";
    json["characters"] = QJsonArray();

    auto episode = EpisodeRepository::parseEpisode(json);
    EXPECT_EQ(episode.id, 1);
    EXPECT_EQ(episode.name, "Pilot");
    EXPECT_EQ(episode.airDate, "December 2, 2013");
    EXPECT_EQ(episode.episodeCode, "S01E01");
    EXPECT_EQ(episode.season, 1);
    EXPECT_EQ(episode.episodeNumber, 1);
    EXPECT_TRUE(episode.isValid());
}

TEST(EpisodeRepositoryTest, ParseEpisode_ParsesCharacterIds) {
    QJsonArray chars;
    chars.append("https://rickandmortyapi.com/api/character/1");
    chars.append("https://rickandmortyapi.com/api/character/2");

    QJsonObject json;
    json["id"] = 1;
    json["name"] = "Pilot";
    json["air_date"] = "";
    json["episode"] = "S01E01";
    json["characters"] = chars;

    auto episode = EpisodeRepository::parseEpisode(json);
    ASSERT_EQ(episode.characterIds.size(), 2);
    EXPECT_EQ(episode.characterIds[0], 1);
    EXPECT_EQ(episode.characterIds[1], 2);
}

TEST(EpisodeRepositoryTest, ParseEpisode_MissingFields_UsesDefaults) {
    QJsonObject json;
    // Only set id
    json["id"] = 42;

    auto episode = EpisodeRepository::parseEpisode(json);
    EXPECT_EQ(episode.id, 42);
    EXPECT_TRUE(episode.name.empty());
    EXPECT_EQ(episode.season, 0);
    EXPECT_EQ(episode.episodeNumber, 0);
}
