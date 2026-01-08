import QtQuick
import QtQuick.Layouts

Item {
    id: root

    // ViewModel references
    property var viewModel: null
    property var castViewModel: null

    ColumnLayout {
        anchors.fill: parent
        spacing: Theme.spacingXLarge

        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingXLarge

            EpisodeHeader {
                id: header
                title: root.viewModel ? root.viewModel.name : ""
                formattedSeason: root.viewModel ? root.viewModel.formattedSeason : ""
            }

            Item { Layout.fillWidth: true }

            Rectangle {
                width: 200
                height: 200
                radius: Theme.borderRadius
                color: Theme.surfaceLight
                visible: root.viewModel && root.viewModel.hasEpisode

                Image {
                    anchors.fill: parent
                    anchors.margins: Theme.spacingMedium
                    source: root.viewModel ? root.viewModel.imageUrl : ""
                    fillMode: Image.PreserveAspectFit
                    asynchronous: true
                }
            }
        }

        CastGrid {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: root.castViewModel
            viewState: root.castViewModel ? root.castViewModel.state : 0
        }
    }
}
