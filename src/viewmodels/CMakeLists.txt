# ViewModels layer - QObject-based ViewModels for QML
qt_add_library(RickMortyViewModels STATIC
    ViewState.h
    EpisodeListViewModel.cpp
    EpisodeListViewModel.h
    EpisodeDetailViewModel.cpp
    EpisodeDetailViewModel.h
    CharacterListViewModel.cpp
    CharacterListViewModel.h
)

qt_add_qml_module(RickMortyViewModels
    URI RickMortyViewModels
    VERSION 1.0
)

target_link_libraries(RickMortyViewModels
    PUBLIC
        Qt6::Core
        Qt6::Qml
        Qt6::Quick
        RickMortyCore
        RickMortyServices
)

target_include_directories(RickMortyViewModels PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
