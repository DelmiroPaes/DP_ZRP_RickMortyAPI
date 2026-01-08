#include "CharacterRepository.h"
#include "ApiService.h"
#include <QJsonObject>
#include <QJsonArray>
#include <algorithm>

namespace rickmorty::services {

CharacterRepository::CharacterRepository(ApiService* apiService, QObject* parent)
    : QObject(parent)
    , m_apiService(apiService)
{
}

void CharacterRepository::fetchCharacters(const std::vector<int>& ids,
                                          core::CharacterCallback callback)
{
    if (ids.empty()) {
        callback(std::vector<core::Character>{});
        return;
    }

    QString idList;
    for (size_t i = 0; i < ids.size(); ++i) {
        if (i > 0) {
            idList += ',';
        }
        idList += QString::number(ids[i]);
    }

    m_apiService->get("/character/" + idList, [callback](ApiService::JsonResult result) {
        if (auto* error = std::get_if<QString>(&result)) {
            callback(error->toStdString());
            return;
        }

        const auto& jsonArray = std::get<QJsonArray>(result);
        std::vector<core::Character> characters;
        characters.reserve(static_cast<size_t>(jsonArray.size()));

        for (const auto& value : jsonArray) {
            characters.push_back(parseCharacter(value.toObject()));
        }

        // Sort alphabetically by name (case-insensitive)
        std::sort(characters.begin(), characters.end(),
                  [](const auto& a, const auto& b) {
                      return std::lexicographical_compare(
                          a.name.begin(), a.name.end(),
                          b.name.begin(), b.name.end(),
                          [](char ca, char cb) {
                              return std::tolower(static_cast<unsigned char>(ca)) <
                                     std::tolower(static_cast<unsigned char>(cb));
                          });
                  });

        callback(std::move(characters));
    });
}

core::Character CharacterRepository::parseCharacter(const QJsonObject& json)
{
    core::Character character;
    character.id = json["id"].toInt();
    character.name = json["name"].toString().toStdString();
    character.status = json["status"].toString().toStdString();
    character.species = json["species"].toString().toStdString();
    character.imageUrl = json["image"].toString().toStdString();
    return character;
}

}  // namespace rickmorty::services
