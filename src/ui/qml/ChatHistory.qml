import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.koolintelligence 1.0
import org.kde.koolintelligence.components 1.0
Controls.ScrollView {
    Layout.fillWidth: true
    Layout.fillHeight: true
    anchors.margins: 0
    width: parent.width
    clip: true
    Controls.ScrollBar.horizontal.policy: Controls.ScrollBar.AlwaysOff
    Controls.ScrollBar.vertical.policy: Controls.ScrollBar.AsNeeded
    Controls.ScrollBar.vertical.interactive: true
    Kirigami.CardsListView {
        id: chatHistoryView
        model: ChatHistoryModel
        delegate: chatHistoryDelegate
    }
    Component {
        id: chatHistoryDelegate
        Kirigami.AbstractCard {
            width: parent?.width - anchors.margins
            anchors.right: alignLeft ? undefined : parent?.right
            anchors.left: alignLeft ? parent?.left : undefined
            anchors.margins: 90
            contentItem: Item {
            implicitHeight: messageLayout.implicitHeight
            implicitWidth: messageLayout.implicitWidth
            ColumnLayout {
                id: messageLayout
                width: parent.width
                Kirigami.Heading {
                text: author
                level:5
                Layout.alignment: alignLeft ? Qt.AlignLeft : Qt.AlignRight
                }
                Kirigami.Separator {
                Layout.fillWidth: true
                }
                Kirigami.Heading {
                Layout.fillWidth: true
                text: chatText
                level: 3
                wrapMode: Text.WordWrap
                Layout.alignment: alignLeft ? Qt.AlignLeft : Qt.AlignRight
                }
                // Controls.MarkdownText {
                // Layout.fillWidth: true
                // text: chatText
                // level: 3
                // wrapMode: Text.WordWrap
                // Layout.alignment: alignLeft ? Qt.AlignLeft : Qt.AlignRight
                // }
                Kirigami.Heading {
                text: time
                level: 9
                Layout.alignment: Qt.AlignRight
                }
            }
            }
        }
    }
}