# Add to this list all files related to imgaos library
add_library(imgaos
        imageaos.cpp
        imageaos.hpp
        #[[cutfreq.cpp
        cutfreq.hpp]] # uncomment this line when cutfreq is implemented
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
target_link_libraries (imgaos PRIVATE common Microsoft.GSL::GSL)