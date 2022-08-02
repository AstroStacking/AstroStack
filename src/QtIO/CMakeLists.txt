FILE(GLOB ASTRO_QTIO_SRC
    QtIO/input.cpp
    QtIO/itkinput.cpp
    QtIO/itkoutput.cpp
    QtIO/output.cpp
)

FILE(GLOB ASTRO_QTIO_HEADERS
    QtIO/config.h
    QtIO/input.h
    QtIO/itkinput.h
    QtIO/itkoutput.h
    QtIO/output.h
)

ASTRO_add_library(
    AstroQtIO
    NAME AstroQtIO
    FOLDER IO
    SRC ${ASTRO_QTIO_SRC}
    HEADERS ${ASTRO_QTIO_HEADERS}
    LIBRARIES AstroPlugin AstroIO
)
