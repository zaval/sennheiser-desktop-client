import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Rectangle {
    id: root
    width: 350
    height: 75
    radius: 20
    color: root.palette.window
    border.color: root.palette.mid
    clip: true

    required property string name
    required property string address
    required property bool isOnline
    required property string uuid

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 10
        anchors.bottomMargin: 10

        spacing: 15
        Item {
            Layout.fillHeight: true
            Layout.preferredWidth: 30
            Image {
                id: headphones
                anchors.fill: parent
                source: "qrc:/icons/headphones.svg"
                fillMode: Image.PreserveAspectFit
            }
            Colorize {
                visible: !UiHelper.isDarkTheme
                anchors.fill: headphones
                source: headphones
                hue: .0
                saturation: 0
                lightness:  -1
            }
        }


        ColumnLayout {
            id: columnLayout
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 5

            Label {
                id: name
                text: root.name
                font.bold: true
                font.pointSize: 18
                color: root.palette.windowText
            }

            Label {
                id: address
                text: root.address
                font.pointSize: 10
                color: root.palette.windowText
            }

        }

        Item {
            Layout.fillWidth: true
        }

        Image {
            id: checkmark
            visible: root.isOnline
            source: "qrc:/icons/checkmark.circle.fill.svg"
            fillMode: Image.PreserveAspectFit
        }

    }




}
