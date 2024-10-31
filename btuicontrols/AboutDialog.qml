import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: aboutDialog
    anchors.centerIn: parent
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    focus: true
    // width: 350
    width: link.implicitWidth + 150
    height: 230
    standardButtons: Dialog.Close
    modal: true
    Overlay.modal: Rectangle {
        color: Qt.alpha(root.palette.shadow, 0.5)
        // color: "#55888888"
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
            clip: true



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
                clip: true
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
                    clip: true
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
                    id: link
                    textFormat: Text.RichText
                    Layout.fillWidth: true
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
        }
    }

}