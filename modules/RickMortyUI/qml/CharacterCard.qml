import QtQuick

Item {
    id: root

    property string imageUrl: ""
    property string name: ""

    implicitWidth: Theme.characterCardWidth
    implicitHeight: Theme.characterCardHeight

    Rectangle {
        anchors.fill: parent
        color: Theme.surface
        radius: Theme.borderRadius

        Column {
            anchors.fill: parent
            anchors.margins: Theme.spacingMedium
            spacing: Theme.spacingMedium

            Rectangle {
                id: avatarContainer
                width: Theme.avatarSize
                height: Theme.avatarSize
                radius: Theme.borderRadius
                color: Theme.surfaceLight
                anchors.horizontalCenter: parent.horizontalCenter
                clip: true

                Image {
                    id: avatarImage
                    anchors.fill: parent
                    source: root.imageUrl
                    fillMode: Image.PreserveAspectCrop
                    asynchronous: true
                    visible: status === Image.Ready

                    onStatusChanged: {
                        if (status === Image.Error) {
                            placeholderText.visible = true
                        }
                    }
                }

                Text {
                    id: placeholderText
                    anchors.centerIn: parent
                    text: root.name.charAt(0).toUpperCase()
                    font.pixelSize: Theme.fontSizeHeader
                    font.bold: true
                    color: Theme.textSecondary
                    visible: avatarImage.status !== Image.Ready
                }
            }

            Text {
                width: parent.width
                text: root.name
                color: Theme.textPrimary
                font.pixelSize: Theme.fontSizeMedium
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideRight
                maximumLineCount: 2
                wrapMode: Text.WordWrap
            }
        }
    }
}
