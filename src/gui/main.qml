import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    id: window
    visible: true
    width: 800
    height: 480
    title: qsTr("GDRUM")

    FontLoader
    {
       id: ubuntu
       source:"qrc:/resources/fonts/ubuntu.ttf"
    }

    Loader {
            id: pageLoader
            anchors.fill: parent
            source: "qrc:/src/gui/pages/SplashScreen.qml"
        }
}
