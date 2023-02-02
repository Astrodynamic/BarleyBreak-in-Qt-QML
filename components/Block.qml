import QtQuick
import QtQuick.Controls

Rectangle {
  id: root

  property string displayText: ""

  color: "lightgreen"

  border.color: "black"
  border.width: 1

  radius: 10

  Text {
    id: _block_text
    anchors.centerIn: root
    text: root.displayText

    font {
      pointSize: Math.min(root.width, root.height) / 3
      bold: true
    }
  }
}
