FILE(GLOB TESTS_ALGORITHMS_SRC
    affinemodel.cpp
    ransac.cpp
    optim/powell.cpp
    optim/quadratic.cpp
    optim/rosenbrock.cpp
    optim/state.cpp
    optim/criteria/composite.cpp
    optim/criteria/criteria.cpp
    optim/helper/quadratic.cpp
    optim/line_search/backtracking.cpp
    optim/line_search/damped.cpp
    optim/line_search/simple.cpp
    optim/optimizer/standard.cpp
    optim/step/conjugate_gradient.cpp
    optim/step/gradient.cpp
    optim/step/newton.cpp
)

FILE(GLOB TESTS_ALGORITHMS_HEADERS
    affinemodel.h
    optim/powell.h
    optim/quadratic.h
    optim/rosenbrock.h
    optim/simple.h
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
