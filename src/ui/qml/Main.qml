import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.koolintelligence 1.0

Kirigami.ApplicationWindow {
    id: root

    width: 800
    height: 500

    title: i18nc("@title:window", "Kool Intelligence")

    globalDrawer: Kirigami.GlobalDrawer {
        id: globalDrawer
        isMenu: true
        actions: [
            Kirigami.Action {
                text: i18n("Quit")
                icon.name: "application-exit-symbolic"
                shortcut: StandardKey.Quit
                onTriggered: Qt.quit()
            },
            Kirigami.Action {
                text: i18n("Settings")
                icon.name: "preferences-system-symbolic"
                onTriggered: {
                    pageStack.push(settingsPage)
                }
            }
        ]
    }
    pageStack.initialPage: Kirigami.Page {
        id: initialPage
        title: i18nc("@title", "LLM Chat")
        Controls.SplitView{
            orientation: Qt.Vertical
            anchors.fill: parent
            ColumnLayout{
                width: parent.width
                height: parent.height
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
                    id: cardsView
                    model: ChatHistoryModel
                    delegate: chatHistoryDelegate
                    }
                }
                Kirigami.ActionTextField {
                    Layout.alignment: Qt.AlignBottom
                    Layout.fillWidth: true
                    id: chatField
                    height: 50
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
                Layout.fillWidth: true
                }
            }
            Controls.ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                // Item {
                //     id: terminal
                //     anchors.fill: parent
                //     contentItem: terminalWidget
                // }

            }
        }
    }

    Kirigami.Page {
        id: settingsPage
        title: i18nc("@title", "Settings")
        visible: false
        actions: [
            Kirigami.Action {
                text: i18n("Go Back")
                icon.name: "go-previous-symbolic"
                onTriggered: {
                    pageStack.pop(settingsPage)
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