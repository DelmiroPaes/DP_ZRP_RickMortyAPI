import QtQuick

Rectangle {
    id: root

    property string thumbnailUrl: ""
    property string title: ""
    property string episodeCode: ""
    property bool isSelected: false

    signal clicked()

    implicitHeight: Theme.episodeItemHeight
    color: isSelected ? Theme.accent : "transparent"
    radius: Theme.borderRadius

    Rectangle {
        anchors.fill: parent
        anchors.margins: 2
        color: isSelected ? Theme.accent : Theme.surface
        radius: Theme.borderRadius

        Row {
            anchors.fill: parent
            anchors.margins: Theme.spacingMedium
            spacing: Theme.spacingLarge

            Rectangle {
                id: thumbnailContainer
                width: Theme.thumbnailWidth
                height: Theme.thumbnailHeight
                radius: Theme.borderRadiusSmall
                color: Theme.surfaceLight
                anchors.verticalCenter: parent.verticalCenter
                clip: true

                Image {
                    anchors.fill: parent
                    source: root.thumbnailUrl
                    fillMode: Image.PreserveAspectCrop
                    asynchronous: true
                    visible: status === Image.Ready
                }

                Text {
                    anchors.centerIn: parent
                    text: root.episodeCode
                    font.pixelSize: Theme.fontSizeSmall
                    font.bold: true
                    color: Theme.textSecondary
                    visible: parent.children[0].status !== Image.Ready
                }
            }

            Column {
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width - thumbnailContainer.width - Theme.spacingLarge
                spacing: Theme.spacingSmall

                Text {
                    width: parent.width
                    text: root.title
                    color: Theme.textPrimary
                    font.pixelSize: Theme.fontSizeMedium
                    font.bold: true
                    elide: Text.ElideRight
                    maximumLineCount: 2
                    wrapMode: Text.WordWrap
                }

                Text {
                    text: root.episodeCode
                    color: Theme.textSecondary
                    font.pixelSize: Theme.fontSizeSmall
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
    }
}
