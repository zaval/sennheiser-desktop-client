pragma Singleton
import QtQuick


Item {
    id: uihelper
    property bool isDarkTheme: false
    function updateTheme() {
        isDarkTheme = systemPalette.window.hslLightness < .5;
    }

    SystemPalette {
        id: systemPalette
        onColorGroupChanged: {
            updateTheme()
        }
    }

    Component.onCompleted: updateTheme()

}