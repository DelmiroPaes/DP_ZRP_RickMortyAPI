#include "CharacterListViewModel.h"
#include <QMetaObject>

namespace rickmorty::viewmodels {

CharacterListViewModel::CharacterListViewModel(core::ICharacterRepository* repository,
                                               QObject* parent)
    : QAbstractListModel(parent)
    , m_repository(repository)
{
}

int CharacterListViewModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(m_characters.size());
}

QVariant CharacterListViewModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(m_characters.size())) {
        return {};
    }

    const auto& character = m_characters[static_cast<size_t>(index.row())];

    switch (role) {
    case IdRole:
        return character.id;
    case NameRole:
        return QString::fromStdString(character.name);
    case StatusRole:
        return QString::fromStdString(character.status);
    case SpeciesRole:
        return QString::fromStdString(character.species);
    case TypeRole:
        return QString::fromStdString(character.type);
    case GenderRole:
        return QString::fromStdString(character.gender);
    case OriginRole:
        return QString::fromStdString(character.origin);
    case LocationRole:
        return QString::fromStdString(character.location);
    case ImageRole:
        return QString::fromStdString(character.imageUrl);
    case InitialRole:
        return QString::fromStdString(character.initial());
    case EpisodeCountRole:
        return character.episodeCount;
    default:
        return {};
    }
}

QHash<int, QByteArray> CharacterListViewModel::roleNames() const
{
    return {
        {IdRole, "id"},
        {NameRole, "name"},
        {StatusRole, "status"},
        {SpeciesRole, "species"},
        {TypeRole, "type"},
        {GenderRole, "gender"},
        {OriginRole, "origin"},
        {LocationRole, "location"},
        {ImageRole, "image"},
        {InitialRole, "initial"},
        {EpisodeCountRole, "episodeCount"}
    };
}

void CharacterListViewModel::loadCharacters(const std::vector<int>& ids)
{
    if (ids.empty()) {
        clear();
        return;
    }

    setState(ViewState::Loading);
    clearSelection();

    m_repository->fetchCharacters(ids, [this](core::CharacterResult result) {
        // Ensure UI updates happen on the main thread
        QMetaObject::invokeMethod(this, [this, result = std::move(result)]() mutable {
            if (auto* characters = std::get_if<std::vector<core::Character>>(&result)) {
                beginResetModel();
                m_characters = std::move(*characters);
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

void CharacterListViewModel::clear()
{
    clearSelection();

    if (m_characters.empty()) {
        return;
    }

    beginResetModel();
    m_characters.clear();
    endResetModel();
    emit countChanged();
    setState(ViewState::Idle);
}

void CharacterListViewModel::setSelectedIndex(int index)
{
    if (m_selectedIndex == index) {
        return;
    }
    m_selectedIndex = index;
    emit selectedIndexChanged();
}

void CharacterListViewModel::clearSelection()
{
    setSelectedIndex(-1);
}

const core::Character* CharacterListViewModel::selectedCharacter() const
{
    if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_characters.size())) {
        return &m_characters[static_cast<size_t>(m_selectedIndex)];
    }
    return nullptr;
}

QString CharacterListViewModel::selectedName() const
{
    const auto* ch = selectedCharacter();
    return ch ? QString::fromStdString(ch->name) : QString();
}

QString CharacterListViewModel::selectedStatus() const
{
    const auto* ch = selectedCharacter();
    return ch ? QString::fromStdString(ch->status) : QString();
}

QString CharacterListViewModel::selectedSpecies() const
{
    const auto* ch = selectedCharacter();
    return ch ? QString::fromStdString(ch->species) : QString();
}

QString CharacterListViewModel::selectedType() const
{
    const auto* ch = selectedCharacter();
    return ch ? QString::fromStdString(ch->type) : QString();
}

QString CharacterListViewModel::selectedGender() const
{
    const auto* ch = selectedCharacter();
    return ch ? QString::fromStdString(ch->gender) : QString();
}

QString CharacterListViewModel::selectedOrigin() const
{
    const auto* ch = selectedCharacter();
    return ch ? QString::fromStdString(ch->origin) : QString();
}

QString CharacterListViewModel::selectedLocation() const
{
    const auto* ch = selectedCharacter();
    return ch ? QString::fromStdString(ch->location) : QString();
}

QString CharacterListViewModel::selectedImageUrl() const
{
    const auto* ch = selectedCharacter();
    return ch ? QString::fromStdString(ch->imageUrl) : QString();
}

int CharacterListViewModel::selectedEpisodeCount() const
{
    const auto* ch = selectedCharacter();
    return ch ? ch->episodeCount : 0;
}

void CharacterListViewModel::setState(ViewState::State state)
{
    if (m_state == state) {
        return;
    }
    m_state = state;
    emit stateChanged();
}

void CharacterListViewModel::setErrorMessage(const QString& message)
{
    if (m_errorMessage == message) {
        return;
    }
    m_errorMessage = message;
    emit errorMessageChanged();
}

}  // namespace rickmorty::viewmodels
