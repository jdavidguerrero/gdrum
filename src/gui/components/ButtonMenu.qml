import QtQuick 2.15

Rectangle {
    id: buttonMenu
    width: 164
    height: 36
    color: "transparent"
    property string buttonText: "Button"

    Rectangle {
        id: button_background
        color: "#e7e5da"
        radius: 10
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }

    Text {
        id: button_text
        color: "#000000"
        text: qsTr(buttonMenu.buttonText)
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
        wrapMode: Text.Wrap
        anchors.leftMargin: 17
        font.weight: Font.Bold
        anchors.topMargin: 6
        anchors.bottomMargin: 6
        anchors.rightMargin: 16
        font.family: "Ubuntu"
    }
}
