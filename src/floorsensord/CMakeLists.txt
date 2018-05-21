project (floorsensord)
add_executable(floorsensord main.cpp Config.cpp ValueSplitter.cpp)
target_link_libraries (floorsensord endpoints mosquittopp)