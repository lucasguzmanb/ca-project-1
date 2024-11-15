# Executable for all unit tests with list of sources
# For example, you may have one ∗_test.cpp for each ∗.cpp in img-aos
add_executable(utest-imgsoa
        resize_test.cpp
        maxlevel_test.cpp
        ../utest-imgaos/compress_test.cpp
        map_test.cpp)
# Library dependencies
target_link_libraries (utest-imgsoa
        PRIVATE
        imgsoa
        GTest::gtest_main
        Microsoft.GSL::GSL)