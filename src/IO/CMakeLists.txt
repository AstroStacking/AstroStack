FILE(GLOB ASTRO_IO_SRC
    IO/input.cpp
    IO/itkInput.cpp
    IO/output.cpp
    IO/pluginfactory.cpp
)

FILE(GLOB ASTRO_IO_HEADERS
    IO/input.h
    IO/itkInput.h
    IO/io.h
    IO/output.h
    IO/pluginfactory.h
)

ASTRO_add_library(
    AstroIO
    NAME AstroIO
    FOLDER IO
    SRC ${ASTRO_IO_SRC}
    HEADERS ${ASTRO_IO_HEADERS}
    LIBRARIES Qt6::Core Qt6::Widgets ${ITK_LIBRARIES}
)
