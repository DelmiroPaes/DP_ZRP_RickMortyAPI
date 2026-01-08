#include "EpisodeDetailViewModel.h"

namespace rickmorty::viewmodels {

EpisodeDetailViewModel::EpisodeDetailViewModel(QObject* parent)
    : QObject(parent)
{
}

void EpisodeDetailViewModel::setEpisode(const core::Episode& episode)
{
    m_episode = episode;
    emit episodeChanged();
}

void EpisodeDetailViewModel::clear()
{
    m_episode = {};
    emit episodeChanged();
}

}  // namespace rickmorty::viewmodels
