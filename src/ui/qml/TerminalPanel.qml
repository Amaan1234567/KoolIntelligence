import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: terminalPanel
    width: parent.width
    height: 300  // Set a default height or adjust as necessary

    ColumnLayout {
        anchors.fill: parent

        // Terminal Output Display
        TextArea {
            id: outputArea
            Layout.fillHeight: true
            readOnly: true  // Make it read-only
            wrapMode: TextArea.Wrap
            placeholderText: "Terminal output will appear here..."
            font.pointSize: 10
            textFormat: TextArea.MarkdownText
        }

        // User Input Command Field
        ActionTextField {
            id: commandInput
            Layout.alignment: Qt.AlignBottom
            placeholderText: "Enter command..."
            onAccepted: {
                // Handle command execution
                terminalPanel.executeCommand(text);
                text = "";  // Clear the input after sending
            }
        }
    }

    // Function to execute command and show output
    function executeCommand(command) {
        // Simulate terminal command execution
        // Here you can call your actual command execution logic
        // For demonstration, we just append the command to the output area
        outputArea.text += "> " + command + "\n";
        
        // You can replace the above line with the actual execution logic and output retrieval
        // e.g., calling a C++ function and getting the result.
        
        // Example output
        outputArea.text += "Executed: " + command + "\n";  // Placeholder output
    }

    // Signal to receive output (optional)
    signal outputReceived(string output)

    // This could be called from C++ to append output to the terminal
    function appendOutput(output) {
        outputArea.text += output + "\n";
        outputReceived(output);  // Emit signal if needed
    }
}
