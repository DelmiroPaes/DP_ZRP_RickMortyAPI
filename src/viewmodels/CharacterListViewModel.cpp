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
    case ImageRole:
        return QString::fromStdString(character.imageUrl);
    case InitialRole:
        return QString::fromStdString(character.initial());
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
        {ImageRole, "image"},
        {InitialRole, "initial"}
    };
}

void CharacterListViewModel::loadCharacters(const std::vector<int>& ids)
{
    if (ids.empty()) {
        clear();
        return;
    }

    setState(ViewState::Loading);

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
    if (m_characters.empty()) {
        return;
    }

    beginResetModel();
    m_characters.clear();
    endResetModel();
    emit countChanged();
    setState(ViewState::Idle);
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
