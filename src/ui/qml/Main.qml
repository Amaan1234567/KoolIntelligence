import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.koolintelligence 1.0
import org.kde.koolintelligence.components 1.0

Kirigami.ApplicationWindow {
    id: root

    width: 800
    height: 500

    title: i18nc("@title:window", "Kool Intelligence")

    globalDrawer: Kirigami.GlobalDrawer {
        id: globalDrawer
        isMenu: false
        actions: [
            Kirigami.Action {
                text: i18n("Settings")
                icon.name: "preferences-system-symbolic"
                onTriggered: {
                    initialPage.visible = false
                    pageStack.push(settingsPage)
                }
            }
        ]
    }
    SettingsPage{
        id: settingsPage
    }
    pageStack.initialPage: Kirigami.Page {
        id: initialPage
        title: i18nc("@title", "LLM Chat")
        actions: [
             Kirigami.Action {
                text: i18n("Terminal")
                icon.name: "utilities-terminal-symbolic"
                shortcut: "Ctrl+T"
                onTriggered: {
                    terminalLayout.visible = !terminalLayout.visible
                    terminalSeperator.visible = !terminalSeperator.visible
                }
            }
        ]
        spacing: 0
        Controls.SplitView{
            spacing: 0
            orientation: Qt.Vertical
            anchors.fill: parent
            ColumnLayout{
                width: parent.width
                height: parent.height
                ChatHistory{
                    id: chatHistory
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
                Kirigami.ActionTextField {
                    Layout.alignment: Qt.AlignBottom
                    Layout.fillWidth: true
                    id: chatField
                    placeholderText: i18n("What do you want to do?")
                    focusSequence: StandardKey.Find
                    rightActions: [
                        Kirigami.Action {
                            icon.name: "go-next"
                            shortcut: StandardKey.Enter
                            onTriggered: {
                                UserPromptField.handleTextChange(chatField.text)
                                chatField.text = ""
                            }
                        }
                    ]
                    Keys.onReturnPressed: {
                        UserPromptField.handleTextChange(chatField.text)
                        chatField.text = ""
                    }
                }
                Kirigami.Separator {
                    id: terminalSeperator
                    visible: false
                    Layout.fillWidth: true
                }
            }
            TerminalLayout{
                spacing: 0
                id: terminalLayout
                visible: false
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

        }
    }
   
}