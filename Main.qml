import QtQuick.Controls
import QtQuick
import btuicontrols
import gaiaV3


import QtQuick.Layouts

import QtQuick
// import QtCore

ApplicationWindow {
    id: mainWindow
    width: 1000
    height: 600
    title: qsTr("Smart Control Desktop")
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
                // onClicked: stackView.currentIndex -= 1;
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
        onIsConnectedChanged: {
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


    Dialog {
        id: aboutDialog
        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        focus: true
        width: 350
        height: 250
        standardButtons: Dialog.Close
        modal: true
        Overlay.modal: Rectangle {
            // color: Qt.transparent(mainWindow.palette.window, 0.1)
            color: "#55888888"
        }

        enter: Transition {
            NumberAnimation {
                from: 0.0
                property: "opacity"
                to: 1.0
            }
        }
        exit: Transition {
            NumberAnimation {
                from: 1.0
                property: "opacity"
                to: 0.0
            }
        }

        title: "About"

        Rectangle {
            id: root
            anchors.fill: parent
            color: root.palette.window
            clip: true

            RowLayout {
                id: rowLayout
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                anchors.topMargin: 20
                anchors.bottomMargin: 20
                spacing: 20



                ColumnLayout {
                    Layout.fillHeight: true
                    Image {
                        id: appicon
                        sourceSize.height: 75
                        sourceSize.width: 75
                        source: "qrc:/appicon.png"
                        fillMode: Image.PreserveAspectFit
                    }
                    Item {
                        Layout.fillHeight: true
                    }
                }
                ColumnLayout {
                    Layout.fillWidth: true
                    Label {
                        color: root.palette.windowText
                        text: "Smart Desktop Client"
                        font.bold: true
                        font.pointSize: 16
                    }
                    Label {
                        Layout.fillWidth: true
                        color: root.palette.windowText
                        wrapMode: Text.WordWrap
                        text: `${projectDescription}`
                    }
                    Label {
                        color: root.palette.windowText
                        text: `version: ${projectVersionMajor}.${projectVersionMinor}.${projectVersionPatch}`
                    }
                    Label {
                        color: root.palette.windowText
                        text: "author: Dmytrii Zavalnyi"
                    }
                    Label {
                        textFormat: Text.RichText
                        color: root.palette.windowText
                        text: `<a href="${projectUrl}">${projectUrl}</a>`
                        onLinkActivated: {
                            Qt.openUrlExternally(projectUrl);
                        }
                    }

                    Item {
                        Layout.fillHeight: true
                    }

                }
                Item {
                    Layout.fillWidth: true
                }
            }
        }

    }
}