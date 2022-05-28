FILE(GLOB TESTS_ALGORITHMS_SRC
    ransac.cpp
)

ASTRO_add_test(
    TestsAlgorithms
    NAME TestsAlgorithms
    TESTNAME TestsAlgorithms
    FOLDER Algorithms
    SRC ${TESTS_ALGORITHMS_SRC}
    HEADERS ${TESTS_ALGORITHMS_HEADERS}
    LIBRARIES AstroAlgorithms ${ITK_LIBRARIES}
)
