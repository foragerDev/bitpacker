include_directories(${libbitpacker_SOURCE_DIR}/include)


set(CXX_STANDARD 23)
set(CXX_STANDARD_REQUIRED true)

add_executable(usage_example
        ../example/example.cpp cbor_variant.cpp encode.cpp serialize.cpp deserialize.cpp)

link_directories(include)
