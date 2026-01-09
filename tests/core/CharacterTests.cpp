#include <gtest/gtest.h>
#include "core/entities/Character.h"

using namespace rickmorty::core;

// Tests for Character::initial()
TEST(CharacterTest, Initial_ReturnsUppercaseFirstLetter) {
    Character c{1, "Rick Sanchez", "", "", "", "", "", "", "", 0};
    EXPECT_EQ(c.initial(), "R");
}

TEST(CharacterTest, Initial_HandlesEmptyName) {
    Character c{1, "", "", "", "", "", "", "", "", 0};
    EXPECT_EQ(c.initial(), "");
}

TEST(CharacterTest, Initial_HandlesLowercaseName) {
    Character c{1, "morty", "", "", "", "", "", "", "", 0};
    EXPECT_EQ(c.initial(), "M");
}

TEST(CharacterTest, Initial_HandlesSingleCharacter) {
    Character c{1, "X", "", "", "", "", "", "", "", 0};
    EXPECT_EQ(c.initial(), "X");
}

TEST(CharacterTest, Initial_HandlesSpecialCharacters) {
    Character c{1, "123abc", "", "", "", "", "", "", "", 0};
    EXPECT_EQ(c.initial(), "1");
}

// Tests for Character::isValid()
TEST(CharacterTest, IsValid_ReturnsTrueForPositiveId) {
    Character c{1, "Rick", "", "", "", "", "", "", "", 0};
    EXPECT_TRUE(c.isValid());
}

TEST(CharacterTest, IsValid_ReturnsTrueForLargeId) {
    Character c{999999, "Rick", "", "", "", "", "", "", "", 0};
    EXPECT_TRUE(c.isValid());
}

TEST(CharacterTest, IsValid_ReturnsFalseForZeroId) {
    Character c{0, "Rick", "", "", "", "", "", "", "", 0};
    EXPECT_FALSE(c.isValid());
}

TEST(CharacterTest, IsValid_ReturnsFalseForNegativeId) {
    Character c{-1, "Rick", "", "", "", "", "", "", "", 0};
    EXPECT_FALSE(c.isValid());
}

// Tests for default constructed Character
TEST(CharacterTest, DefaultConstruction_HasZeroId) {
    Character c;
    EXPECT_EQ(c.id, 0);
    EXPECT_FALSE(c.isValid());
}

TEST(CharacterTest, DefaultConstruction_HasEmptyStrings) {
    Character c;
    EXPECT_TRUE(c.name.empty());
    EXPECT_TRUE(c.status.empty());
    EXPECT_TRUE(c.species.empty());
}
