import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import QtCore
import gaiaV3

Page {
    id: connectPage

    Layout.fillWidth: true
    Layout.fillHeight: true
    // anchors.fill: parent

    Component.onCompleted: {
        // popup.open();
        if (permission.status === Qt.PermissionStatus.Undetermined)
            permission.request();
        else if (permission.status === Qt.PermissionStatus.Granted)
            bluetoothHandler.startDeviceDiscovery();
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 10

        Label {
            Layout.alignment: Qt.AlignCenter
            text: "Loading Bluetooth Devices"
        }
        BusyIndicator {
            Layout.alignment: Qt.AlignCenter
            visible: bluetoothHandler.isSearching
        }
        Button {
            Layout.alignment: Qt.AlignCenter
            visible: !bluetoothHandler.isSearching
            text: "Scan"
            onClicked: {
                bluetoothHandler.startDeviceDiscovery();
            }
        }
    }
    BluetoothHandler {
        id: bluetoothHandler

        onIsSearchingChanged: function (val) {
            if (!bluetoothHandler.isSearching && bluetoothHandler.deviceAddress === "00:00:00:00:00:00") {
                if (AppSettings.deviceAddress === "") {
                    popup.open();
                } else {
                    bluetoothHandler.deviceAddress = AppSettings.deviceAddress;
                }

            }
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

        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        focus: true
        width: 300
        height: 200
        modal: true

        title: "Address not found"
        standardButtons: Dialog.Save | Dialog.Cancel


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

        ColumnLayout {
            anchors.fill: parent

            Label {
                text: "I found the device but can't find it's address.\nIf you know the address, fill it here"
            }

            TextField {
                Layout.fillWidth: true
                id: deviceAddress
                placeholderText: "xx:xx:xx:xx:xx:xx"
                inputMask: "HH:HH:HH:HH:HH:HH"
            }
        }

        onAccepted: {
            AppSettings.deviceAddress = deviceAddress.text
            bluetoothHandler.deviceAddress = deviceAddress.text
        }

    }

}
