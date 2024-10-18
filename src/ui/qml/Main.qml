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
    
    property bool splitTerminalVertical: false
    property string terminalColorScheme: "Ubuntu"
    
    pageStack.columnView.columnResizeMode: Kirigami.ColumnView.SingleColumn
    pageStack.globalToolBar.showNavigationButtons: !Kirigami.ApplicationHeaderStyle.ShowBackButton && !Kirigami.ApplicationHeaderStyle.ShowForwardButton
    globalDrawer: Kirigami.GlobalDrawer {
        id: globalDrawer
        isMenu: false
        actions: [
        ]
    }
    SettingsPage{
        id: settingsPage
    }
    pageStack.initialPage: Kirigami.Page {
        id: initialPage
        title: i18nc("@title", "LLM Chat")
        spacing: 0
        topPadding: 0
        bottomPadding: 0
        leftPadding: 0
        rightPadding: 0

        actions: [
             Kirigami.Action {
                visible: initialPage.visible
                text: i18n("Terminal")
                icon.name: "utilities-terminal-symbolic"
                shortcut: "Ctrl+T"
                checkable: true
                checked: terminalLayout.visible
                onTriggered: {
                    terminalLayout.visible = !terminalLayout.visible
                }
            },
            Kirigami.Action{
                visible: initialPage.visible
                text: i18n("New Chat")
                icon.name: "document-new-symbolic"
                onTriggered: {
                    UserPromptField.resetChat()
                }
            },
            Kirigami.Action{
                visible: initialPage.visible
                text: i18n("Speak output")
                checkable: true
                icon.name: "audio-speakers-symbolic"
                onTriggered: {
                }
            },
            Kirigami.Action {
                text: i18n("Settings")
                icon.name: "preferences-system-symbolic"
                onTriggered: {
                    // initialPage.visible = false
                    pageStack.push(settingsPage)
                }
            }
        ]
        Controls.SplitView{
            orientation: splitTerminalVertical ? Qt.Horizontal : Qt.Vertical
            anchors.fill: parent
            anchors.margins: Kirigami.Units.gridUnit
            ColumnLayout{
                width: parent.width
                height: parent.height
                ChatHistory{
                    id: chatHistory
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
                RowLayout{
                    Layout.fillWidth: true
                    enabled: UserPromptField.enableChatField
                    Controls.TextArea{
                        Layout.alignment: Qt.AlignBottom
                        Layout.fillWidth: true
                        id: chatField
                        Kirigami.SpellCheck.enabled : false
                        placeholderText: UserPromptField.enableChatField ? i18n("What do you want to do?") : i18n("Please wait for response...")
                        // focusSequence: StandardKey.Find
                        wrapMode: TextEdit.Wrap
                    }
                    Controls.Button{
                        icon.name: "go-next"
                        id: sendButton
                        onClicked: {
                            if(chatField.text.length > 0){
                                UserPromptField.handleTextChange(chatField.text)
                                chatField.text = ""
                            }
                        }
                    }
                    Controls.Button{
                        icon.name: "audio-input-microphone-symbolic"
                        checkable: true
                        Controls.ToolTip {
                            text: "Enable dictation"
                        }
                        // checked: 
                        onClicked: {
                            // UserPromptField.handleTextChange(chatField.text)
                            // chatField.text = ""
                        }
                    }
                    Controls.Button{
                        icon.name: "spectacle"
                        checkable: true
                        Controls.ToolTip {
                            text: "Let model see your screen"
                        }
                        onClicked: {
                            // UserPromptField.handleTextChange(chatField.text)
                            // chatField.text = ""
                        }
                    }
                }
                
            }
            TerminalLayout{
                id: terminalLayout
                visible: false
                Layout.fillWidth: true
                Layout.fillHeight: true
                onHeightChanged: {
                    if(terminalLayout.height < 10){
                        terminalLayout.visible = !terminalLayout.visible
                    }
                }
            }
        }
    }
   
}