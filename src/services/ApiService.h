#ifndef RICKMORTY_SERVICES_API_SERVICE_H
#define RICKMORTY_SERVICES_API_SERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <functional>
#include <variant>

class QJsonArray;

namespace rickmorty::services {

class ApiService : public QObject {
    Q_OBJECT

public:
    using JsonResult = std::variant<QJsonArray, QString>;
    using JsonCallback = std::function<void(JsonResult)>;

    explicit ApiService(QObject* parent = nullptr);

    void get(const QString& endpoint, JsonCallback callback);

private:
    QNetworkAccessManager m_networkManager;
    static constexpr const char* BASE_URL = "https://rickandmortyapi.com/api";
};

}  // namespace rickmorty::services

#endif  // RICKMORTY_SERVICES_API_SERVICE_H
