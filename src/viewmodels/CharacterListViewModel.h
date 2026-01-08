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

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        StatusRole,
        SpeciesRole,
        ImageRole,
        InitialRole
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

    // Actions
    void loadCharacters(const std::vector<int>& ids);
    Q_INVOKABLE void clear();

signals:
    void stateChanged();
    void errorMessageChanged();
    void countChanged();

private:
    core::ICharacterRepository* m_repository;
    std::vector<core::Character> m_characters;
    ViewState::State m_state = ViewState::Idle;
    QString m_errorMessage;

    void setState(ViewState::State state);
    void setErrorMessage(const QString& message);
};

}  // namespace rickmorty::viewmodels

#endif  // RICKMORTY_VIEWMODELS_CHARACTER_LIST_VIEW_MODEL_H
