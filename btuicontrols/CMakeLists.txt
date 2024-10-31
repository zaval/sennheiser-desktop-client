set_source_files_properties(UiHelper.qml PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(
        btuicontrols
        URI btuicontrols
        VERSION 1.0
        QML_FILES
            DevicePage.qml
            DeviceInfoCard.qml
            ConnectedDevicesCard.qml
            FoundDeviceCard.qml
            DeviceListPage.qml
            ANCCard2.qml
            UiHelper.qml
            CallSettingsCard.qml
            OnHeadSettingsCard.qml
            SettingsDialog.qml
            AboutDialog.qml
            DatabaseStorage.qml
        SOURCES
            BluetoothHandler.cpp
            BluetoothHandler.h
            DeviceInfoCardHelper.cpp
            DeviceInfoCardHelper.h
            ANCCardHelper.cpp
            ANCCardHelper.h
            ConnectedDevicesCardHelper.cpp
            ConnectedDevicesCardHelper.h
            OnHeadSettingsCardHelper.cpp
            OnHeadSettingsCardHelper.h
            CallSettingsCardHelper.cpp
            CallSettingsCardHelper.h

)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(btuicontrols PRIVATE Qt6::Core Qt6::Quick Qt6::Bluetooth gaiaV3)

qt_add_resources(
        btuicontrols "icons"
        PREFIX "/icons"
        BASE assets
        FILES
            assets/arrow.left.svg
            assets/battery.0percent.svg
            assets/battery.25percent.svg
            assets/battery.50percent.svg
            assets/battery.75percent.svg
            assets/battery.100percent.svg
            assets/checkmark.circle.fill.svg
            assets/display.2.svg
            assets/gearshape.svg
            assets/headphones.svg
            assets/m4.png
            assets/mask.png
            assets/person.wave.2.svg
            assets/phone.badge.waveform.svg
            assets/questionmark.circle.svg
            assets/trash.svg
            assets/waveform.png
            assets/waveform-dark.png
            assets/waveform.path.badge.minus.svg
)
