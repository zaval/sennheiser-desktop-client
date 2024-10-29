import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects


Rectangle {
    id: root
    width: 400
    height: 310
    radius: 20
    color: root.palette.window
    border.color: root.palette.mid
    clip: true
    property real maskPosition: 0

    ANCCardHelper {
        id: helper

        onAncTransparencyChanged: function() {
            slider.value = helper.ancTransparency / 100;
            root.maskPosition = image.width * slider.value
        }
    }

    Image {
        id: cardIcon
        source: "qrc:/icons/waveform.path.badge.minus.svg"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 10
        width: 36
        fillMode: Image.PreserveAspectFit
    }
    Colorize {
        visible: !UiHelper.isDarkTheme
        anchors.fill: cardIcon
        source: cardIcon
        hue: .0
        saturation: 0
        lightness:  -1
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 10
        spacing: 20
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2
            Switch {
                id: ancEnabledSwitch
                text: "Noise cancelation"
                checked: helper.ancEnabled

                onClicked: {
                    helper.ancEnabled = ancEnabledSwitch.checked
                }
            }
            Label {
                text: "Active noise cancellation"
                Layout.leftMargin: 50
                font.pointSize: 10
                color: root.palette.windowText

            }
        }
        ColumnLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 6

            spacing: 5
            Label {
                text: "Wind Noise Reduction"
                font.bold: true
            }

            Label {
                text: "Adjusts ANC to avoid unwanted noise in windy conditions"
                font.pointSize: 10
                color: root.palette.windowText

            }
            RowLayout {
                Layout.minimumWidth: root.width - 20
                Layout.fillHeight: false
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                layoutDirection: Qt.LeftToRight
                uniformCellSizes: true
                spacing: 20
                Layout.fillWidth: true

                Button {
                    id: buttonNoiseOff
                    width: 30
                    // Layout.preferredWidth: 30
                    text: "Off"
                    spacing: 5
                    autoExclusive: true
                    checked: helper.antiWindValue === 0
                    checkable: true
                    onClicked: {
                            helper.antiWindValue = 0;
                    }
                }
                Button {
                    id: buttonNoiseAuto
                    width: 30
                    // Layout.preferredWidth: 30
                    text: "Auto"
                    autoExclusive: true
                    checked: helper.antiWindValue === 2
                    checkable: true
                    onClicked: {
                        helper.antiWindValue = 2;
                    }
                }
                Button {
                    id: buttonNoiseMax
                    width: 30
                    // Layout.preferredWidth: 30
                    text: "Max"
                    autoExclusive: true
                    checked: helper.antiWindValue === 1
                    checkable: true
                    onClicked: {
                        helper.antiWindValue = 1;
                    }
                }

            }

        }

        ColumnLayout {
            spacing: 2
            Layout.fillWidth: true

            Switch {
                text: "Adaptive"
                id: adaptiveSwitch
                checked: helper.adaptiveEnabled
                onClicked: {
                    helper.adaptiveEnabled = adaptiveSwitch.checked
                }
            }
            Label {
                text: "ANC automatically adapts to the environment."
                font.pointSize: 10
                Layout.leftMargin: 50
                color: root.palette.windowText

            }
        }

        ColumnLayout {
            spacing: 2
            Layout.fillWidth: true
            Rectangle {
                Layout.fillWidth: true
                // height: 30
                Layout.preferredHeight: 30
                clip: true
                color: "transparent"
                Image {
                    id: image
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 30
                    source: UiHelper.isDarkTheme ? "qrc:/icons/waveform-dark.png" : "qrc:/icons/waveform.png"
                    fillMode: Image.Stretch
                    clip: true

                }
                Image {
                    id: mask
                    anchors.left: parent.left
                    anchors.right: parent.right
                    width: image.width
                    anchors.leftMargin: root.maskPosition
                    height: 30
                    source: "qrc:/icons/mask.png"
                    fillMode: Image.Stretch
                }
                Colorize {
                    visible: UiHelper.isDarkTheme
                    anchors.fill: mask
                    source: mask
                    hue: 1.0
                    saturation: 0
                    lightness:  -0.7490196228027344
                }
            }

            Slider {
                Layout.fillWidth: true
                id: slider
                enabled: !adaptiveSwitch.checked
                value: 0.5
                onMoved: {
                    root.maskPosition = image.width * slider.value;
                    helper.ancTransparency = slider.value * 100
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

}
