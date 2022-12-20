FILE(GLOB TESTS_IO_SRC
    read.cpp
)

FILE(GLOB TESTS_IO_HEADERS
    gradient.png
    gradient_greyed.png
)

ASTRO_add_test(
    TestsIO
    NAME TestsIO
    TESTNAME TestsIO
    FOLDER IO
    SRC ${TESTS_IO_SRC}
    HEADERS ${TESTS_IO_HEADERS}
    LIBRARIES AstroIO ${ITK_LIBRARIES}
)
