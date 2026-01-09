import QtQuick

Column {
    id: root

    property string title: ""
    property string formattedSeason: ""

    spacing: Theme.spacingSmall

    Text {
        width: root.width
        text: root.title
        color: Theme.accentLight
        font.pixelSize: Theme.fontSizeHeader
        font.bold: true
        font.italic: true
        elide: Text.ElideRight
    }

    Text {
        text: root.formattedSeason
        color: Theme.textSecondary
        font.pixelSize: Theme.fontSizeLarge
    }
}
