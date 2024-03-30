import QtQuick 2.15

Rectangle {
    id: buttonIcon
    width: 78
    height: 68
    color: "transparent"

    property string iconSource: "qrc:/resources/images/settings.png"

    Rectangle {
        id: bkg_button
        color: "#d9d9d9"
        radius: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

    Image {
            id: icon
            source: buttonIcon.iconSource
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 40
            height: 40
        }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            console.log("Botón clickeado");
        }
    }

}
