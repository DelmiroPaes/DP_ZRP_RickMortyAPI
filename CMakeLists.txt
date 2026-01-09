cmake_minimum_required(VERSION 3.16)

project(RickMortyAPI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTING "Build unit tests" ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Network Svg)

qt_standard_project_setup(REQUIRES 6.8)

# Add layers in dependency order
add_subdirectory(src/core)
add_subdirectory(src/services)
add_subdirectory(src/viewmodels)
# RickMortyUI must come after ViewModels since QML imports RickMortyViewModels
add_subdirectory(modules/RickMortyUI)

qt_add_executable(appRickMortyAPI
    src/main.cpp
)

# Add application icon resources
qt_add_resources(appRickMortyAPI "icons"
    PREFIX "/"
    FILES
        resources/icon.svg
)

# Windows executable icon (requires icon.ico file)
if(WIN32 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.ico")
    target_sources(appRickMortyAPI PRIVATE resources/app.rc)
endif()

qt_add_qml_module(appRickMortyAPI
    URI RickMortyAPI
    QML_FILES
        qml/Main.qml
)

set_target_properties(appRickMortyAPI PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appRickMortyAPI
    PRIVATE
        Qt6::Quick
        Qt6::Network
        Qt6::Svg
        RickMortyCore
        RickMortyServices
        RickMortyViewModelsplugin
        RickMortyUIplugin
)

target_include_directories(appRickMortyAPI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Deploy Qt dependencies to build directory after build
if(WIN32)
    # Find windeployqt executable
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET appRickMortyAPI POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --no-translations
                --no-system-d3d-compiler
                --no-opengl-sw
                --qmldir "${CMAKE_CURRENT_SOURCE_DIR}/qml"
                --qmldir "${CMAKE_CURRENT_SOURCE_DIR}/modules/RickMortyUI/qml"
                "$<TARGET_FILE:appRickMortyAPI>"
            COMMENT "Deploying Qt dependencies..."
            VERBATIM
        )
    else()
        message(WARNING "windeployqt not found, Qt DLLs will not be copied automatically")
    endif()
endif()

include(GNUInstallDirs)
install(TARGETS appRickMortyAPI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Deploy target: install + windeployqt (Release only)
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set(DEPLOY_DIR "${CMAKE_SOURCE_DIR}/install")

    add_custom_target(deploy
        COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --prefix ${DEPLOY_DIR}
        COMMAND ${WINDEPLOYQT_EXECUTABLE}
            --no-translations
            --no-system-d3d-compiler
            --no-opengl-sw
            --qmldir "${CMAKE_SOURCE_DIR}/qml"
            --qmldir "${CMAKE_SOURCE_DIR}/modules/RickMortyUI/qml"
            "${DEPLOY_DIR}/bin/appRickMortyAPI.exe"
        DEPENDS appRickMortyAPI
        COMMENT "Deploying application to ${DEPLOY_DIR}..."
        VERBATIM
    )
endif()

# Unit tests
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
