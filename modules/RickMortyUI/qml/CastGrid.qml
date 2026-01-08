import QtQuick

Item {
    id: root

    property var model: null
    property bool isLoading: false

    Column {
        anchors.fill: parent
        spacing: Theme.spacingLarge

        Text {
            text: qsTr("Cast (A-Z)")
            color: Theme.textPrimary
            font.pixelSize: Theme.fontSizeLarge
            font.bold: true
        }

        Item {
            width: parent.width
            height: parent.height - y

            GridView {
                id: gridView
                anchors.fill: parent
                model: root.model
                cellWidth: Theme.characterCardWidth + Theme.spacingMedium
                cellHeight: Theme.characterCardHeight + Theme.spacingMedium
                clip: true
                visible: !root.isLoading && root.model && root.model.count > 0

                delegate: CharacterCard {
                    width: Theme.characterCardWidth
                    height: Theme.characterCardHeight
                    imageUrl: model.image || ""
                    name: model.name || ""
                }
            }

            Text {
                anchors.centerIn: parent
                text: root.isLoading ? qsTr("Loading cast...") : qsTr("No cast information")
                color: Theme.textSecondary
                font.pixelSize: Theme.fontSizeMedium
                visible: root.isLoading || !root.model || root.model.count === 0
            }
        }
    }
}
