include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
enable_testing()

add_executable(test_GAIAPropertyBase test_GAIAPropertyBase.cpp)
target_link_libraries(test_GAIAPropertyBase Qt6::Core Qt6::Test gaiaV3)
add_test(GAIAPropertyBaseTests test_GAIAPropertyBase)

add_executable(test_GAIARfcommClient test_GAIARfcommClient.cpp)
target_link_libraries(test_GAIARfcommClient Qt6::Core Qt6::Test Qt6::Bluetooth Qt6::Quick gaiaV3)
add_test(GAIARfcommClientTests test_GAIARfcommClient)

