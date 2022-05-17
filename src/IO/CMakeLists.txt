FILE(GLOB ASTRO_IO_SRC
    input.cpp
    output.cpp
)

FILE(GLOB ASTRO_IO_HEADERS
    io.h
    input.h
    output.h
)

ASTRO_add_library(
    AstroIO
    NAME AstroIO
    FOLDER IO
    SRC ${ASTRO_IO_SRC}
    HEADERS ${ASTRO_IO_HEADERS}
    LIBRARIES Qt6::Core ${ITK_LIBRARIES}
)
