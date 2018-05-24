project (test)
add_executable(testFloorSensorJsonMapping configtest.cpp)

add_executable(testLoguru testLoguru.cpp)
target_link_libraries (testLoguru pthread dl )


# Install binaries
install (TARGETS testFloorSensorJsonMapping testLoguru RUNTIME DESTINATION bin)