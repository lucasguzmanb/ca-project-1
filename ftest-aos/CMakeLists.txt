add_test(NAME ftest-aos-maxlevel
        COMMAND imtool-aos "${CMAKE_CURRENT_SOURCE_DIR}/input/prueba.ppm" maxlevel 5535)
add_test(NAME ftest-aos-cutfreq
        COMMAND imtool-aos "${CMAKE_CURRENT_SOURCE_DIR}/input/prueba.ppm" cutfreq 2000)
add_test(NAME ftest-aos-resize
        COMMAND imtool-aos "${CMAKE_CURRENT_SOURCE_DIR}/input/prueba.ppm" resize 200 200)
add_test(NAME ftest-aos-compress
        COMMAND imtool-aos "${CMAKE_CURRENT_SOURCE_DIR}/input/prueba.ppm" compress)