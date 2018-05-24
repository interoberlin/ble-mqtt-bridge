include_directories("${PROJECT_SOURCE_DIR}/include/")

# define libraries
add_subdirectory(libendpoints)
# add_subdirectory(libprocessors)

# define binaries
add_subdirectory(floorsensord)
# add_subdirectory(alarmlightd)
add_subdirectory(test)

