project (floorsensord)

add_executable(generateJsonFromCsv main.cpp )

# Install binaries
install (TARGETS generateJsonFromCsv RUNTIME DESTINATION bin)
