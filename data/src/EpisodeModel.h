#ifndef EPISODEMODEL_H
#define EPISODEMODEL_H

#include <QAbstractListModel>
#include <QJsonArray>

struct Episode
{
    int id;
    QString name;
    QString airDate;
    QString episodeCode;
    int season;
    int episodeNumber;
    QList<int> characterIds;
    QString image;
};

class EpisodeModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        AirDateRole,
        EpisodeCodeRole,
        SeasonRole,
        EpisodeNumberRole,
        CharacterIdsRole,
        ImageRole
    };

    explicit EpisodeModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setEpisodes(const QJsonArray &episodes);
    Episode getEpisode(int index) const;

signals:
    void countChanged();

private:
    QList<Episode> m_episodes;
    void parseEpisodeCode(const QString &code, int &season, int &episode);
    QList<int> parseCharacterIds(const QJsonArray &characters);
};

#endif // EPISODEMODEL_H
