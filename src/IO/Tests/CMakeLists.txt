FILE(GLOB TESTS_IO_SRC
    hdf5.cpp
    itk.cpp
)

FILE(GLOB TESTS_IO_HEADERS
    black.png
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
