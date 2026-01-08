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
    color: "#1a1a2e"

    EpisodeBrowser {
        anchors.fill: parent
        anchors.margins: 16

        episodeListViewModel: $episodeListVM
        episodeDetailViewModel: $episodeDetailVM
        characterListViewModel: $characterListVM

        onEpisodeSelected: function(index) {
            $episodeListVM.selectedIndex = index
        }
    }
}
