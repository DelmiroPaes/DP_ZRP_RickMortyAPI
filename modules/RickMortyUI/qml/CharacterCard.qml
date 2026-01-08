import QtQuick

Item {
    id: root

    property string imageUrl: ""
    property string name: ""
    property string initial: ""

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
                    cache: true
                    sourceSize.width: Theme.avatarSize * 2
                    sourceSize.height: Theme.avatarSize * 2
                    visible: status === Image.Ready
                }

                // Loading spinner
                Item {
                    id: loadingSpinner
                    anchors.centerIn: parent
                    width: 32
                    height: 32
                    visible: avatarImage.status === Image.Loading

                    Rectangle {
                        id: spinnerRing
                        anchors.fill: parent
                        radius: width / 2
                        color: "transparent"
                        border.width: 3
                        border.color: Theme.surfaceLight

                        Rectangle {
                            width: parent.width
                            height: parent.height
                            radius: width / 2
                            color: "transparent"
                            border.width: 3
                            border.color: Theme.accent
                            visible: false

                            layer.enabled: true
                            layer.effect: Item {}
                        }

                        // Animated arc indicator
                        Rectangle {
                            id: spinnerArc
                            width: 8
                            height: 8
                            radius: 4
                            color: Theme.accent
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.top: parent.top
                            anchors.topMargin: -1
                        }

                        RotationAnimation on rotation {
                            from: 0
                            to: 360
                            duration: 1000
                            loops: Animation.Infinite
                            running: loadingSpinner.visible
                        }
                    }
                }

                // Placeholder with initial (shown on error or empty URL)
                Text {
                    id: placeholderText
                    anchors.centerIn: parent
                    text: root.initial
                    font.pixelSize: Theme.fontSizeHeader
                    font.bold: true
                    color: Theme.textSecondary
                    visible: avatarImage.status === Image.Error ||
                             avatarImage.status === Image.Null ||
                             root.imageUrl === ""
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
