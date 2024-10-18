import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.koolintelligence 1.0
import org.kde.koolintelligence.components 1.0
import org.kde.konsoleqml
Kirigami.ScrollablePage {
    id: settingsPage
    title: i18nc("@title", "Settings")
    visible: false
    actions: [
        Kirigami.Action {
            text: i18n("Close")
            icon.name: "window-close-symbolic"
            onTriggered: {
                pageStack.pop()
            }
        }
    ]
    ColumnLayout{
        width: parent.width
        height: parent.height
        RowLayout{
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Heading {
                text: i18n("General Settings")
                level: 1       
            }
            Kirigami.Separator {
                Layout.fillWidth: true
            }
        }
        Controls.CheckBox {
            Layout.alignment: Qt.AlignHCenter
            text: i18n("Enable screenshots")
            checked: true
            onCheckedChanged: {
            }
        }
        Controls.CheckBox {
            Layout.alignment: Qt.AlignHCenter
            text: i18n("Enable file search and indexing results")
            checked: true
            onCheckedChanged: {
            }
        }
        RowLayout{
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Heading {
                text: i18n("Model Settings")
                level: 1       
            }
            Kirigami.Separator {
                Layout.fillWidth: true
            }
        }
        RowLayout{
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Heading {
                Layout.alignment: Qt.AlignHCenter
                text: i18n("System prompt: ")
                level: 5
            }
            Controls.TextArea {
                id: field
                Layout.fillWidth: true
                Layout.fillHeight: true
                Kirigami.SpellCheck.enabled : true
                text: "Model settings"
                wrapMode: TextEdit.Wrap
            }
        }

        Controls.ComboBox {
            Layout.alignment: Qt.AlignHCenter
            id: deviceComboBox
            model: ["CPU", "OpenVino", "GPU"]
            textRole: "Device to run model on"
            onCurrentIndexChanged: {
            }
        }
        Controls.ComboBox {
            Layout.alignment: Qt.AlignHCenter
            id: modelComboBox
            model: ["LLama", "GPT-2", "GPT-3"]
            textRole: "LLM model to run"
            onCurrentIndexChanged: {
            }
        }
        RowLayout{
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Heading {
                text: i18n("Chat Settings")
                level: 1       
            }
            Kirigami.Separator {
                Layout.fillWidth: true
            }
        }
        Controls.CheckBox {
            Layout.alignment: Qt.AlignHCenter
            text: i18n("Save chat history")
            checked: false
            onCheckedChanged: {
            }
        }
        Controls.CheckBox {
            Layout.alignment: Qt.AlignHCenter
            text: i18n("Restore chat on startup")
            checked: false
            onCheckedChanged: {
            }
        }
        RowLayout{
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Heading {
                text: i18n("Terminal Settings")
                level: 1       
            }
            Kirigami.Separator {
                Layout.fillWidth: true
            }
        }
        RowLayout{
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Heading {
                text: i18n("Align Terminal: ")
                level: 5
            }
            Controls.ComboBox {
                id: splitTerminalComboBox
                Layout.alignment: Qt.AlignHCenter
                model: ["Horizontal", "Vertical"]
                currentIndex: root.splitTerminalVertical ? 1 : 0
                onCurrentIndexChanged: {
                    if(splitTerminalComboBox.currentIndex == 0){
                        root.splitTerminalVertical = false
                    } else {
                        root.splitTerminalVertical = true
                    }
                }
            }
        }
        RowLayout{
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Heading {
                text: i18n("Color Scheme: ")
                level: 5
            }
            Controls.ComboBox {
                Layout.alignment: Qt.AlignHCenter
                id: terminalColorSchemeComboBox
                model: ["BlackOnLightYellow", 
                "BlackOnRandomLight", 
                "BlackOnWhite", 
                "BreezeModified", 
                "cool-retro-term",
                "DarkPastels",
                "GreenOnBlack",
                "Linux",
                "Solarized",
                "SolarizedLight",
                "Tango",
                "Ubuntu",
                "WhiteOnBlack",
                ]
                onCurrentIndexChanged: {
                    root.terminalColorScheme = terminalColorSchemeComboBox.currentText
                }
            }
        }
        RowLayout{
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Heading {
                text: i18n("Font Size: ")
                level: 5
            }
            Controls.Slider {
                id: fontSizeSlider
                from: 10
                to: 30
                stepSize: 1
                onValueChanged: {
                    // root.terminalTransparency = slider.value
                }
                Layout.fillWidth: true
            }
        }
        RowLayout{
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Heading {
                text: i18n("Background transparency: ")
                level: 5
            }
            Controls.Slider {
                id: terminalTransparencySlider
                Layout.fillWidth: true
                from: 1
                to: 10
                stepSize: 1
                onValueChanged: {
                    // root.terminalTransparency = slider.value
                }
            }
        }
    }
}