FILE(GLOB ASTRO_IO_READ
    read.cpp
)

ASTRO_add_executable(
    Read
    NAME Read
    FOLDER IO/Standalone
    SRC ${ASTRO_IO_READ}
    LIBRARIES AstroIO Qt6::Core
)
