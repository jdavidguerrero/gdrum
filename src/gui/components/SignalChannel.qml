import QtQuick 2.15


Rectangle {
    id: signalChannel
    width: 20
    height: 20
    color: "#dc2727" // Color inicial
    radius: width / 2

    signal changeColor(string newColor) // Define una señal personalizada

    onChangeColor: {
        color = newColor // Cambia el color cuando se emite la señal
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            signalChannel.changeColor("#27dc28") // Emite la señal con un nuevo color
        }
    }
}
