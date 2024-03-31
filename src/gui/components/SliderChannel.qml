import QtQuick 2.15
import QtQuick.Controls 2.15

Slider {
    id: customSlider
    orientation: Qt.Vertical
    from: 0
    to: 100
    value: 50 // Valor inicial
    height: 99
    width: 12

    property int channelId: -1

    background: Rectangle {
        implicitWidth: 12
        implicitHeight: 100
        color: "#d9d9d9"
        radius: 10
        anchors.fill: parent
    }

    handle: Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
                // Ajuste simplificado para la posición Y
            y: (customSlider.height - height) - ((value - from) / (to - from) * (customSlider.height - height))
            width: 20
            height: 20
            color: "#fef1d0"
            radius: 10
        }


    onValueChanged: {
        readSensors.setVolume(channelId, Math.round(value));
    }
}

