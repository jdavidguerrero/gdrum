import QtQuick 2.15


Rectangle {
    id: channelSample
    width: 110
    height: 260
    color: "transparent"
    property int channelId: -1

    Rectangle {
        color: "#000e1a18"
        radius: 20
        border.color: "#dfdfdf"
        border.width: 1
        anchors.fill: parent
    }

    Image {
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
        channelId: channelSample.channelId
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
        id: signalChannel
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 45
        anchors.topMargin: 11
        anchors.bottomMargin: 229
        anchors.rightMargin: 45
    }

    function blinkSignalChannel() {
        signalChannel.blink();
    }



}
