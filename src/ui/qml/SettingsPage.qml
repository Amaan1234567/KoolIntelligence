import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.koolintelligence 1.0
import org.kde.koolintelligence.components 1.0

Kirigami.Page {
    id: settingsPage
    title: i18nc("@title", "Settings")
    visible: false
    actions: [
        Kirigami.Action {
            text: i18n("Go Back")
            icon.name: "go-previous-symbolic"
            onTriggered: {
                pageStack.pop()
                initialPage.visible = true
            }
        }
    ]
    ColumnLayout{
        width: parent.width
        height: parent.height
        Controls.Label {
            text: i18n("Settings")
        }
    }
}