import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCore
import QtQuick.LocalStorage
import "Database.js" as DB

Dialog {
    id: root
    anchors.centerIn: parent
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    focus: true
    title: "Settings"
    leftPadding: 20
    rightMargin: 20
    topPadding: 20
    standardButtons: Dialog.Close
    modal: true
    Overlay.modal: Rectangle {
        // color: "#55888888"
        color: Qt.alpha(root.palette.shadow, 0.5)
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

    function updateDemoDevice(){
        if (demoSwitch.checked){
            DB.execute("select * from devices where address = '11:11:11:11:11:11';", (res) => {
                if (res.rows.length === 0){
                    DB.execute("insert into devices (name, address, uuid) values ('Demo Device', '11:11:11:11:11:11', '1111');")
                }
            });

        } else {
            DB.execute("delete from devices where address = '11:11:11:11:11:11';");
        }
    }

    ColumnLayout{
        anchors.fill: parent
        spacing: 10
        Layout.minimumWidth: 300

        ColumnLayout{
            spacing: 5
            Switch {
                id: demoSwitch
                text: "Enable demo device"
                checked: settings.demoDevice
                onClicked: {
                    updateDemoDevice();
                }
            }
            Label {
                text: "Active demo device in the list.\nYou can use this device to explore the app if no other devices are available"
                Layout.leftMargin: 50
                Layout.rightMargin: 20
                Layout.fillWidth: true
                font.pointSize: 10
                color: root.palette.windowText
                wrapMode: Text.WordWrap
            }
        }
        ColumnLayout{
            spacing: 5
            Switch {
                id: debugSwitch
                text: "Debug packets"
                checked: settings.saveDebug
            }
            Label {
                text: `Save packet debug log into the <a href='file://${logFile}'>file.</a>`
                Layout.leftMargin: 50
                Layout.rightMargin: 20
                Layout.fillWidth: true
                font.pointSize: 10
                color: root.palette.windowText
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                onLinkActivated: function(url){
                    Qt.openUrlExternally(url);
                }
            }
        }
    }

    Settings {
        id: settings
        property alias demoDevice: demoSwitch.checked
        property alias saveDebug: debugSwitch.checked
    }
}