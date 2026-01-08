#ifndef RICKMORTY_VIEWMODELS_CHARACTER_LIST_VIEW_MODEL_H
#define RICKMORTY_VIEWMODELS_CHARACTER_LIST_VIEW_MODEL_H

#include <QAbstractListModel>
#include <QtQml/qqmlregistration.h>
#include "ViewState.h"
#include "core/entities/Character.h"
#include "core/repositories/ICharacterRepository.h"

namespace rickmorty::viewmodels {

class CharacterListViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int state READ state NOTIFY stateChanged FINAL)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged FINAL)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged FINAL)
    Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY selectedIndexChanged FINAL)
    Q_PROPERTY(bool hasSelection READ hasSelection NOTIFY selectedIndexChanged FINAL)

    // Selected character properties
    Q_PROPERTY(QString selectedName READ selectedName NOTIFY selectedIndexChanged FINAL)
    Q_PROPERTY(QString selectedStatus READ selectedStatus NOTIFY selectedIndexChanged FINAL)
    Q_PROPERTY(QString selectedSpecies READ selectedSpecies NOTIFY selectedIndexChanged FINAL)
    Q_PROPERTY(QString selectedType READ selectedType NOTIFY selectedIndexChanged FINAL)
    Q_PROPERTY(QString selectedGender READ selectedGender NOTIFY selectedIndexChanged FINAL)
    Q_PROPERTY(QString selectedOrigin READ selectedOrigin NOTIFY selectedIndexChanged FINAL)
    Q_PROPERTY(QString selectedLocation READ selectedLocation NOTIFY selectedIndexChanged FINAL)
    Q_PROPERTY(QString selectedImageUrl READ selectedImageUrl NOTIFY selectedIndexChanged FINAL)
    Q_PROPERTY(int selectedEpisodeCount READ selectedEpisodeCount NOTIFY selectedIndexChanged FINAL)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        StatusRole,
        SpeciesRole,
        TypeRole,
        GenderRole,
        OriginRole,
        LocationRole,
        ImageRole,
        InitialRole,
        EpisodeCountRole
    };
    Q_ENUM(Roles)

    explicit CharacterListViewModel(core::ICharacterRepository* repository,
                                    QObject* parent = nullptr);

    // QAbstractListModel interface
    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    // Property accessors
    [[nodiscard]] int state() const noexcept { return static_cast<int>(m_state); }
    [[nodiscard]] QString errorMessage() const noexcept { return m_errorMessage; }
    [[nodiscard]] int selectedIndex() const noexcept { return m_selectedIndex; }
    [[nodiscard]] bool hasSelection() const noexcept { return m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_characters.size()); }

    // Selected character accessors
    [[nodiscard]] QString selectedName() const;
    [[nodiscard]] QString selectedStatus() const;
    [[nodiscard]] QString selectedSpecies() const;
    [[nodiscard]] QString selectedType() const;
    [[nodiscard]] QString selectedGender() const;
    [[nodiscard]] QString selectedOrigin() const;
    [[nodiscard]] QString selectedLocation() const;
    [[nodiscard]] QString selectedImageUrl() const;
    [[nodiscard]] int selectedEpisodeCount() const;

    // Actions
    void loadCharacters(const std::vector<int>& ids);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void setSelectedIndex(int index);
    Q_INVOKABLE void clearSelection();

signals:
    void stateChanged();
    void errorMessageChanged();
    void countChanged();
    void selectedIndexChanged();

private:
    core::ICharacterRepository* m_repository;
    std::vector<core::Character> m_characters;
    ViewState::State m_state = ViewState::Idle;
    QString m_errorMessage;
    int m_selectedIndex = -1;

    void setState(ViewState::State state);
    void setErrorMessage(const QString& message);
    [[nodiscard]] const core::Character* selectedCharacter() const;
};

}  // namespace rickmorty::viewmodels

#endif  // RICKMORTY_VIEWMODELS_CHARACTER_LIST_VIEW_MODEL_H
