include_directories("${PROJECT_SOURCE_DIR}/include/")

# define libraries
add_subdirectory(libendpoints)
add_subdirectory(libprocessors)
add_subdirectory(libsensors)

# define binaries
add_subdirectory(floorsensord)
add_subdirectory(alarmlightd)
add_subdirectory(robbyd)
add_subdirectory(joystickd)
add_subdirectory(dustsensord)
add_subdirectory(playground)

