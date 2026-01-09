#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QCoreApplication>
#include <QSignalSpy>
#include "viewmodels/CharacterListViewModel.h"
#include "viewmodels/ViewState.h"
#include "../mocks/MockCharacterRepository.h"

using namespace rickmorty::viewmodels;
using namespace rickmorty::core;
using namespace rickmorty::tests;
using ::testing::_;
using ::testing::Invoke;

class CharacterListViewModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (!QCoreApplication::instance()) {
            int argc = 0;
            app = new QCoreApplication(argc, nullptr);
        }
    }

    void TearDown() override {
        delete app;
        app = nullptr;
    }

    QCoreApplication* app = nullptr;
};

// Initial state tests
TEST_F(CharacterListViewModelTest, InitialState_IsIdle) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);

    EXPECT_EQ(vm.state(), static_cast<int>(ViewState::Idle));
}

TEST_F(CharacterListViewModelTest, InitialState_RowCountIsZero) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);

    EXPECT_EQ(vm.rowCount(), 0);
}

TEST_F(CharacterListViewModelTest, InitialState_SelectedIndexIsNegative) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);

    EXPECT_EQ(vm.selectedIndex(), -1);
}

TEST_F(CharacterListViewModelTest, InitialState_HasNoSelection) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);

    EXPECT_FALSE(vm.hasSelection());
}

TEST_F(CharacterListViewModelTest, InitialState_ErrorMessageIsEmpty) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);

    EXPECT_TRUE(vm.errorMessage().isEmpty());
}

// loadCharacters tests
TEST_F(CharacterListViewModelTest, LoadCharacters_ChangesStateToLoading) {
    MockCharacterRepository mockRepo;
    EXPECT_CALL(mockRepo, fetchCharacters(_, _)).Times(1);

    CharacterListViewModel vm(&mockRepo);
    vm.loadCharacters({1, 2, 3});

    EXPECT_EQ(vm.state(), static_cast<int>(ViewState::Loading));
}

TEST_F(CharacterListViewModelTest, LoadCharacters_CallsRepositoryWithIds) {
    MockCharacterRepository mockRepo;
    std::vector<int> capturedIds;

    EXPECT_CALL(mockRepo, fetchCharacters(_, _))
        .WillOnce(Invoke([&capturedIds](const std::vector<int>& ids, CharacterCallback) {
            capturedIds = ids;
        }));

    CharacterListViewModel vm(&mockRepo);
    vm.loadCharacters({1, 42, 183});

    ASSERT_EQ(capturedIds.size(), 3);
    EXPECT_EQ(capturedIds[0], 1);
    EXPECT_EQ(capturedIds[1], 42);
    EXPECT_EQ(capturedIds[2], 183);
}

TEST_F(CharacterListViewModelTest, LoadCharacters_EmptyIds_CallsClearInstead) {
    MockCharacterRepository mockRepo;
    // When empty IDs are passed, the VM calls clear() instead of fetching
    EXPECT_CALL(mockRepo, fetchCharacters(_, _)).Times(0);

    CharacterListViewModel vm(&mockRepo);
    vm.loadCharacters({});

    // State should remain Idle (clear was called on empty list)
    EXPECT_EQ(vm.state(), static_cast<int>(ViewState::Idle));
}

// clear tests
TEST_F(CharacterListViewModelTest, Clear_ResetsRowCount) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);

    vm.clear();

    EXPECT_EQ(vm.rowCount(), 0);
}

TEST_F(CharacterListViewModelTest, Clear_OnEmptyList_KeepsCurrentState) {
    MockCharacterRepository mockRepo;
    EXPECT_CALL(mockRepo, fetchCharacters(_, _)).Times(1);

    CharacterListViewModel vm(&mockRepo);
    vm.loadCharacters({1});  // Changes state to Loading

    // clear() on empty list returns early without changing state
    vm.clear();

    // State remains Loading because there were no characters to clear
    EXPECT_EQ(vm.state(), static_cast<int>(ViewState::Loading));
}

TEST_F(CharacterListViewModelTest, Clear_ResetsSelectedIndex) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);

    vm.setSelectedIndex(5);
    vm.clear();

    EXPECT_EQ(vm.selectedIndex(), -1);
}

// setSelectedIndex tests
TEST_F(CharacterListViewModelTest, SetSelectedIndex_UpdatesValue) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);

    vm.setSelectedIndex(3);

    EXPECT_EQ(vm.selectedIndex(), 3);
}

TEST_F(CharacterListViewModelTest, SetSelectedIndex_EmitsSignal) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);
    QSignalSpy spy(&vm, &CharacterListViewModel::selectedIndexChanged);

    vm.setSelectedIndex(3);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(CharacterListViewModelTest, SetSelectedIndex_SameValue_DoesNotEmitSignal) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);

    vm.setSelectedIndex(3);

    QSignalSpy spy(&vm, &CharacterListViewModel::selectedIndexChanged);
    vm.setSelectedIndex(3);

    EXPECT_EQ(spy.count(), 0);
}

// clearSelection tests
TEST_F(CharacterListViewModelTest, ClearSelection_SetsIndexToNegativeOne) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);

    vm.setSelectedIndex(5);
    vm.clearSelection();

    EXPECT_EQ(vm.selectedIndex(), -1);
}

TEST_F(CharacterListViewModelTest, ClearSelection_EmitsSignal) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);
    vm.setSelectedIndex(5);

    QSignalSpy spy(&vm, &CharacterListViewModel::selectedIndexChanged);
    vm.clearSelection();

    EXPECT_EQ(spy.count(), 1);
}

// roleNames tests
TEST_F(CharacterListViewModelTest, RoleNames_ContainsExpectedRoles) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);

    auto roles = vm.roleNames();

    EXPECT_TRUE(roles.contains(CharacterListViewModel::IdRole));
    EXPECT_TRUE(roles.contains(CharacterListViewModel::NameRole));
    EXPECT_TRUE(roles.contains(CharacterListViewModel::StatusRole));
    EXPECT_TRUE(roles.contains(CharacterListViewModel::SpeciesRole));
    EXPECT_TRUE(roles.contains(CharacterListViewModel::InitialRole));
}

TEST_F(CharacterListViewModelTest, RoleNames_HasCorrectNames) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);

    auto roles = vm.roleNames();

    EXPECT_EQ(roles[CharacterListViewModel::NameRole], "name");
    EXPECT_EQ(roles[CharacterListViewModel::StatusRole], "status");
    EXPECT_EQ(roles[CharacterListViewModel::InitialRole], "initial");
}

// Selected character properties (when no selection)
TEST_F(CharacterListViewModelTest, SelectedProperties_NoSelection_ReturnEmpty) {
    MockCharacterRepository mockRepo;
    CharacterListViewModel vm(&mockRepo);

    EXPECT_TRUE(vm.selectedName().isEmpty());
    EXPECT_TRUE(vm.selectedStatus().isEmpty());
    EXPECT_TRUE(vm.selectedSpecies().isEmpty());
    EXPECT_EQ(vm.selectedEpisodeCount(), 0);
}
