project (alarmlightd)
add_executable(alarmlightd main.cpp)
target_link_libraries (alarmlightd processors endpoints mosquittopp tinyb pthread  dl )

# Install binaries
install (TARGETS alarmlightd RUNTIME DESTINATION bin)