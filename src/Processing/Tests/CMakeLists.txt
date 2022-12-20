FILE(GLOB TESTS_PROCESSING_SRC
    grey.cpp
)

FILE(GLOB TESTS_PROCESSING_HEADERS
    simple.png
    simple_greyed.png
)

ASTRO_add_test(
    TestsProcessing
    NAME TestsProcessing
    TESTNAME TestsProcessing
    FOLDER Processing
    SRC ${TESTS_PROCESSING_SRC}
    HEADERS ${TESTS_PROCESSING_HEADERS}
    LIBRARIES AstroProcessing ${ITK_LIBRARIES}
)
