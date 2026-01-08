#include "CharacterModel.h"
#include <QJsonObject>
#include <algorithm>

CharacterModel::CharacterModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int CharacterModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_characters.count();
}

QVariant CharacterModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_characters.count())
        return QVariant();

    const Character &character = m_characters.at(index.row());

    switch (role) {
    case IdRole:
        return character.id;
    case NameRole:
        return character.name;
    case StatusRole:
        return character.status;
    case SpeciesRole:
        return character.species;
    case ImageRole:
        return character.image;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> CharacterModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[StatusRole] = "status";
    roles[SpeciesRole] = "species";
    roles[ImageRole] = "image";
    return roles;
}

void CharacterModel::setCharacters(const QJsonArray &characters)
{
    beginResetModel();
    m_characters.clear();

    for (const QJsonValue &value : characters) {
        QJsonObject obj = value.toObject();
        Character character;
        character.id = obj["id"].toInt();
        character.name = obj["name"].toString();
        character.status = obj["status"].toString();
        character.species = obj["species"].toString();
        character.image = obj["image"].toString();

        m_characters.append(character);
    }

    std::sort(m_characters.begin(), m_characters.end(),
              [](const Character &a, const Character &b) {
                  return a.name.toLower() < b.name.toLower();
              });

    endResetModel();
    emit countChanged();
}

void CharacterModel::clear()
{
    beginResetModel();
    m_characters.clear();
    endResetModel();
    emit countChanged();
}
