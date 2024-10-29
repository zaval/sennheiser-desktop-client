import QtQuick.Controls
import QtQuick

Page {

    id: devicePage
    anchors.fill: parent
    anchors.leftMargin: 10
    anchors.rightMargin: 10
    anchors.topMargin: 10
    anchors.bottomMargin: 10

        Flow {
            // anchors.fill: parent
            spacing: 10


            // anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: devicePage.width * .9

            Column {
                spacing: 10
                DeviceInfoCard{}
                OnHeadSettingsCard{}
                // SettingsCard{}

            }
            ANCCard2{}
            ConnectedDevicesCard{}
            CallSettingsCard{}
        }
}