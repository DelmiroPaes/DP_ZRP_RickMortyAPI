#include "EpisodeListViewModel.h"
#include <QMetaObject>
#include <random>

namespace rickmorty::viewmodels {

namespace {
// Get two random character indices for an episode (deterministic based on episode ID)
std::pair<size_t, size_t> getRandomCharacterIndices(int episodeId, size_t charCount) {
    if (charCount == 0) return {0, 0};
    if (charCount == 1) return {0, 0};

    // Use episode ID as seed for reproducible randomness
    std::mt19937 gen(static_cast<unsigned>(episodeId * 12345));
    std::uniform_int_distribution<size_t> dist(0, charCount - 1);

    size_t idx1 = dist(gen);
    size_t idx2 = dist(gen);

    // Ensure idx2 is different from idx1 if possible
    if (charCount > 1) {
        int attempts = 0;
        while (idx2 == idx1 && attempts < 10) {
            idx2 = dist(gen);
            attempts++;
        }
    }

    return {idx1, idx2};
}
}

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

    static constexpr const char* IMAGE_URL_PATTERN = "https://rickandmortyapi.com/api/character/avatar/%1.jpeg";

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
    case ThumbnailUrl1Role:
    case ThumbnailUrl2Role:
        if (!episode.characterIds.empty()) {
            auto [idx1, idx2] = getRandomCharacterIndices(episode.id, episode.characterIds.size());
            size_t idx = (role == ThumbnailUrl1Role) ? idx1 : idx2;
            return QString(IMAGE_URL_PATTERN).arg(episode.characterIds[idx]);
        }
        return QString();
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
        {ImageRole, "image"},
        {ThumbnailUrl1Role, "thumbnailUrl1"},
        {ThumbnailUrl2Role, "thumbnailUrl2"}
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

std::vector<std::string> EpisodeListViewModel::getFirstCharacterImageUrls(int episodeId, int count) const
{
    static constexpr const char* IMAGE_URL_PATTERN = "https://rickandmortyapi.com/api/character/avatar/%1.jpeg";

    std::vector<std::string> urls;

    // Find episode by ID
    for (const auto& episode : m_episodes) {
        if (episode.id == episodeId) {
            const auto& charIds = episode.characterIds;
            const int numChars = std::min(count, static_cast<int>(charIds.size()));

            for (int i = 0; i < numChars; ++i) {
                urls.push_back(QString(IMAGE_URL_PATTERN).arg(charIds[static_cast<size_t>(i)]).toStdString());
            }
            break;
        }
    }

    return urls;
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
