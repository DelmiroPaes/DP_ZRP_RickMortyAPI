import QtQuick

Rectangle {
    id: root

    property var model: null
    property int selectedIndex: -1
    property int viewState: 0  // 0=Idle, 1=Loading, 2=Success, 3=Error

    signal itemClicked(int index)
    signal menuClicked()

    color: Theme.surface
    radius: Theme.borderRadius

    Column {
        anchors.fill: parent
        anchors.margins: Theme.spacingLarge
        spacing: Theme.spacingLarge

        Row {
            spacing: Theme.spacingMedium

            Text {
                id: menuIcon
                text: "\u2630"
                color: menuMouseArea.containsMouse ? Theme.accent : Theme.textSecondary
                font.pixelSize: Theme.fontSizeLarge

                MouseArea {
                    id: menuMouseArea
                    anchors.fill: parent
                    anchors.margins: -Theme.spacingSmall
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.menuClicked()
                }
            }

            Text {
                text: qsTr("EPISODES")
                color: Theme.textPrimary
                font.pixelSize: Theme.fontSizeMedium
                font.bold: true
                font.letterSpacing: 2
            }
        }

        Item {
            width: parent.width
            height: parent.height - y

            ListView {
                id: listView
                anchors.fill: parent
                model: root.model
                spacing: Theme.spacingMedium
                clip: true
                visible: root.viewState === 2  // Success

                delegate: EpisodeListItem {
                    width: listView.width
                    thumbnailUrl: model.image || ""
                    title: model.name || ""
                    episodeCode: model.episodeCode || ""
                    isSelected: index === root.selectedIndex

                    onClicked: root.itemClicked(index)
                }
            }

            // Loading state
            Text {
                anchors.centerIn: parent
                text: qsTr("Loading episodes...")
                color: Theme.textSecondary
                font.pixelSize: Theme.fontSizeMedium
                visible: root.viewState === 1  // Loading
            }

            // Error state
            Column {
                anchors.centerIn: parent
                spacing: Theme.spacingMedium
                visible: root.viewState === 3  // Error

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTr("Failed to load episodes")
                    color: Theme.textSecondary
                    font.pixelSize: Theme.fontSizeMedium
                }
            }

            // Idle state
            Text {
                anchors.centerIn: parent
                text: qsTr("No episodes")
                color: Theme.textSecondary
                font.pixelSize: Theme.fontSizeMedium
                visible: root.viewState === 0  // Idle
            }
        }
    }
}
