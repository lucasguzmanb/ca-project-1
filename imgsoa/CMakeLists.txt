# Add to this list all files related to imgsoa library
add_library(imgsoa
        imagesoa.cpp
        cutfreq.cpp
        cutfreq.hpp
        maxlevel.cpp
        maxlevel.hpp
        resize.cpp
        resize.hpp
        info.cpp
        info.hpp
        compress.cpp
        compress.hpp
        resize.cpp
        resize.hpp
        info.cpp
        info.hpp
        compress.cpp
        compress.hpp
)
# Use this line only if you have dependencies from this library to GSL
target_link_libraries (imgsoa PRIVATE common Microsoft.GSL::GSL)