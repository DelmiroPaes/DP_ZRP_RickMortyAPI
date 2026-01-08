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
            spacing: Theme.spacingLarge

            EpisodeHeader {
                id: header
                Layout.fillWidth: true
                Layout.minimumWidth: 150
                title: root.viewModel ? root.viewModel.name : ""
                formattedSeason: root.viewModel ? root.viewModel.formattedSeason : ""
            }

            // Character detail panel - fixed width, scrollable content
            Rectangle {
                id: characterDetailPanel
                Layout.preferredWidth: 220
                Layout.minimumWidth: 220
                Layout.maximumWidth: 220
                height: 200
                radius: Theme.borderRadius
                color: Theme.surfaceLight
                visible: root.viewModel && root.viewModel.hasEpisode
                clip: true

                Row {
                    id: panelRow
                    anchors.fill: parent
                    anchors.margins: Theme.spacingSmall
                    spacing: Theme.spacingSmall

                    // Character image
                    Rectangle {
                        id: imageContainer
                        width: 80
                        height: parent.height
                        radius: Theme.borderRadiusSmall
                        color: Theme.surface
                        clip: true

                        Image {
                            id: characterImage
                            anchors.fill: parent
                            source: root.castViewModel && root.castViewModel.hasSelection
                                    ? root.castViewModel.selectedImageUrl : ""
                            fillMode: Image.PreserveAspectCrop
                            asynchronous: true
                            cache: true
                            opacity: status === Image.Ready ? 1 : 0

                            Behavior on opacity {
                                NumberAnimation { duration: 300; easing.type: Easing.OutCubic }
                            }

                            onSourceChanged: {
                                if (source != "") entranceAnimation.restart()
                            }

                            SequentialAnimation {
                                id: entranceAnimation
                                NumberAnimation { target: characterImage; property: "scale"; from: 0.9; to: 1.02; duration: 200; easing.type: Easing.OutQuad }
                                NumberAnimation { target: characterImage; property: "scale"; to: 1.0; duration: 150; easing.type: Easing.InOutQuad }
                            }

                            SequentialAnimation {
                                id: breathingAnimation
                                running: root.castViewModel && root.castViewModel.hasSelection && characterImage.status === Image.Ready
                                loops: Animation.Infinite
                                NumberAnimation { target: characterImage; property: "scale"; from: 1.0; to: 1.03; duration: 2000; easing.type: Easing.InOutSine }
                                NumberAnimation { target: characterImage; property: "scale"; from: 1.03; to: 1.0; duration: 2000; easing.type: Easing.InOutSine }
                            }
                        }

                        Text {
                            anchors.centerIn: parent
                            text: qsTr("Select a\ncharacter")
                            color: Theme.textSecondary
                            font.pixelSize: 10
                            horizontalAlignment: Text.AlignHCenter
                            visible: !root.castViewModel || !root.castViewModel.hasSelection
                        }

                        Text {
                            anchors.centerIn: parent
                            text: "..."
                            color: Theme.textSecondary
                            font.pixelSize: Theme.fontSizeLarge
                            visible: characterImage.status === Image.Loading
                        }
                    }

                    // Info area - scrollable
                    Flickable {
                        id: infoFlickable
                        width: parent.width - imageContainer.width - parent.spacing
                        height: parent.height
                        contentWidth: width
                        contentHeight: infoColumn.height
                        clip: true
                        boundsBehavior: Flickable.StopAtBounds
                        flickableDirection: Flickable.VerticalFlick

                        Column {
                            id: infoColumn
                            width: infoFlickable.width
                            spacing: 4

                            Text {
                                width: parent.width
                                text: root.castViewModel && root.castViewModel.hasSelection ? root.castViewModel.selectedName : ""
                                color: Theme.textPrimary
                                font.pixelSize: 13
                                font.bold: true
                                wrapMode: Text.Wrap
                            }

                            Row {
                                spacing: 4
                                visible: root.castViewModel && root.castViewModel.hasSelection

                                Rectangle {
                                    width: 6; height: 6; radius: 3
                                    anchors.verticalCenter: parent.verticalCenter
                                    color: {
                                        if (!root.castViewModel) return Theme.textSecondary
                                        var s = root.castViewModel.selectedStatus
                                        return s === "Alive" ? "#4CAF50" : s === "Dead" ? "#F44336" : Theme.textSecondary
                                    }
                                }
                                Text {
                                    text: root.castViewModel ? root.castViewModel.selectedStatus : ""
                                    color: Theme.textSecondary
                                    font.pixelSize: 11
                                }
                            }

                            Text {
                                width: parent.width
                                text: {
                                    if (!root.castViewModel || !root.castViewModel.hasSelection) return ""
                                    var t = root.castViewModel.selectedSpecies
                                    if (root.castViewModel.selectedType) t += " (" + root.castViewModel.selectedType + ")"
                                    return t
                                }
                                color: Theme.textSecondary
                                font.pixelSize: 11
                                wrapMode: Text.Wrap
                                visible: text !== ""
                            }

                            Text {
                                width: parent.width
                                text: root.castViewModel && root.castViewModel.hasSelection ? root.castViewModel.selectedGender : ""
                                color: Theme.textSecondary
                                font.pixelSize: 11
                                visible: text !== ""
                            }

                            Item { width: 1; height: 4 }

                            Text {
                                width: parent.width
                                text: root.castViewModel && root.castViewModel.hasSelection && root.castViewModel.selectedOrigin !== "unknown"
                                      ? "Origin: " + root.castViewModel.selectedOrigin : ""
                                color: Theme.textSecondary
                                font.pixelSize: 10
                                wrapMode: Text.Wrap
                                visible: text !== ""
                            }

                            Text {
                                width: parent.width
                                text: root.castViewModel && root.castViewModel.hasSelection && root.castViewModel.selectedLocation !== "unknown"
                                      ? "Location: " + root.castViewModel.selectedLocation : ""
                                color: Theme.textSecondary
                                font.pixelSize: 10
                                wrapMode: Text.Wrap
                                visible: text !== ""
                            }

                            Item { width: 1; height: 4 }

                            Text {
                                text: root.castViewModel && root.castViewModel.hasSelection
                                      ? root.castViewModel.selectedEpisodeCount + " episodes" : ""
                                color: Theme.accent
                                font.pixelSize: 11
                                font.bold: true
                                visible: text !== ""
                            }
                        }
                    }

                    // Scroll indicator
                    Rectangle {
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: 2
                        color: "transparent"
                        visible: infoFlickable.contentHeight > infoFlickable.height

                        Rectangle {
                            anchors.right: parent.right
                            width: 2
                            height: Math.max(20, infoFlickable.height * (infoFlickable.height / infoFlickable.contentHeight))
                            radius: 1
                            color: Theme.accent
                            opacity: 0.4
                            y: infoFlickable.contentHeight > infoFlickable.height
                               ? (infoFlickable.contentY / (infoFlickable.contentHeight - infoFlickable.height)) * (parent.height - height)
                               : 0
                        }
                    }
                }
            }
        }

        CastGrid {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: root.castViewModel
            viewState: root.castViewModel ? root.castViewModel.state : 0
            selectedIndex: root.castViewModel ? root.castViewModel.selectedIndex : -1

            onCharacterClicked: function(index) {
                if (root.castViewModel) {
                    root.castViewModel.setSelectedIndex(index)
                }
            }
        }
    }
}
