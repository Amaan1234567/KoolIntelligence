import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import org.kde.konsoleqml
import org.kde.kirigami as Kirigami
import org.kde.koolintelligence 1.0

ColumnLayout {
    id: terminalView
    property TerminalEmulator currentTerminal: tabSwipeView.contentChildren[tabSwipeView.currentIndex].termWidget
    function forceTerminalFocus(forceInput) {
        const wasVisible = Qt.inputMethod.visible;
        currentTerminal.forceActiveFocus();
        if (forceInput) {
            Qt.inputMethod.show();
        } else if (!wasVisible) {
            Qt.inputMethod.hide();
        }
    }
    spacing: 0
    // anchors.fill: parent
    Layout.fillWidth: true
    Layout.fillHeight: true

    SwipeView {
        id: tabSwipeView

        Layout.fillWidth: true
        Layout.fillHeight: true

        onCurrentItemChanged: currentTerminal.forceActiveFocus()

        Repeater {
            id: terminalRepeater
            model: TerminalTabModel

            delegate: Item {
                property alias termWidget: terminal

                TerminalEmulator {
                    id: terminal
                    anchors.fill: parent

                    readonly property string tabName: model.name
                    readonly property int modelIndex: model.index
                    readonly property bool isCurrentItem: SwipeView.isCurrentItem

                    //TODO: Impment a way for user to change the customization of the terminal
                    font.family: "Monospace"
                    font.pixelSize: 12
                    colorScheme: "cool-retro-term"

                    Component.onCompleted: {

                        mainsession.startShellProgram();
                    }

                    function pressKey(key, modifiers, pressed, nativeScanCode, text) {
                        terminal.simulateKeyPress(key, modifiers, pressed, nativeScanCode, text);
                    }

                    session: TerminalSession {
                        id: mainsession
                        initialWorkingDirectory: "$HOME"
                    }

                    ScrollBar {
                        Kirigami.Theme.colorSet: Kirigami.Theme.Complementary // text color of terminal is also complementary
                        Kirigami.Theme.inherit: false
                        anchors {
                            right: parent.right
                            top: parent.top
                            bottom: parent.bottom
                        }
                        visible: true
                        orientation: Qt.Vertical
                        size: (terminal.lines / (terminal.lines + terminal.scrollbarMaximum - terminal.scrollbarMinimum))
                        position: terminal.scrollbarCurrentValue / (terminal.lines + terminal.scrollbarMaximum)
                        interactive: false
                    }

                    TapHandler {
                        cursorShape: Qt.IBeamCursor
                        onTapped: terminalView.forceTerminalFocus(true);
                    }
                }
            }
        }
    }
}
