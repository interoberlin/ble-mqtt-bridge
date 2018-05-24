project (floorsensord)

add_executable(floorsensord main.cpp Config.cpp ValueSplitter.cpp)

target_link_libraries (floorsensord endpoints mosquittopp pthread tinyb )


# Install binaries
install (TARGETS floorsensord RUNTIME DESTINATION bin)