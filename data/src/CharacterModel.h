#ifndef CHARACTERMODEL_H
#define CHARACTERMODEL_H

#include <QAbstractListModel>
#include <QJsonArray>

struct Character
{
    int id;
    QString name;
    QString status;
    QString species;
    QString image;
};

class CharacterModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        StatusRole,
        SpeciesRole,
        ImageRole
    };

    explicit CharacterModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setCharacters(const QJsonArray &characters);
    void clear();

signals:
    void countChanged();

private:
    QList<Character> m_characters;
};

#endif // CHARACTERMODEL_H
