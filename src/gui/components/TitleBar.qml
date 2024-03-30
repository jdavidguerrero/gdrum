import QtQuick 2.15

Rectangle {
    id: titleBar
    width: 599
    height: 35
    color: "transparent"
    property string title: "GDRUM"

    Rectangle {
        id: bkg_title_bar
        color: "#2f322f"
        radius: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }

    Text {
        id: title_text
        color: "#ffffff"
        text: qsTr(titleBar.title)
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
        wrapMode: Text.Wrap
        anchors.leftMargin: 235
        font.weight: Font.Bold
        anchors.topMargin: 2
        anchors.bottomMargin: 3
        anchors.rightMargin: 234
        font.family: "Ubuntu"
    }
}
