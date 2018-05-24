project (test)
add_executable(testJsonMapping configtest.cpp)

# Install binaries
install (TARGETS testJsonMapping RUNTIME DESTINATION bin)