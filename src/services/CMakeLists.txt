# Services layer - Qt Network for HTTP requests
add_library(RickMortyServices STATIC
    ApiService.cpp
    ApiService.h
    EpisodeRepository.cpp
    EpisodeRepository.h
    CharacterRepository.cpp
    CharacterRepository.h
)

target_link_libraries(RickMortyServices
    PUBLIC
        Qt6::Core
        Qt6::Network
        RickMortyCore
)

target_include_directories(RickMortyServices PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
