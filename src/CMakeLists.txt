include_directories(${libbitpacker_SOURCE_DIR}/include)


set(CXX_STANDARD 23)
set(CXX_STANDARD_REQUIRED true)

add_executable(usage_example
        ../exampple/example.cpp)
link_directories(include)