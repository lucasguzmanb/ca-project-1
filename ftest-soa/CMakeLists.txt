add_test(NAME ftest-soa-maxlevel
        COMMAND imtool-soa "${CMAKE_CURRENT_SOURCE_DIR}/input/deer-small.ppm" "${CMAKE_CURRENT_SOURCE_DIR}/output/deer-small-soa.ppm" maxlevel 5535)
add_test(NAME ftest-soa-cutfreq
        COMMAND imtool-soa "${CMAKE_CURRENT_SOURCE_DIR}/input/deer-small.ppm" "${CMAKE_CURRENT_SOURCE_DIR}/output/deer-small-soa.ppm" cutfreq 2000)
add_test(NAME ftest-soa-resize
        COMMAND imtool-soa "${CMAKE_CURRENT_SOURCE_DIR}/input/deer-small.ppm" "${CMAKE_CURRENT_SOURCE_DIR}/output/deer-small-soa.ppm" resize 200 200)
add_test(NAME ftest-soa-compress
        COMMAND imtool-soa "${CMAKE_CURRENT_SOURCE_DIR}/input/deer-small.ppm" "${CMAKE_CURRENT_SOURCE_DIR}/output/deer-small-soa.ppm" compress)