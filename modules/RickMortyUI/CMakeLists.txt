set_source_files_properties(qml/Theme.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(RickMortyUI
    URI RickMortyUI
    VERSION 1.0
    STATIC
    RESOURCE_PREFIX /qt/qml
    QML_FILES
        qml/EpisodeBrowser.qml
        qml/Theme.qml
        qml/EpisodeListPanel.qml
        qml/EpisodeListItem.qml
        qml/EpisodeDetailPanel.qml
        qml/EpisodeHeader.qml
        qml/CastGrid.qml
        qml/CharacterCard.qml
)
