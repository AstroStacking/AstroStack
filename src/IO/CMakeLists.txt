FILE(GLOB ASTRO_IO_SRC
    IO/hdf5.cpp
    IO/io.cpp
    IO/itkinput.cpp
    IO/itkoutput.cpp
    IO/traits.cpp
)

FILE(GLOB ASTRO_IO_HEADERS
    IO/config.h
    IO/hdf5.h
    IO/io.h
    IO/itkinput.h
    IO/itkoutput.h
    IO/traits.h
    IO/tests/hdf5suite.h
)

SET(ASTRO_IO_LIBS ${ITK_LIBRARIES})
if(EXIV2_FOUND)
    LIST(APPEND ASTRO_IO_LIBS exiv2lib)
endif()

ASTRO_add_library(
    AstroIO
    NAME AstroIO
    FOLDER IO
    SRC ${ASTRO_IO_SRC}
    HEADERS ${ASTRO_IO_HEADERS}
    LIBRARIES ${ASTRO_IO_LIBS} hdf5::hdf5_cpp hdf5::hdf5 ${ASTROSTACK_TBB}
)

add_subdirectory(Standalone)
add_subdirectory(Tests)
