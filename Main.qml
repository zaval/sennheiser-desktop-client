import QtQuick.Controls
import QtQuick
import btuicontrols
import gaiaV3

import QtCore
import QtQuick.Layouts

import QtQuick

ApplicationWindow {
    id: mainWindow
    width: 1000
    height: 600
    title: "Smart Control Desktop"
    visible: true

    header: ToolBar {
        height: 30
        RowLayout {
            anchors.fill: parent
            spacing: 5
            ToolButton {
                flat: true
                visible: stackView.currentIndex > 0;
                icon.source: "qrc:/icons/arrow.left.svg"
                icon.color: mainWindow.palette.windowText
                onClicked: stackView.currentIndex -= 1;
            }
            Item {
                Layout.fillWidth: true
            }
            ToolButton {
                flat: true
                icon.source: "qrc:/icons/gearshape.svg"
                icon.color: mainWindow.palette.windowText
                onClicked: settingsDialog.open();
            }
            ToolButton {
                flat: true
                icon.source: "qrc:/icons/questionmark.circle.svg"
                icon.color: mainWindow.palette.windowText
                onClicked: aboutDialog.open();
            }
        }
    }


    Connections {
        target: GAIARfcommClient
        function onIsConnectedChanged(){
            stackView.currentIndex = GAIARfcommClient.isConnected ? 1 : 0;
        }
    }

    StackLayout {
        id: stackView

        anchors.fill: parent

        currentIndex: GAIARfcommClient.isConnected ? 1 : 0

        onCurrentIndexChanged: {
            if (stackView.currentIndex === 1)
                pageLoader.source = "qrc:/qt/qml/btuicontrols/DevicePage.qml";
            else
            {
                pageLoader.source = "";
                deviceListPage.bluetoothHandler.deviceAddress = "00:00:00:00:00:00";
            }
        }

        DeviceListPage{
            id: deviceListPage
        }
        Loader {
            id: pageLoader
        }
    }

    AboutDialog {
        id: aboutDialog
    }
    SettingsDialog {
        id: settingsDialog
        onClosed: {
            deviceListPage.loadDevices();
        }
    }

    Settings {
        property alias x: mainWindow.x
        property alias y: mainWindow.y
        property alias width: mainWindow.width
        property alias height: mainWindow.height
    }
}