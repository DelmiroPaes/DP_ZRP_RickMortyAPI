import QtQuick

Item {
    id: root

    property var model: null
    property int viewState: 0  // 0=Idle, 1=Loading, 2=Success, 3=Error
    property int selectedIndex: -1

    signal characterClicked(int index)

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
                visible: root.viewState === 2 && root.model && root.model.count > 0  // Success

                delegate: CharacterCard {
                    width: Theme.characterCardWidth
                    height: Theme.characterCardHeight
                    imageUrl: model.image || ""
                    name: model.name || ""
                    initial: model.initial || ""
                    selected: root.selectedIndex === model.index

                    onClicked: {
                        root.characterClicked(model.index)
                    }
                }
            }

            // Loading state
            Text {
                anchors.centerIn: parent
                text: qsTr("Loading cast...")
                color: Theme.textSecondary
                font.pixelSize: Theme.fontSizeMedium
                visible: root.viewState === 1  // Loading
            }

            // Error state
            Text {
                anchors.centerIn: parent
                text: qsTr("Failed to load cast")
                color: Theme.textSecondary
                font.pixelSize: Theme.fontSizeMedium
                visible: root.viewState === 3  // Error
            }

            // Empty/Idle state
            Text {
                anchors.centerIn: parent
                text: qsTr("No cast information")
                color: Theme.textSecondary
                font.pixelSize: Theme.fontSizeMedium
                visible: (root.viewState === 0 || root.viewState === 2)
                         && (!root.model || root.model.count === 0)
            }
        }
    }
}
