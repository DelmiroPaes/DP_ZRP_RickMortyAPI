# Google Test configuration
include(FetchContent)

# Download dependencies to deps/ at project root (not inside build/)
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/deps")

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(googletest)

# Hide Google Test targets from IDE (group under "deps" folder)
set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES FOLDER "deps")

# Enable GoogleTest CMake integration for Qt Creator
include(GoogleTest)

# Include directories for tests
include_directories(${CMAKE_SOURCE_DIR}/src)

# Helper function to deploy Qt dependencies for test executables (Windows)
function(deploy_qt_for_test target)
    if(WIN32)
        get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
        get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
        find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

        if(WINDEPLOYQT_EXECUTABLE)
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                    --no-translations
                    --no-system-d3d-compiler
                    --no-opengl-sw
                    --no-compiler-runtime
                    "$<TARGET_FILE:${target}>"
                COMMENT "Deploying Qt for ${target}..."
                VERBATIM
            )
        endif()
    endif()
endfunction()

# Core Tests (no Qt dependency)
add_executable(CoreTests
    main.cpp
    core/CharacterTests.cpp
    core/EpisodeTests.cpp
)
target_link_libraries(CoreTests PRIVATE
    GTest::gtest
    GTest::gmock
    RickMortyCore
)
gtest_discover_tests(CoreTests)

# Services Tests (Qt dependency)
add_executable(ServicesTests
    main.cpp
    services/EpisodeRepositoryTests.cpp
    services/CharacterRepositoryTests.cpp
)
target_link_libraries(ServicesTests PRIVATE
    GTest::gtest
    GTest::gmock
    RickMortyServices
    Qt6::Core
    Qt6::Network
)
deploy_qt_for_test(ServicesTests)
gtest_discover_tests(ServicesTests)

# ViewModel Tests (Qt + QML dependency)
find_package(Qt6 REQUIRED COMPONENTS Test)

add_executable(ViewModelTests
    main.cpp
    viewmodels/EpisodeDetailViewModelTests.cpp
    viewmodels/EpisodeListViewModelTests.cpp
    viewmodels/CharacterListViewModelTests.cpp
)
target_link_libraries(ViewModelTests PRIVATE
    GTest::gtest
    GTest::gmock
    RickMortyViewModels
    RickMortyServices
    Qt6::Core
    Qt6::Qml
    Qt6::Quick
    Qt6::Test
)
deploy_qt_for_test(ViewModelTests)
gtest_discover_tests(ViewModelTests)
