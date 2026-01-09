#ifndef RICKMORTY_VIEWMODELS_EPISODE_LIST_VIEW_MODEL_H
#define RICKMORTY_VIEWMODELS_EPISODE_LIST_VIEW_MODEL_H

#include <QAbstractListModel>
#include <QtQml/qqmlregistration.h>
#include "ViewState.h"
#include "core/entities/Episode.h"
#include "core/repositories/IEpisodeRepository.h"

namespace rickmorty::viewmodels {

class EpisodeListViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int state READ state NOTIFY stateChanged FINAL)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged FINAL)
    Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY selectedIndexChanged FINAL)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged FINAL)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        AirDateRole,
        EpisodeCodeRole,
        SeasonRole,
        EpisodeNumberRole,
        FormattedSeasonRole,
        ImageRole,
        ThumbnailUrl1Role,
        ThumbnailUrl2Role
    };
    Q_ENUM(Roles)

    explicit EpisodeListViewModel(core::IEpisodeRepository* repository,
                                  QObject* parent = nullptr);

    // QAbstractListModel interface
    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    // Property accessors
    [[nodiscard]] int state() const noexcept { return static_cast<int>(m_state); }
    [[nodiscard]] QString errorMessage() const noexcept { return m_errorMessage; }
    [[nodiscard]] int selectedIndex() const noexcept { return m_selectedIndex; }
    void setSelectedIndex(int index);

    // Actions
    Q_INVOKABLE void loadEpisodes();
    [[nodiscard]] core::Episode getEpisode(int index) const;
    [[nodiscard]] std::vector<std::string> getFirstCharacterImageUrls(int episodeId, int count = 2) const;

signals:
    void stateChanged();
    void errorMessageChanged();
    void selectedIndexChanged();
    void countChanged();
    void episodeSelected(const std::vector<int>& characterIds);

private:
    core::IEpisodeRepository* m_repository;
    std::vector<core::Episode> m_episodes;
    ViewState::State m_state = ViewState::Idle;
    QString m_errorMessage;
    int m_selectedIndex = -1;

    void setState(ViewState::State state);
    void setErrorMessage(const QString& message);
};

}  // namespace rickmorty::viewmodels

#endif  // RICKMORTY_VIEWMODELS_EPISODE_LIST_VIEW_MODEL_H
