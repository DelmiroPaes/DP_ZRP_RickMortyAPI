qt_add_library(RickMortyData STATIC
    src/ApiClient.cpp
    src/ApiClient.h
    src/EpisodeModel.cpp
    src/EpisodeModel.h
    src/CharacterModel.cpp
    src/CharacterModel.h
    src/DataController.cpp
    src/DataController.h
)

target_link_libraries(RickMortyData
    PUBLIC Qt6::Core Qt6::Network
    PRIVATE Qt6::Qml
)

target_include_directories(RickMortyData PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
