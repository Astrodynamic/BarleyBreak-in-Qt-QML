import QtQuick
import QtQuick.Controls

import Game 1.0

GridView {
  id: root

  model: GameBoardModel {}

  cellHeight: height / root.model.dimension
  cellWidth: width / root.model.dimension

  delegate: Item {
    id: _background_delegate

    width: root.cellWidth
    height: root.cellHeight

    visible: display != root.model.board_size

    Block {
      displayText: display
      anchors.fill: _background_delegate
      anchors.margins: 5

      MouseArea {
        anchors.fill: parent
        onClicked: {
          root.model.move(index)
        }
      }
    }
  }
}
