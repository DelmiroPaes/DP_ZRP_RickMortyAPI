import QtQuick
import QtQuick.Layouts

Item {
    id: root

    // ViewModel references (any QAbstractListModel or compatible object)
    property var episodeListViewModel: null
    property var episodeDetailViewModel: null
    property var characterListViewModel: null

    // State constants (matching ViewState enum)
    readonly property int stateIdle: 0
    readonly property int stateLoading: 1
    readonly property int stateSuccess: 2
    readonly property int stateError: 3

    signal episodeSelected(int index)

    RowLayout {
        anchors.fill: parent
        spacing: Theme.spacingLarge

        EpisodeListPanel {
            Layout.preferredWidth: Theme.episodePanelWidth
            Layout.fillHeight: true
            model: root.episodeListViewModel
            viewState: root.episodeListViewModel ? root.episodeListViewModel.state : root.stateIdle
            selectedIndex: root.episodeListViewModel ? root.episodeListViewModel.selectedIndex : -1

            onItemClicked: function(index) {
                root.episodeSelected(index)
            }
        }

        EpisodeDetailPanel {
            Layout.fillWidth: true
            Layout.fillHeight: true
            viewModel: root.episodeDetailViewModel
            castViewModel: root.characterListViewModel
            visible: root.episodeDetailViewModel && root.episodeDetailViewModel.hasEpisode
        }

        // Empty state placeholder
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: !root.episodeDetailViewModel || !root.episodeDetailViewModel.hasEpisode

            Text {
                anchors.centerIn: parent
                text: qsTr("Select an episode to view details")
                color: Theme.textSecondary
                font.pixelSize: Theme.fontSizeLarge
            }
        }
    }
}
