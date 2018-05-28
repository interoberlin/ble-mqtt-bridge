project (dustsensord)

add_executable(dustsensord main.cpp)

target_link_libraries (dustsensord endpoints sensors mosquittopp pthread dl)


# Install binaries
install (TARGETS dustsensord RUNTIME DESTINATION bin)
