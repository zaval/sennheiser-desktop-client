cmake_minimum_required(VERSION 3.20)

project(sennheiserDesktopClient
        VERSION 0.1.0
        LANGUAGES CXX
        DESCRIPTION "Sennheiser headphones configuration app"
        HOMEPAGE_URL "https://github.com/zaval/sennheiser-desktop-client"
)
add_definitions(-DPROJECT_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR})
add_definitions(-DPROJECT_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR})
add_definitions(-DPROJECT_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH})
add_definitions(-DPROJECT_DESCRIPTION=${CMAKE_PROJECT_DESCRIPTION})
add_definitions(-DPROJECT_URL="${CMAKE_PROJECT_HOMEPAGE_URL}")


set(CMAKE_CXX_STANDARD 17)
set(MACOSX_BUNDLE_ICON_FILE sennheiserDesktopClient.icns)
set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/resources/sennheiserDesktopClient.icns")
set_source_files_properties(${app_icon_macos} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")

enable_testing()

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Bluetooth Core Gui QuickControls2 Test)
qt_standard_project_setup(REQUIRES 6.5)
add_subdirectory(gaiaV3)
add_subdirectory(btuicontrols)

qt_add_executable(
        sennheiserDesktopClient
        MACOSX_BUNDLE
        main.cpp
        ${app_icon_macos}

)
qt_add_resources(
        sennheiserDesktopClient "res"
        PREFIX "/"
        BASE "resources"
        FILES
            resources/appicon.png
)

qt_add_qml_module(sennheiserDesktopClient
        URI btui
        VERSION 1.0
        QML_FILES
            Main.qml
)

set_target_properties(sennheiserDesktopClient PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER com.github.zaval.sennheiser-desktop-client
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(sennheiserDesktopClient
        PRIVATE
        Qt6::Quick
        Qt6::Bluetooth
        Qt6::QuickControls2
        Qt6::Gui
        gaiaV3
        btuicontrols
)

if (APPLE)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    if (IOS)
        set_target_properties(sennheiserDesktopClient PROPERTIES
                MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.cmake.ios.plist"
        )
    else()
        set_target_properties(sennheiserDesktopClient PROPERTIES
                MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.cmake.macos.plist"
        )
    endif()
endif()
