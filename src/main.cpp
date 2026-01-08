#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/qqmlextensionplugin.h>

// Services
#include "services/ApiService.h"
#include "services/EpisodeRepository.h"
#include "services/CharacterRepository.h"

// ViewModels
#include "viewmodels/EpisodeListViewModel.h"
#include "viewmodels/EpisodeDetailViewModel.h"
#include "viewmodels/CharacterListViewModel.h"

Q_IMPORT_QML_PLUGIN(RickMortyUIPlugin)
Q_IMPORT_QML_PLUGIN(RickMortyViewModelsPlugin)

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Create services (dependency injection)
    rickmorty::services::ApiService apiService;
    rickmorty::services::EpisodeRepository episodeRepository(&apiService);
    rickmorty::services::CharacterRepository characterRepository(&apiService);

    // Create ViewModels
    rickmorty::viewmodels::EpisodeListViewModel episodeListVM(&episodeRepository);
    rickmorty::viewmodels::EpisodeDetailViewModel episodeDetailVM;
    rickmorty::viewmodels::CharacterListViewModel characterListVM(&characterRepository);

    // Connect ViewModels: when episode is selected, update detail and load characters
    QObject::connect(&episodeListVM,
                     &rickmorty::viewmodels::EpisodeListViewModel::episodeSelected,
                     [&](const std::vector<int>& characterIds) {
        const int selectedIndex = episodeListVM.selectedIndex();
        if (selectedIndex >= 0) {
            episodeDetailVM.setEpisode(episodeListVM.getEpisode(selectedIndex));
            characterListVM.loadCharacters(characterIds);
        } else {
            episodeDetailVM.clear();
            characterListVM.clear();
        }
    });

    // Expose ViewModels to QML (prefixed to avoid name shadowing in QML)
    engine.rootContext()->setContextProperty("$episodeListVM", &episodeListVM);
    engine.rootContext()->setContextProperty("$episodeDetailVM", &episodeDetailVM);
    engine.rootContext()->setContextProperty("$characterListVM", &characterListVM);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
    );

    engine.loadFromModule("RickMortyAPI", "Main");

    // Load initial data
    episodeListVM.loadEpisodes();

    return app.exec();
}
