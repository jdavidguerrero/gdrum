import QtQuick 2.15
import QtQuick.Shapes 1.0
import "../components"

    Rectangle {
        id: home
        width: 800
        height: 480
        color: "#ffffff"
        property var channelComponents: [];
        Item {
            id: borderOne
            x: 0
            y: 0
            width: 800
            height: 480
            Rectangle {
                id: rectangle_46
                width: 800
                height: 480
                color: "#3C2A1F"
                anchors.left: parent.left
                anchors.top: parent.top
            }

            Rectangle {
                id: borderTwo
                width: 780
                height: 460
                color: "#000000"
                radius: 20
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 10
                anchors.topMargin: 10
            }

            Rectangle {
                id: content
                width: 740
                height: 440
                color: "#0b1919"
                radius: 20
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 30
                anchors.topMargin: 20
            }


        }

        TitleBar {
            id: titleBar_instance
            title: "Sample Mode"
            width: 599
            height: 35
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 102
            anchors.topMargin: 33
        }

        WaveContainer {
            id: waveContainer
            width: 330
            height: 90
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 105
            anchors.topMargin: 88
        }

        ButtonMenu {
            id: buttonMidi
            buttonText: "MIDI"
            width: 164
            height: 36
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 446
            anchors.topMargin: 90
        }
        ButtonMenu {
            id: buttonEffects
            buttonText: "Effects"
            width: 164
            height: 36
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 446
            anchors.topMargin: 141
        }
        ButtonIcon {
            id: settings_button
            width: 78
            height: 68
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 621
            anchors.topMargin: 99
        }

        ChannelSample {
            id: channel0
            channelId:0
            width: 110
            height: 260
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 101
            anchors.topMargin: 195
        }

        ChannelSample {
            id: channel1
            channelId:1
            width: 110
            height: 260
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 223
            anchors.topMargin: 195
        }



        ChannelSample {
            id: channel2
            channelId:2
            width: 110
            height: 260
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 345
            anchors.topMargin: 195
        }

        ChannelSample {
            id: channel3
            channelId:3
            width: 110
            height: 260
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 467
            anchors.topMargin: 195
        }

        ChannelSample {
            id: channel4
            channelId:4
            width: 110
            height: 260
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 589
            anchors.topMargin: 195
        }

       Component.onCompleted: {
    channelComponents = [channel0, channel1, channel2, channel3, channel4];
}

    Connections {
    target: readSensors
    function onThresholdExceeded(channel) {
    if (channel >= 0 && channel < channelComponents.length) {
        channelComponents[channel].blinkSignalChannel();
      
    }
    }

}



    }
 
