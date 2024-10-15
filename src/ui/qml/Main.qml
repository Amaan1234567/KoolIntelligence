import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
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

    SplitView {
        id: splitView
        orientation: Qt.Vertical
        anchors.fill: parent

        // Chat area (first item in SplitView)
        Kirigami.Page {
            id: chatPage
            title: i18nc("@title", "LLM Chat")

            ColumnLayout {
                width: parent.width
                height: parent.height

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    anchors.margins: 0
                    width: parent.width
                    clip: true

                    Kirigami.CardsListView {
                        id: cardsView
                        model: ChatHistoryModel
                        delegate: chatHistoryDelegate
                    }
                }

                Kirigami.ActionTextField {
                    id: chatField
                    Layout.alignment: Qt.AlignBottom
                    Layout.fillWidth: true
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
            }
        }

        // Terminal area (second item in SplitView)
        Kirigami.Page {
            id: terminalPanel
            title: i18nc("@title", "Terminal")
            Layout.minimumHeight: 150 // Minimum height for the terminal

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                TerminalPanel {
                    id: terminalPanel
                    width: parent.width
                    height: parent.height
                }
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
                        level: 5
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
        ColumnLayout {
            width: parent.width
            height: parent.height

            Label {
                text: i18n("Settings")
            }
        }
    }
}
