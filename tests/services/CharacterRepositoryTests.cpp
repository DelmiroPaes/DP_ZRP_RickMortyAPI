#include <gtest/gtest.h>
#include <QJsonObject>
#include <QJsonArray>
#include "services/CharacterRepository.h"

using namespace rickmorty::services;

// Tests for parseCharacter()
TEST(CharacterRepositoryTest, ParseCharacter_CreatesValidCharacter) {
    QJsonObject json;
    json["id"] = 1;
    json["name"] = "Rick Sanchez";
    json["status"] = "Alive";
    json["species"] = "Human";
    json["type"] = "";
    json["gender"] = "Male";
    json["image"] = "https://rickandmortyapi.com/api/character/avatar/1.jpeg";

    QJsonObject origin;
    origin["name"] = "Earth (C-137)";
    json["origin"] = origin;

    QJsonObject location;
    location["name"] = "Citadel of Ricks";
    json["location"] = location;

    QJsonArray episodes;
    episodes.append("https://rickandmortyapi.com/api/episode/1");
    episodes.append("https://rickandmortyapi.com/api/episode/2");
    episodes.append("https://rickandmortyapi.com/api/episode/3");
    json["episode"] = episodes;

    auto character = CharacterRepository::parseCharacter(json);

    EXPECT_EQ(character.id, 1);
    EXPECT_EQ(character.name, "Rick Sanchez");
    EXPECT_EQ(character.status, "Alive");
    EXPECT_EQ(character.species, "Human");
    EXPECT_EQ(character.type, "");
    EXPECT_EQ(character.gender, "Male");
    EXPECT_EQ(character.origin, "Earth (C-137)");
    EXPECT_EQ(character.location, "Citadel of Ricks");
    EXPECT_EQ(character.imageUrl, "https://rickandmortyapi.com/api/character/avatar/1.jpeg");
    EXPECT_EQ(character.episodeCount, 3);
    EXPECT_TRUE(character.isValid());
}

TEST(CharacterRepositoryTest, ParseCharacter_HandlesMinimalData) {
    QJsonObject json;
    json["id"] = 42;
    json["name"] = "Test Character";

    auto character = CharacterRepository::parseCharacter(json);

    EXPECT_EQ(character.id, 42);
    EXPECT_EQ(character.name, "Test Character");
    EXPECT_TRUE(character.status.empty());
    EXPECT_EQ(character.episodeCount, 0);
}

TEST(CharacterRepositoryTest, ParseCharacter_HandlesUnknownStatus) {
    QJsonObject json;
    json["id"] = 1;
    json["name"] = "Mystery";
    json["status"] = "unknown";

    auto character = CharacterRepository::parseCharacter(json);

    EXPECT_EQ(character.status, "unknown");
}

TEST(CharacterRepositoryTest, ParseCharacter_HandlesDeadStatus) {
    QJsonObject json;
    json["id"] = 1;
    json["name"] = "Dead Character";
    json["status"] = "Dead";

    auto character = CharacterRepository::parseCharacter(json);

    EXPECT_EQ(character.status, "Dead");
}

TEST(CharacterRepositoryTest, ParseCharacter_CountsEpisodes) {
    QJsonObject json;
    json["id"] = 1;
    json["name"] = "Test";

    QJsonArray episodes;
    for (int i = 1; i <= 51; ++i) {
        episodes.append(QString("https://rickandmortyapi.com/api/episode/%1").arg(i));
    }
    json["episode"] = episodes;

    auto character = CharacterRepository::parseCharacter(json);

    EXPECT_EQ(character.episodeCount, 51);
}

TEST(CharacterRepositoryTest, ParseCharacter_MissingOrigin_UsesEmpty) {
    QJsonObject json;
    json["id"] = 1;
    json["name"] = "Test";
    // No origin field

    auto character = CharacterRepository::parseCharacter(json);

    EXPECT_TRUE(character.origin.empty());
}

TEST(CharacterRepositoryTest, ParseCharacter_MissingLocation_UsesEmpty) {
    QJsonObject json;
    json["id"] = 1;
    json["name"] = "Test";
    // No location field

    auto character = CharacterRepository::parseCharacter(json);

    EXPECT_TRUE(character.location.empty());
}

TEST(CharacterRepositoryTest, ParseCharacter_InitialIsCorrect) {
    QJsonObject json;
    json["id"] = 1;
    json["name"] = "Morty Smith";

    auto character = CharacterRepository::parseCharacter(json);

    EXPECT_EQ(character.initial(), "M");
}
