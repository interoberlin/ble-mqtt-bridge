project (playground)

add_executable(testFloorSensorJsonMapping testJsonMapping.cpp)

add_executable(testLoguru testLoguru.cpp)
target_link_libraries (testLoguru pthread dl )

# Install binaries
install (TARGETS testFloorSensorJsonMapping testLoguru RUNTIME DESTINATION bin/test)



