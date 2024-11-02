cmake_minimum_required(VERSION 3.20)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(ECM_MODULE_DIR ${CMAKE_SOURCE_DIR}/cmake )
include(FeatureSummary)

include(ECMAddAppIcon)


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


set(
        ICONS
        resources/icons/16-apps-sennheiser-desktop-client.png
        resources/icons/32-apps-sennheiser-desktop-client.png
        resources/icons/64-apps-sennheiser-desktop-client.png
        resources/icons/128-apps-sennheiser-desktop-client.png
        resources/icons/256-apps-sennheiser-desktop-client.png
        resources/icons/512-apps-sennheiser-desktop-client.png
        resources/icons/1024-apps-sennheiser-desktop-client.png
)


set(CMAKE_CXX_STANDARD 17)
set_source_files_properties(${MACOSX_BUNDLE_ICON_FILE} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")

enable_testing()

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Bluetooth Core Gui QuickControls2 Svg Test)
qt_standard_project_setup(REQUIRES 6.5)
add_subdirectory(gaiaV3)
add_subdirectory(btuicontrols)

qt_add_executable(
        sennheiserDesktopClient
        MACOSX_BUNDLE
        main.cpp
        ${app_icon_macos}

)
ecm_add_app_icon(sennheiserDesktopClient ICONS ${ICONS} OUTFILE_BASENAME "Appicon")

if(UNIX AND NOT APPLE)
        include(GNUInstallDirs)
        include(ECMInstallIcons)
        ecm_install_icons( ICONS ${ICONS} DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons )
endif()

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
        Qt6::Svg
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

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/app) # for testing

if(APPLE)
    set(deploy_tool_options_arg -qmldir=${CMAKE_SOURCE_DIR})
    if (DEFINED CMAKE_CODESIGN_IDENTITY)
        set(deploy_tool_options_arg "${deploy_tool_options_arg} -codesign=${CMAKE_CODESIGN_IDENTITY}")
    endif ()
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/app)
endif()

install(TARGETS sennheiserDesktopClient
        BUNDLE  DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_qml_app_script(
        TARGET sennheiserDesktopClient
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
        DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
        MACOS_BUNDLE_POST_BUILD
        DEPLOY_TOOL_OPTIONS ${deploy_tool_options_arg}
)
install(SCRIPT ${deploy_script})

if(UNIX AND NOT APPLE)
    install(FILES ${CMAKE_SOURCE_DIR}/sennheiserDesktopClient.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
#     install(FILES ${CMAKE_SOURCE_DIR}/resources/icons/256-apps-sennheiser-desktop-client.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/256x256/apps)
endif()
set(CPACK_PACKAGE_NAME sennheiser-desktop-client)
#https://github.com/elfmz/far2l/blob/c35f97e275ef5bbbd8f292207e8d28b6e8f4af1d/packaging/CMakeLists.txt
if(APPLE)
    set(CPACK_GENERATOR DragNDrop)
    set(CPACK_DMG_VOLUME_NAME ${CPACK_PACKAGE_NAME}-${VERSION})
    set(CPACK_DMG_DS_STORE_SETUP_SCRIPT ${CMAKE_BINARY_DIR}/misc/setup_dmg.scpt)
    set(CPACK_DMG_BACKGROUND_IMAGE ${CMAKE_SOURCE_DIR}/misc/background.png)
    configure_file(${PROJECT_SOURCE_DIR}/misc/setup_dmg.scpt.in ${CMAKE_BINARY_DIR}/misc/setup_dmg.scpt)
endif()

if (UNIX AND NOT APPLE)
    find_program(DPKG_CMD dpkg)
    if(DPKG_CMD)
        set(CPACK_PACKAGE_VENDOR "zaval")
        set(CPACK_GENERATOR DEB)
        set(CPACK_DEBIAN_BASE_PACKAGE_NAME ${CPACK_PACKAGE_NAME})
        set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${CPACK_PACKAGE_VENDOR})
        set(CPACK_DEBIAN_PACKAGE_SECTION sound)
        set(CPACK_DEBIAN_PACKAGE_PRIORITY optional)
        set(CPACK_PACKAGE_CONTACT ${CPACK_PACKAGE_VENDOR})
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6dbus6 (>= 6.5.0), libqt6qml6 (>= 6.5.0), libqt6core6t64 (>= 6.5.0), libqt6network6 (>= 6.5.0), libqt6gui6t64 (>= 6.5.0), libqt6bluetooth6 (>= 6.5.0)")
    else(DPKG_CMD)
        MESSAGE( STATUS "dpkg binary not found, not building debian package" )
    endif(DPKG_CMD)
endif ()


include(CPack)
feature_summary(
        FATAL_ON_MISSING_REQUIRED_PACKAGES
        WHAT ALL
)
