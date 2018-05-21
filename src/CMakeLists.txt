include_directories("${PROJECT_SOURCE_DIR}/include/")

# define libraries
add_subdirectory(endpoints)
# add_subdirectory(processors)

# define binaries
add_subdirectory(floorsensord)
# add_subdirectory(alarmlightd)