import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property var episode: null
    property var castModel: null
    property bool isLoadingCast: false

    ColumnLayout {
        anchors.fill: parent
        spacing: Theme.spacingXLarge

        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingXLarge

            EpisodeHeader {
                id: header
                title: root.episode ? root.episode.name : ""
                season: root.episode ? root.episode.season : 0
                episodeNumber: root.episode ? root.episode.episodeNumber : 0
            }

            Item { Layout.fillWidth: true }

            Rectangle {
                width: 200
                height: 200
                radius: Theme.borderRadius
                color: Theme.surfaceLight
                visible: root.episode !== null

                Image {
                    anchors.fill: parent
                    anchors.margins: Theme.spacingMedium
                    source: root.episode ? root.episode.image : ""
                    fillMode: Image.PreserveAspectFit
                    asynchronous: true
                }
            }
        }

        CastGrid {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: root.castModel
            isLoading: root.isLoadingCast
        }
    }
}
