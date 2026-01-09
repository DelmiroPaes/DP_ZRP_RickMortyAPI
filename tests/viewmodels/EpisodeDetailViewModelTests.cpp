#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QSignalSpy>
#include "viewmodels/EpisodeDetailViewModel.h"
#include "core/entities/Episode.h"

using namespace rickmorty::viewmodels;
using namespace rickmorty::core;

class EpisodeDetailViewModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Qt requires QCoreApplication for signal/slot mechanism
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
TEST_F(EpisodeDetailViewModelTest, InitialState_HasNoEpisode) {
    EpisodeDetailViewModel vm;
    EXPECT_FALSE(vm.hasEpisode());
}

TEST_F(EpisodeDetailViewModelTest, InitialState_EpisodeIdIsZero) {
    EpisodeDetailViewModel vm;
    EXPECT_EQ(vm.episodeId(), 0);
}

TEST_F(EpisodeDetailViewModelTest, InitialState_NameIsEmpty) {
    EpisodeDetailViewModel vm;
    EXPECT_TRUE(vm.name().isEmpty());
}

// setEpisode tests
TEST_F(EpisodeDetailViewModelTest, SetEpisode_UpdatesProperties) {
    EpisodeDetailViewModel vm;
    Episode ep{1, "Pilot", "December 2, 2013", "S01E01", 1, 1, {1, 2}, ""};

    vm.setEpisode(ep);

    EXPECT_EQ(vm.episodeId(), 1);
    EXPECT_EQ(vm.name(), "Pilot");
    EXPECT_EQ(vm.airDate(), "December 2, 2013");
    EXPECT_EQ(vm.episodeCode(), "S01E01");
    EXPECT_TRUE(vm.hasEpisode());
}

TEST_F(EpisodeDetailViewModelTest, SetEpisode_FormattedSeasonIsCorrect) {
    EpisodeDetailViewModel vm;
    Episode ep{1, "Pilot", "", "S01E01", 1, 1, {}, ""};

    vm.setEpisode(ep);

    EXPECT_EQ(vm.formattedSeason(), "Season 1, Episode 1");
}

TEST_F(EpisodeDetailViewModelTest, SetEpisode_EmitsSignal) {
    EpisodeDetailViewModel vm;
    QSignalSpy spy(&vm, &EpisodeDetailViewModel::episodeChanged);

    Episode ep{1, "Pilot", "", "S01E01", 1, 1, {}, ""};
    vm.setEpisode(ep);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(EpisodeDetailViewModelTest, SetEpisode_MultipleCalls_EmitsMultipleSignals) {
    EpisodeDetailViewModel vm;
    QSignalSpy spy(&vm, &EpisodeDetailViewModel::episodeChanged);

    Episode ep1{1, "Pilot", "", "", 1, 1, {}, ""};
    Episode ep2{2, "Lawnmower Dog", "", "", 1, 2, {}, ""};

    vm.setEpisode(ep1);
    vm.setEpisode(ep2);

    EXPECT_EQ(spy.count(), 2);
    EXPECT_EQ(vm.episodeId(), 2);
    EXPECT_EQ(vm.name(), "Lawnmower Dog");
}

// clear tests
TEST_F(EpisodeDetailViewModelTest, Clear_ResetsToInitialState) {
    EpisodeDetailViewModel vm;
    Episode ep{1, "Pilot", "", "", 1, 1, {}, ""};
    vm.setEpisode(ep);

    vm.clear();

    EXPECT_EQ(vm.episodeId(), 0);
    EXPECT_FALSE(vm.hasEpisode());
    EXPECT_TRUE(vm.name().isEmpty());
}

TEST_F(EpisodeDetailViewModelTest, Clear_EmitsSignal) {
    EpisodeDetailViewModel vm;
    Episode ep{1, "Pilot", "", "", 1, 1, {}, ""};
    vm.setEpisode(ep);

    QSignalSpy spy(&vm, &EpisodeDetailViewModel::episodeChanged);
    vm.clear();

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(EpisodeDetailViewModelTest, Clear_OnEmptyViewModel_StillEmitsSignal) {
    EpisodeDetailViewModel vm;
    QSignalSpy spy(&vm, &EpisodeDetailViewModel::episodeChanged);

    vm.clear();

    EXPECT_EQ(spy.count(), 1);
}
