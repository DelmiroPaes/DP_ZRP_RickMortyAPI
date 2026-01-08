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

    // Panel visibility state
    property bool panelVisible: true

    signal episodeSelected(int index)

    Row {
        anchors.fill: parent
        spacing: Theme.spacingLarge

        // Episode list panel container with slide animation
        Item {
            id: panelContainer
            width: root.panelVisible ? Theme.episodePanelWidth : 0
            height: parent.height
            clip: true

            Behavior on width {
                NumberAnimation {
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }

            EpisodeListPanel {
                id: episodeListPanel
                width: Theme.episodePanelWidth
                height: parent.height
                x: root.panelVisible ? 0 : -width
                model: root.episodeListViewModel
                viewState: root.episodeListViewModel ? root.episodeListViewModel.state : root.stateIdle
                selectedIndex: root.episodeListViewModel ? root.episodeListViewModel.selectedIndex : -1

                Behavior on x {
                    NumberAnimation {
                        duration: 250
                        easing.type: Easing.InOutQuad
                    }
                }

                onItemClicked: function(index) {
                    root.episodeSelected(index)
                }

                onMenuClicked: {
                    root.panelVisible = !root.panelVisible
                }
            }
        }

        // Detail area
        Item {
            width: parent.width - panelContainer.width - parent.spacing
            height: parent.height

            EpisodeDetailPanel {
                anchors.fill: parent
                viewModel: root.episodeDetailViewModel
                castViewModel: root.characterListViewModel
                visible: root.episodeDetailViewModel && root.episodeDetailViewModel.hasEpisode
            }

            // Empty state placeholder
            Text {
                anchors.centerIn: parent
                text: qsTr("Select an episode to view details")
                color: Theme.textSecondary
                font.pixelSize: Theme.fontSizeLarge
                visible: !root.episodeDetailViewModel || !root.episodeDetailViewModel.hasEpisode
            }
        }
    }

    // Toggle button - fixed position at top left (same as menu icon in EpisodeListPanel)
    Rectangle {
        id: toggleButton
        x: Theme.spacingLarge
        y: Theme.spacingLarge
        width: 30
        height: 30
        radius: Theme.borderRadiusSmall
        color: toggleMouseArea.containsMouse ? Theme.surfaceLight : Theme.surface
        visible: !root.panelVisible
        opacity: visible ? 1 : 0
        scale: visible ? 1 : 0.8

        Behavior on opacity {
            NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
        }

        Behavior on scale {
            NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
        }

        Text {
            anchors.centerIn: parent
            text: "\u2630"
            color: toggleMouseArea.containsMouse ? Theme.accent : Theme.textSecondary
            font.pixelSize: Theme.fontSizeLarge
        }

        MouseArea {
            id: toggleMouseArea
            anchors.fill: parent
            anchors.margins: -Theme.spacingSmall
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: root.panelVisible = true
        }
    }
}
