find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(GAIAPropertyClasses_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/GAIAPropertyClasses.h
        ${CMAKE_CURRENT_SOURCE_DIR}/GAIAPropertyClasses.cpp
)

set(GAIAPropertyManager_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/GAIAPropertyManager.h
        ${CMAKE_CURRENT_SOURCE_DIR}/GAIAPropertyManager.cpp
)

add_custom_command(
        OUTPUT ${GAIAPropertyClasses_SRC}
        COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/property_creator.py
            -i ${CMAKE_CURRENT_SOURCE_DIR}/m4.json
            -o ${CMAKE_CURRENT_SOURCE_DIR}/GAIAPropertyClasses
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/m4.json ${CMAKE_CURRENT_SOURCE_DIR}/scripts/property_creator.py
        VERBATIM
)

add_custom_command(
        OUTPUT ${GAIAPropertyManager_SRC}
        COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/property_manager_creator.py
            -i ${CMAKE_CURRENT_SOURCE_DIR}/m4.json
            -o ${CMAKE_CURRENT_SOURCE_DIR}/GAIAPropertyManager
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/m4.json ${CMAKE_CURRENT_SOURCE_DIR}/scripts/property_manager_creator.py
        VERBATIM
)

qt_add_library(gaiaV3
        GAIAPropertyBase.cpp
        GAIAPropertyBase.h
        ${GAIAPropertyClasses_SRC}
        ${GAIAPropertyManager_SRC}
        GAIARfcommClient.cpp
        GAIARfcommClient.h
        GAIAPropertyManager.cpp
        GAIAPropertyManager.h
        GAIAPropertyManagerBase.cpp
        GAIAPropertyManagerBase.h
        BluetoothSocketWrapper.cpp
        BluetoothSocketWrapper.h
)

qt_add_qml_module(gaiaV3
        URI gaiaV3
        VERSION 1.0
        SOURCES
            ${GAIAPropertyClasses_SRC}
            ${GAIAPropertyManager_SRC}
            GAIARfcommClient.cpp
            GAIARfcommClient.h
)

target_link_libraries(gaiaV3 PRIVATE Qt6::Core Qt6::Quick Qt6::Bluetooth)

add_subdirectory(tests)