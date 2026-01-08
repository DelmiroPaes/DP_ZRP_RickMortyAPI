import QtQuick
import RickMortyUI 1.0

Window {
    id: root
    width: 1280
    height: 800
    minimumWidth: 900
    minimumHeight: 600
    visible: true
    title: qsTr("Rick and Morty Episode Browser")
    color: "#1a1a2e"  // Theme.background

    EpisodeBrowser {
        anchors.fill: parent
        anchors.margins: 16

        episodesModel: dataController.episodes
        charactersModel: dataController.characters
        selectedEpisode: dataController.selectedEpisode
        isLoadingEpisodes: dataController.isLoadingEpisodes
        isLoadingCharacters: dataController.isLoadingCharacters

        onEpisodeSelected: function(index) {
            dataController.selectEpisode(index)
        }
    }
}
