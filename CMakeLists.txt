cmake_minimum_required(VERSION 3.22.1)
project(libbitpacker)

add_subdirectory(src)
add_subdirectory(test)
include_directories(include)

set(CXX_STANDARD 23)
set(CXX_STANDARD_REQUIRED true)

# add_executable(TARGET  src/bitpacker.cpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)