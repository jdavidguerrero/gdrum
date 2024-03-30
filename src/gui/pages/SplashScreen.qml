import QtQuick 2.15
import QtQuick.Controls 2.15
import "../components"


    Rectangle {
        id: splashScreen
        width: 800
        height: 480

        Image {
            id: splashBackground
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            source: "qrc:/resources/images/background.png"

            Image {
                id: logo
                x: 179
                y: 41
                width: 422
                height: 329
                source: "qrc:/resources/images/logo_gdrum.png"
                fillMode: Image.PreserveAspectFit
            }

            Rectangle {
                id: loadBar
                width: 645
                height: 59
                color: "transparent"
                x: 68
                y: 380
                ProgressBar {
                    id: initialProgressBar
                    width: 645
                    height: 12
                    value: 0
                    from: 0
                    to: 100

                }

                Rectangle {
                    anchors.left: initialProgressBar.left
                    anchors.bottom: initialProgressBar.bottom
                    width: initialProgressBar.width * (initialProgressBar.value / initialProgressBar.to)
                    height: initialProgressBar.height
                    color: "#c7c0b0"
                    radius: 20
                }

                Text {
                    x: 305
                    y: 24
                    anchors {
                        top: initialProgressBar.bottom
                        horizontalCenter: initialProgressBar.horizontalCenter
                        topMargin: 10
                    }
                    font.pixelSize: 20
                    font.weight: Font.Bold
                    font.family: "Ubuntu"
                    color: "#c7c0b0"
                    text: initialProgressBar.value + " %"
                }
            }

            Timer {
                id: progressTimer
                interval: 10 // Carga durante aproximadamente 5 segundos
                repeat: true
                running: true
                onTriggered: {
                    if (initialProgressBar.value < initialProgressBar.to) {
                        initialProgressBar.value += 1; // Incrementa el valor
                    } else {
                        progressTimer.stop();
                        splashScreen.visible = false;
                        pageLoader.source = "qrc:/src/gui/pages/MainScreen.qml"; // Cambia la pantalla a la principal
                    }
                }
            }

        }
    }





