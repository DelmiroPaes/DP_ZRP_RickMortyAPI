#include "EpisodeListViewModel.h"
#include <QMetaObject>

namespace rickmorty::viewmodels {

EpisodeListViewModel::EpisodeListViewModel(core::IEpisodeRepository* repository,
                                           QObject* parent)
    : QAbstractListModel(parent)
    , m_repository(repository)
{
}

int EpisodeListViewModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(m_episodes.size());
}

QVariant EpisodeListViewModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(m_episodes.size())) {
        return {};
    }

    const auto& episode = m_episodes[static_cast<size_t>(index.row())];

    switch (role) {
    case IdRole:
        return episode.id;
    case NameRole:
        return QString::fromStdString(episode.name);
    case AirDateRole:
        return QString::fromStdString(episode.airDate);
    case EpisodeCodeRole:
        return QString::fromStdString(episode.episodeCode);
    case SeasonRole:
        return episode.season;
    case EpisodeNumberRole:
        return episode.episodeNumber;
    case FormattedSeasonRole:
        return QString::fromStdString(episode.formattedSeason());
    case ImageRole:
        return QString::fromStdString(episode.imageUrl);
    default:
        return {};
    }
}

QHash<int, QByteArray> EpisodeListViewModel::roleNames() const
{
    return {
        {IdRole, "id"},
        {NameRole, "name"},
        {AirDateRole, "airDate"},
        {EpisodeCodeRole, "episodeCode"},
        {SeasonRole, "season"},
        {EpisodeNumberRole, "episodeNumber"},
        {FormattedSeasonRole, "formattedSeason"},
        {ImageRole, "image"}
    };
}

void EpisodeListViewModel::loadEpisodes()
{
    setState(ViewState::Loading);

    m_repository->fetchAllEpisodes([this](core::EpisodeResult result) {
        // Ensure UI updates happen on the main thread
        QMetaObject::invokeMethod(this, [this, result = std::move(result)]() mutable {
            if (auto* episodes = std::get_if<std::vector<core::Episode>>(&result)) {
                beginResetModel();
                m_episodes = std::move(*episodes);
                endResetModel();
                emit countChanged();
                setState(ViewState::Success);
            } else {
                setErrorMessage(QString::fromStdString(std::get<std::string>(result)));
                setState(ViewState::Error);
            }
        }, Qt::QueuedConnection);
    });
}

void EpisodeListViewModel::setSelectedIndex(int index)
{
    if (m_selectedIndex == index) {
        return;
    }

    m_selectedIndex = index;
    emit selectedIndexChanged();

    if (index >= 0 && index < static_cast<int>(m_episodes.size())) {
        emit episodeSelected(m_episodes[static_cast<size_t>(index)].characterIds);
    }
}

core::Episode EpisodeListViewModel::getEpisode(int index) const
{
    if (index >= 0 && index < static_cast<int>(m_episodes.size())) {
        return m_episodes[static_cast<size_t>(index)];
    }
    return {};
}

void EpisodeListViewModel::setState(ViewState::State state)
{
    if (m_state == state) {
        return;
    }
    m_state = state;
    emit stateChanged();
}

void EpisodeListViewModel::setErrorMessage(const QString& message)
{
    if (m_errorMessage == message) {
        return;
    }
    m_errorMessage = message;
    emit errorMessageChanged();
}

}  // namespace rickmorty::viewmodels
