import QtQuick 2.15

Rectangle {
    id: channel
    width: 110
    height: 260
    color: "transparent"

    Rectangle {
        id: container_channel
        color: "#000e1a18"
        radius: 20
        border.color: "#dfdfdf"
        border.width: 1
        anchors.fill: parent
    }

    Image {
        id: drum_image
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        source: "qrc:/resources/images/djembe.png"
        anchors.leftMargin: 12
        anchors.topMargin: 144
        anchors.bottomMargin: 9
        anchors.rightMargin: 9
    }

    SliderChannel {
        id: sliderChannel
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 47
        anchors.topMargin: 41
        anchors.bottomMargin: 120
        anchors.rightMargin: 47
    }

    SignalChannel {
        id: signal_channel
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 45
        anchors.topMargin: 11
        anchors.bottomMargin: 229
        anchors.rightMargin: 45
    }



}
