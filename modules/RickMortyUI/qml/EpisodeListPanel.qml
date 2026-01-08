import QtQuick

Rectangle {
    id: root

    property var model: null
    property int selectedIndex: -1
    property bool isLoading: false

    signal itemClicked(int index)

    color: Theme.surface
    radius: Theme.borderRadius

    Column {
        anchors.fill: parent
        anchors.margins: Theme.spacingLarge
        spacing: Theme.spacingLarge

        Row {
            spacing: Theme.spacingMedium

            Text {
                text: "\u2630"
                color: Theme.textSecondary
                font.pixelSize: Theme.fontSizeLarge
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
                visible: !root.isLoading

                delegate: EpisodeListItem {
                    width: listView.width
                    thumbnailUrl: model.image || ""
                    title: model.name || ""
                    episodeCode: model.episode || ""
                    isSelected: index === root.selectedIndex

                    onClicked: root.itemClicked(index)
                }
            }

            Text {
                anchors.centerIn: parent
                text: qsTr("Loading episodes...")
                color: Theme.textSecondary
                font.pixelSize: Theme.fontSizeMedium
                visible: root.isLoading
            }
        }
    }
}
