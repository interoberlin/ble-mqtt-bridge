project (test)
add_executable(testJsonMapping testJsonMapping.cpp)

# Install binaries
install (TARGETS testJsonMapping RUNTIME DESTINATION bin)
