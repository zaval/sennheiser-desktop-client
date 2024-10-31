import QtQuick.Controls
import QtQuick.Layouts
import QtCore
import QtQuick
import btuicontrols
import QtQuick.LocalStorage
import "Database.js" as DB
import gaiaV3


Page {
    id: deviceListPage
    property alias bluetoothHandler: bluetoothHandler

    Layout.fillWidth: true
    Layout.fillHeight: true

    function loadDevices(){
        listModel.clear();
        DB.execute( 'SELECT * FROM devices;', (result) => {
            for (let i = 0; i < result.rows.length; i++) {
                listModel.append(
                    {
                        name: result.rows.item(i).name,
                        address: result.rows.item(i).address,
                        isOnline: result.rows.item(i).address === "11:11:11:11:11:11",
                        uuid: result.rows.item(i).uuid
                    }
                );
            }
        });
    }

    Component.onCompleted: {
        // popup.open();
        if (permission.status === Qt.PermissionStatus.Undetermined)
            permission.request();
        else if (permission.status === Qt.PermissionStatus.Granted)
            bluetoothHandler.startDeviceDiscovery();

        loadDevices();
    }

    Item {
        id: row
        width: 350
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        // spacing: 5
        visible: bluetoothHandler.isSearching


        Label {
            id: loadingLabel
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Loading Bluetooth Devices"
        }
        BusyIndicator {
            visible: bluetoothHandler.isSearching
            anchors.left: loadingLabel.right
            anchors.leftMargin: 5
        }
    }


    ScrollView {
        width: 370
        height: deviceListPage.height * .60

        anchors.centerIn: parent
        ListView {
            id: listView
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            spacing: 10
            model: listModel

            delegate: FoundDeviceCard {
                id: delegate
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true

                    onClicked: {
                        if (!delegate.isOnline) {
                            return;
                        }

                        if (delegate.address === "00:00:00:00:00:00") {
                            popup.uuid = delegate.uuid;
                            popup.open();
                        } else {
                            bluetoothHandler.deviceAddress = delegate.address;
                            GAIARfcommClient.deviceName = delegate.name;
                        }
                    }

                    onEntered: {
                        delegate.border.color = delegate.palette.highlight;
                    }
                    onExited: {
                        delegate.border.color = delegate.palette.mid;
                    }
                }
            }
        }
    }

    ListModel {
        id: listModel
    }

    BluetoothHandler {
        id: bluetoothHandler

        onNewDeviceAdded: function(name, address, uuid) {

            DB.executeWithParams('SELECT * FROM devices WHERE uuid = ?;', [uuid], (result) => {
                if (result.rows.length === 0) {
                    DB.executeWithParams('INSERT INTO devices (name, address, uuid) VALUES (?, ?, ?);', [name, address, uuid]);
                    listModel.append({name: name, address: address, isOnline: true});
                } else {
                    let uuid = result.rows[0].uuid;
                    for (let i = 0; i < listModel.count; i++){
                        if (listModel.get(i).uuid === uuid) {
                            listModel.setProperty(i, "isOnline", true);
                            break;
                        }
                    }
                }
            });

            // listModel.append({name: name, address: address, isOnline: true});
        }
    }
    BluetoothPermission {
        id: permission

        communicationModes: BluetoothPermission.Access

        onStatusChanged: {
            if (permission.status === Qt.PermissionStatus.Denied)
                // Device.update = "Bluetooth permission required";
                console.log("Bluetooth permission required");
            else if (permission.status === Qt.PermissionStatus.Granted)
                bluetoothHandler.startDeviceDiscovery();
        }
    }
    Dialog {
        id: popup

        property string uuid: ""

        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        focus: true
        width: 350
        height: 200
        modal: true

        // parent: Overlay.overlay

        title: "Address not found"
        standardButtons: Dialog.Save | Dialog.Cancel

        enter: Transition {
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
        }

        exit: Transition {
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
        }


        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10
            Label {
                text: "Device's address was not found,\nplease type the MAC address."
            }
            TextField {
                Layout.fillWidth: true
                id: deviceAddress
                placeholderText: "xx:xx:xx:xx:xx:xx"
                inputMask: "HH:HH:HH:HH:HH:HH"
            }
        }

        onAccepted: function() {
            DB.executeWithParams('UPDATE devices SET address = ? WHERE uuid = ?', [deviceAddress.text, popup.uuid]);
            bluetoothHandler.deviceAddress = deviceAddress.text
        }

    }
}