import QtQuick

Column {
    id: root

    property string title: ""
    property int season: 0
    property int episodeNumber: 0

    spacing: Theme.spacingSmall

    Text {
        text: root.title
        color: Theme.accentLight
        font.pixelSize: Theme.fontSizeHeader
        font.bold: true
        font.italic: true
    }

    Text {
        text: "Season %1, Episode %2".arg(root.season).arg(root.episodeNumber)
        color: Theme.textSecondary
        font.pixelSize: Theme.fontSizeLarge
    }
}
