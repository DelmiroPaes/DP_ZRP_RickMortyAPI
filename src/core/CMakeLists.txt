# Core layer - Pure C++17, NO Qt dependencies
add_library(RickMortyCore INTERFACE)

target_include_directories(RickMortyCore INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(RickMortyCore INTERFACE cxx_std_17)
