import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Rectangle {
    id: root
    width: 400
    height: 310
    radius: 20
    border.color: root.palette.mid
    clip: true
    color: root.palette.window

    ConnectedDevicesCardHelper {
        id: helper
    }

    Image {
        id: linkIcon
        source: "qrc:/icons/display.2.svg"
        // color: deviceInfoRoot.palette.windowText
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 10
        anchors.topMargin: 10
        width: 32
        fillMode: Image.PreserveAspectFit
    }
    Colorize {
        visible: !UiHelper.isDarkTheme
        anchors.fill: linkIcon
        source: linkIcon
        hue: .0
        saturation: 0
        lightness:  -1
    }

    Label {
        id: devicesLabel
        text: `Paired ${helper.totalPaired} devices. Max online: ${helper.maxConnections}`
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 10
        anchors.topMargin: 10
        font.pointSize: 20
    }

    ScrollView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: devicesLabel.bottom
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 20
        anchors.bottomMargin: 10
        ListView {
            id: devicesList
            spacing: 10
            model: helper.model

            delegate: RowLayout {
                width: devicesList.width
                // anchors.left: devicesList.left
                // anchors.right: devicesList.right
                required property string name;
                required property bool connected;
                required property int deviceIndex;
                id: rl
                Switch {
                    checked: rl.connected
                    Layout.fillWidth: true
                    text: rl.name + (rl.deviceIndex === helper.ownIndex ? " (current device)" : "")
                    onClicked: {
                        rl.connected ? helper.disconnectDevice(rl.deviceIndex) : helper.connectDevice(rl.deviceIndex)
                    }
                }
                Button {
                    flat: true
                    icon.source: "qrc:/icons/trash.svg"
                    icon.color: "#b50927"
                    onClicked: {
                        // helper.deleteDevice(rm.deviceIndex)
                        confirmDeleteDialog.deviceIndex = rl.deviceIndex;
                        confirmDeleteDialog.deviceName = rl.name;
                        confirmDeleteDialog.open();
                    }
                }
            }
        }
    }

    Dialog{
        id: confirmDeleteDialog
        title: "Delete device"
        property string deviceName: ""
        property int deviceIndex: -1


        standardButtons: Dialog.Yes | Dialog.No
        modal: true
        Overlay.modal: Rectangle {
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

        contentItem: Column {
            anchors.fill: parent
            anchors.topMargin: 20
            spacing: 10
            padding: 20
            Label {
                id: messageText
                text: `Are you sure you want to delete device ${confirmDeleteDialog.deviceName}?`;
            }
        }

        onAccepted: {
            if (confirmDeleteDialog.deviceIndex === -1){
                return;
            }
            helper.deleteDevice(confirmDeleteDialog.deviceIndex);
            confirmDeleteDialog.deviceName = "";
            confirmDeleteDialog.deviceIndex = -1;
        }
        onClosed: {
            confirmDeleteDialog.deviceName = "";
            confirmDeleteDialog.deviceIndex = -1;
        }



    }



}