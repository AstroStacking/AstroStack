FILE(GLOB ASTRO_IO_SRC
    IO/input.cpp
    IO/output.cpp
)

FILE(GLOB ASTRO_IO_HEADERS
    IO/input.h
    IO/io.h
    IO/output.h
)

ASTRO_add_library(
    AstroIO
    NAME AstroIO
    FOLDER IO
    SRC ${ASTRO_IO_SRC}
    HEADERS ${ASTRO_IO_HEADERS}
    LIBRARIES AstroPlugin Qt6::Core Qt6::Widgets ${ITK_LIBRARIES}
)
