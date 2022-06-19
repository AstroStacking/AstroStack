FILE(GLOB ASTRO_IO_SRC
    IO/input.cpp
    IO/io.cpp
    IO/itkinput.cpp
    IO/itkoutput.cpp
    IO/output.cpp
)

FILE(GLOB ASTRO_IO_HEADERS
    IO/config.h
    IO/input.h
    IO/io.h
    IO/itkinput.h
    IO/itkoutput.h
    IO/output.h
)

SET(ASTRO_IO_LIBS Qt6::Core Qt6::Widgets ${ITK_LIBRARIES})
if(EXIV2_FOUND)
    LIST(APPEND ASTRO_IO_LIBS exiv2lib)
endif()

ASTRO_add_library(
    AstroIO
    NAME AstroIO
    FOLDER IO
    SRC ${ASTRO_IO_SRC}
    HEADERS ${ASTRO_IO_HEADERS}
    LIBRARIES AstroPlugin ${ASTRO_IO_LIBS}
)
