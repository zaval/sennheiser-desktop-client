import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Rectangle {
    id: deviceInfoRoot
    width: 400
    height: 150
    radius: 20
    color: deviceInfoRoot.palette.window
    border.color: deviceInfoRoot.palette.mid
    property string iconName: "qrc:/icons/battery.0percent.svg"

    DeviceInfoCardHelper {
        id: deviceInfoHelper
        onBatteryPercentChanged: function(){
            if (deviceInfoHelper.batteryPercent.length < 1){
                return;
            }
            if (deviceInfoHelper.batteryPercent > 75){
                deviceInfoRoot.iconName = "qrc:/icons/battery.100percent.svg"
            }
            else if (deviceInfoHelper.batteryPercent > 50){
                deviceInfoRoot.iconName = "qrc:/icons/battery.75percent.svg"
            }
            else if (deviceInfoHelper.batteryPercent > 25){
                deviceInfoRoot.iconName = "qrc:/icons/battery.50percent.svg"
            }
            else if (deviceInfoHelper.batteryPercent > 10){
                deviceInfoRoot.iconName = "qrc:/icons/battery.25percent.svg"
            }
        }
    }

    Image {
        id: battery
        source: deviceInfoRoot.iconName
        // color: deviceInfoRoot.palette.windowText
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 10
        anchors.topMargin: 10
        height: 16
        // width: 52
        fillMode: Image.PreserveAspectFit
    }
    Colorize {
        visible: !UiHelper.isDarkTheme
        anchors.fill: battery
        source: battery
        hue: .0
        saturation: 0
        lightness:  -1
    }

    RowLayout {
        anchors.fill: parent
        spacing: 5

        Image {
            id: logo
            Layout.fillHeight: true

            // width: 100
            // height: 100
            source: "qrc:/icons/m4.png"
            Layout.maximumHeight: 100
            Layout.maximumWidth: 100
            sourceSize.height: 100
            sourceSize.width: 100
            fillMode: Image.PreserveAspectFit
        }
        ColumnLayout{
            // Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            spacing: 10
            Layout.fillWidth: true

            Label {
                id: bigName
                color: deviceInfoRoot.palette.windowText

                text: deviceInfoHelper.deviceName
                verticalAlignment: Text.AlignTop
                font.bold: true
                font.pointSize: 24
            }
            Label {
                id: macAddress
                color: deviceInfoRoot.palette.windowText
                text: "(" + deviceInfoHelper.deviceAddress + ")"
            }
            RowLayout {
                Layout.fillWidth: true

                Label {
                    id: version
                    text: qsTr("Version: ") + deviceInfoHelper.releaseVersion

                    color: deviceInfoRoot.palette.windowText
                }
                Label {
                    id: modelId
                    text: qsTr("Model: ") + deviceInfoHelper.modelId

                    color: deviceInfoRoot.palette.windowText
                }
            }
            Label {
                id: serialNumber
                text: `Serial: ${deviceInfoHelper.serialNumber}`
                color: deviceInfoRoot.palette.windowText
            }
        }
        Item {
            Layout.fillWidth: true
        }
    }






    // Image {
    //     id: logo
    //     width: 100
    //     height: 100
    //     anchors.left: parent.left
    //     anchors.top: parent.top
    //     anchors.leftMargin: 10
    //     anchors.topMargin: 10
    //     source: "qrc:/icons/m4.png"
    //     fillMode: Image.PreserveAspectFit
    // }
    //
    // Label {
    //     id: bigName
    //     color: deviceInfoRoot.palette.windowText
    //
    //     text: deviceInfoHelper.deviceName
    //     anchors.left: logo.right
    //     anchors.top: parent.top
    //     anchors.leftMargin: 10
    //     anchors.topMargin: 10
    //     verticalAlignment: Text.AlignTop
    //     font.bold: true
    //     font.pointSize: 24
    // }
    //
    // Label {
    //     id: macAddress
    //     color: deviceInfoRoot.palette.windowText
    //     text: "(" + deviceInfoHelper.deviceAddress + ")"
    //     anchors.left: logo.right
    //     anchors.top: bigName.bottom
    //     anchors.leftMargin: 10
    //     anchors.topMargin: 10
    // }
    //
    // Image {
    //     id: battery
    //     source: deviceInfoRoot.iconName
    //     // color: deviceInfoRoot.palette.windowText
    //     anchors.right: parent.right
    //     anchors.top: parent.top
    //     anchors.rightMargin: 10
    //     anchors.topMargin: 10
    //     height: 16
    //     // width: 52
    //     fillMode: Image.PreserveAspectFit
    // }
    // Colorize {
    //     visible: !deviceInfoRoot.isDarkTheme
    //     anchors.fill: battery
    //     source: battery
    //     hue: .0
    //     saturation: 0
    //     lightness:  -1
    // }
    // Label {
    //     id: version
    //     text: qsTr("Version: ") + deviceInfoHelper.releaseVersion
    //     anchors.left: logo.right
    //     anchors.top: macAddress.bottom
    //     anchors.leftMargin: 10
    //     anchors.topMargin: 10
    //     color: deviceInfoRoot.palette.windowText
    // }
    // Label {
    //     id: modelId
    //     text: qsTr("Model: ") + deviceInfoHelper.modelId
    //     anchors.left: version.right
    //     anchors.top: macAddress.bottom
    //     anchors.leftMargin: 10
    //     anchors.topMargin: 10
    //     color: deviceInfoRoot.palette.windowText
    // }
    // Label {
    //     id: serialNumber
    //     text: `Serial: ${deviceInfoHelper.serialNumber}`
    //     anchors.left: logo.right
    //     anchors.top: version.bottom
    //     anchors.leftMargin: 10
    //     anchors.topMargin: 10
    //     color: deviceInfoRoot.palette.windowText
    // }
}
