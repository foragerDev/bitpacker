include_directories(${libbitpacker_SOURCE_DIR}/include)

set(sourcepath ${libbitpacker_SOURCE_DIR}/src)

set(sources
        ${sourcepath}/encode.cpp
        ${sourcepath}/serialize.cpp
        ${sourcepath}/cbor_variant.cpp
        ${sourcepath}/deserialize.cpp
        test_serializer.cpp
        test_deserializers.cpp
)

#message(echo ${sources})
add_executable(tests ${sources})
target_link_libraries(
        tests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)