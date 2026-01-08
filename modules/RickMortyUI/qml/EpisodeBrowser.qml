import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property var episodesModel: null
    property var charactersModel: null
    property var selectedEpisode: null
    property bool isLoadingEpisodes: false
    property bool isLoadingCharacters: false

    signal episodeSelected(int index)

    RowLayout {
        anchors.fill: parent
        spacing: Theme.spacingLarge

        EpisodeListPanel {
            Layout.preferredWidth: Theme.episodePanelWidth
            Layout.fillHeight: true
            model: root.episodesModel
            isLoading: root.isLoadingEpisodes
            selectedIndex: root.selectedEpisode ? root.selectedEpisode.index : -1

            onItemClicked: function(index) {
                root.episodeSelected(index)
            }
        }

        EpisodeDetailPanel {
            Layout.fillWidth: true
            Layout.fillHeight: true
            episode: root.selectedEpisode
            castModel: root.charactersModel
            isLoadingCast: root.isLoadingCharacters
            visible: root.selectedEpisode && root.selectedEpisode.id !== undefined
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: !root.selectedEpisode || root.selectedEpisode.id === undefined

            Text {
                anchors.centerIn: parent
                text: qsTr("Select an episode to view details")
                color: Theme.textSecondary
                font.pixelSize: Theme.fontSizeLarge
            }
        }
    }
}
