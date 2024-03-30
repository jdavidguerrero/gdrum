import QtQuick 2.15

Rectangle {
    id: waveContainer
    width: 330
    height: 90
    color: "transparent"

    Item {
        id: waveImage
        x: 0
        y: 0
        width: 330
        height: 90
        Rectangle {
            id: frame_1
            color: "#00ffffff"
            radius: 20
            border.color: "#d9d9d9"
            border.width: 6
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            clip: true
            Image {
                id: image_6
                anchors.left: parent.left
                anchors.top: parent.top
                source: "qrc:/resources/images/wave.png"
            }
        }
    }
}
