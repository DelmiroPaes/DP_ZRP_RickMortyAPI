#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QCoreApplication>
#include <QSignalSpy>
#include "viewmodels/EpisodeListViewModel.h"
#include "viewmodels/ViewState.h"
#include "../mocks/MockEpisodeRepository.h"

using namespace rickmorty::viewmodels;
using namespace rickmorty::core;
using namespace rickmorty::tests;
using ::testing::_;
using ::testing::Invoke;
using ::testing::SaveArg;

class EpisodeListViewModelTest : public ::testing::Test {
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
TEST_F(EpisodeListViewModelTest, InitialState_IsIdle) {
    MockEpisodeRepository mockRepo;
    EpisodeListViewModel vm(&mockRepo);

    EXPECT_EQ(vm.state(), static_cast<int>(ViewState::Idle));
}

TEST_F(EpisodeListViewModelTest, InitialState_RowCountIsZero) {
    MockEpisodeRepository mockRepo;
    EpisodeListViewModel vm(&mockRepo);

    EXPECT_EQ(vm.rowCount(), 0);
}

TEST_F(EpisodeListViewModelTest, InitialState_SelectedIndexIsNegative) {
    MockEpisodeRepository mockRepo;
    EpisodeListViewModel vm(&mockRepo);

    EXPECT_EQ(vm.selectedIndex(), -1);
}

TEST_F(EpisodeListViewModelTest, InitialState_ErrorMessageIsEmpty) {
    MockEpisodeRepository mockRepo;
    EpisodeListViewModel vm(&mockRepo);

    EXPECT_TRUE(vm.errorMessage().isEmpty());
}

// loadEpisodes tests
TEST_F(EpisodeListViewModelTest, LoadEpisodes_ChangesStateToLoading) {
    MockEpisodeRepository mockRepo;
    EXPECT_CALL(mockRepo, fetchAllEpisodes(_)).Times(1);

    EpisodeListViewModel vm(&mockRepo);
    vm.loadEpisodes();

    EXPECT_EQ(vm.state(), static_cast<int>(ViewState::Loading));
}

TEST_F(EpisodeListViewModelTest, LoadEpisodes_CallsRepository) {
    MockEpisodeRepository mockRepo;
    EXPECT_CALL(mockRepo, fetchAllEpisodes(_)).Times(1);

    EpisodeListViewModel vm(&mockRepo);
    vm.loadEpisodes();
}

TEST_F(EpisodeListViewModelTest, LoadEpisodes_EmitsStateChangedSignal) {
    MockEpisodeRepository mockRepo;
    EXPECT_CALL(mockRepo, fetchAllEpisodes(_)).Times(1);

    EpisodeListViewModel vm(&mockRepo);
    QSignalSpy spy(&vm, &EpisodeListViewModel::stateChanged);

    vm.loadEpisodes();

    EXPECT_GE(spy.count(), 1);
}

// setSelectedIndex tests
TEST_F(EpisodeListViewModelTest, SetSelectedIndex_UpdatesSelectedIndex) {
    MockEpisodeRepository mockRepo;
    EpisodeListViewModel vm(&mockRepo);

    vm.setSelectedIndex(5);

    EXPECT_EQ(vm.selectedIndex(), 5);
}

TEST_F(EpisodeListViewModelTest, SetSelectedIndex_EmitsSignal) {
    MockEpisodeRepository mockRepo;
    EpisodeListViewModel vm(&mockRepo);
    QSignalSpy spy(&vm, &EpisodeListViewModel::selectedIndexChanged);

    vm.setSelectedIndex(3);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(EpisodeListViewModelTest, SetSelectedIndex_SameValue_DoesNotEmitSignal) {
    MockEpisodeRepository mockRepo;
    EpisodeListViewModel vm(&mockRepo);

    vm.setSelectedIndex(3);

    QSignalSpy spy(&vm, &EpisodeListViewModel::selectedIndexChanged);
    vm.setSelectedIndex(3);  // Same value

    EXPECT_EQ(spy.count(), 0);
}

// roleNames tests
TEST_F(EpisodeListViewModelTest, RoleNames_ContainsExpectedRoles) {
    MockEpisodeRepository mockRepo;
    EpisodeListViewModel vm(&mockRepo);

    auto roles = vm.roleNames();

    EXPECT_TRUE(roles.contains(EpisodeListViewModel::IdRole));
    EXPECT_TRUE(roles.contains(EpisodeListViewModel::NameRole));
    EXPECT_TRUE(roles.contains(EpisodeListViewModel::AirDateRole));
    EXPECT_TRUE(roles.contains(EpisodeListViewModel::EpisodeCodeRole));
    EXPECT_TRUE(roles.contains(EpisodeListViewModel::SeasonRole));
    EXPECT_TRUE(roles.contains(EpisodeListViewModel::EpisodeNumberRole));
}

TEST_F(EpisodeListViewModelTest, RoleNames_HasCorrectNames) {
    MockEpisodeRepository mockRepo;
    EpisodeListViewModel vm(&mockRepo);

    auto roles = vm.roleNames();

    EXPECT_EQ(roles[EpisodeListViewModel::NameRole], "name");
    EXPECT_EQ(roles[EpisodeListViewModel::EpisodeCodeRole], "episodeCode");
}

// getEpisode tests
TEST_F(EpisodeListViewModelTest, GetEpisode_InvalidIndex_ReturnsInvalidEpisode) {
    MockEpisodeRepository mockRepo;
    EpisodeListViewModel vm(&mockRepo);

    auto episode = vm.getEpisode(-1);
    EXPECT_FALSE(episode.isValid());

    episode = vm.getEpisode(100);
    EXPECT_FALSE(episode.isValid());
}
