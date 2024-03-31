import QtQuick 2.15


    Rectangle {
        id: signalContainer
        width: 20
        height: 20
        color: "#2C422B" 
        radius: width / 2

        Timer {
            id: sleepTimer
            interval: 200 
            repeat: false 
            onTriggered: {
                signalContainer.color = "#2C422B"
            }
        }


          function blink() {
            signalContainer.color = "#27dc28"
            sleepTimer.start();

        }
    }
  


