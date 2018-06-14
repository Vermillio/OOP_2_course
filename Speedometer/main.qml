import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4



Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Speedometer")
    objectName: "mainWindow"


    property alias gpsLabelTextProperty: gpsLabel.text

    Label {
        visible: true
        text: "No data received from GPS."
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        id: gpsLabel
    }
}
