# CMakeList.txt : CMake project for lameduck, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (lameduck "lameduck.cpp" "lameduck.h")
find_package(fmt CONFIG REQUIRED)
target_link_libraries(lameduck PRIVATE fmt::fmt)

# TODO: Add tests and install targets if needed.
