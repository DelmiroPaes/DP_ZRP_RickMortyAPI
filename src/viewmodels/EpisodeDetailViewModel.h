#ifndef RICKMORTY_VIEWMODELS_EPISODE_DETAIL_VIEW_MODEL_H
#define RICKMORTY_VIEWMODELS_EPISODE_DETAIL_VIEW_MODEL_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include "core/entities/Episode.h"

namespace rickmorty::viewmodels {

class EpisodeDetailViewModel : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int episodeId READ episodeId NOTIFY episodeChanged FINAL)
    Q_PROPERTY(QString name READ name NOTIFY episodeChanged FINAL)
    Q_PROPERTY(QString airDate READ airDate NOTIFY episodeChanged FINAL)
    Q_PROPERTY(QString episodeCode READ episodeCode NOTIFY episodeChanged FINAL)
    Q_PROPERTY(QString formattedSeason READ formattedSeason NOTIFY episodeChanged FINAL)
    Q_PROPERTY(QString imageUrl READ imageUrl NOTIFY episodeChanged FINAL)
    Q_PROPERTY(bool hasEpisode READ hasEpisode NOTIFY episodeChanged FINAL)

public:
    explicit EpisodeDetailViewModel(QObject* parent = nullptr);

    // Property accessors
    [[nodiscard]] int episodeId() const noexcept { return m_episode.id; }
    [[nodiscard]] QString name() const { return QString::fromStdString(m_episode.name); }
    [[nodiscard]] QString airDate() const { return QString::fromStdString(m_episode.airDate); }
    [[nodiscard]] QString episodeCode() const { return QString::fromStdString(m_episode.episodeCode); }
    [[nodiscard]] QString formattedSeason() const { return QString::fromStdString(m_episode.formattedSeason()); }
    [[nodiscard]] QString imageUrl() const { return QString::fromStdString(m_episode.imageUrl); }
    [[nodiscard]] bool hasEpisode() const noexcept { return m_episode.isValid(); }

    // Actions
    void setEpisode(const core::Episode& episode);
    Q_INVOKABLE void clear();

signals:
    void episodeChanged();

private:
    core::Episode m_episode;
};

}  // namespace rickmorty::viewmodels

#endif  // RICKMORTY_VIEWMODELS_EPISODE_DETAIL_VIEW_MODEL_H
