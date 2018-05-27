project (robbyd)

add_executable(robbyd main.cpp MQTTClientRobby.cpp)

target_link_libraries (robbyd endpoints mosquittopp pthread tinyb dl)


# Install binaries
install (TARGETS robbyd RUNTIME DESTINATION bin)
