import QtQuick
import QtQuick.Controls

ApplicationWindow {
  id: root
  width: 640
  height: 480
  visible: true
  title: qsTr("Barley-Break")

  GameBoard {
    anchors.fill: parent
  }
}
