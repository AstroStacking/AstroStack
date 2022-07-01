FILE(GLOB TESTS_ALGORITHMS_SRC
    affinemodel.cpp
    ransac.cpp
    optim/rosenbrock.cpp
    optim/state.cpp
    optim/criteria/composite.cpp
    optim/criteria/criteria.cpp
    optim/line_search/simple.cpp
)

FILE(GLOB TESTS_ALGORITHMS_HEADERS
    optim/rosenbrock.h
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
