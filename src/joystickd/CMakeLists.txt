project (joystickd)

add_executable(joystickd main.cpp Steering.cpp)

target_link_libraries (joystickd endpoints sensors mosquittopp pthread dl)


# Install binaries
install (TARGETS joystickd RUNTIME DESTINATION bin)
