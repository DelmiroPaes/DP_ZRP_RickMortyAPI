#ifndef RICKMORTY_SERVICES_CHARACTER_REPOSITORY_H
#define RICKMORTY_SERVICES_CHARACTER_REPOSITORY_H

#include <QObject>
#include "core/repositories/ICharacterRepository.h"

namespace rickmorty::services {

class ApiService;

class CharacterRepository : public QObject, public core::ICharacterRepository {
    Q_OBJECT

public:
    explicit CharacterRepository(ApiService* apiService, QObject* parent = nullptr);

    void fetchCharacters(const std::vector<int>& ids, core::CharacterCallback callback) override;

    // Static parsing method (public for testing)
    static core::Character parseCharacter(const QJsonObject& json);

private:
    ApiService* m_apiService;
};

}  // namespace rickmorty::services

#endif  // RICKMORTY_SERVICES_CHARACTER_REPOSITORY_H
