import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects


Rectangle {
    id: root
    width: 400
    height: 150
    radius: 20
    border.color: root.palette.mid
    clip: true
    color: root.palette.window

    OnHeadSettingsCardHelper {
        id: helper
    }

    Image {
        id: linkIcon
        source: "qrc:/icons/person.wave.2.svg"
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
    ColumnLayout{
        spacing: 10
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 10
        ColumnLayout {
            spacing: 2
            clip: true
            Switch {
                id: switchOnHeadDetection
                checked: helper.onHeadEnabled
                text: "On-head detection"
                onClicked: {
                    helper.onHeadEnabled = switchOnHeadDetection.checked;
                }
            }
            Label {
                Layout.leftMargin: 50
                text: "Activate Smart Pause, hold calls automatically, power saving mode."
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                font.pointSize: 10
                color: root.palette.windowText
                clip: true
            }

        }
        ColumnLayout {
            spacing: 2
            clip: true
            Switch {
                id: switchSmartPause
                checked: helper.smartPauseEnabled
                text: "Smart Pause"
                onClicked: {
                    helper.smartPauseEnabled = switchSmartPause.checked;
                }
            }
            Label {
                Layout.leftMargin: 50
                text: "Halts playback when the headphones are taken off and resumes playback when they are placed back on the ears."
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                font.pointSize: 10
                color: root.palette.windowText
                clip: true
            }

        }

        Item {
            Layout.fillHeight: true
        }
    }

}
