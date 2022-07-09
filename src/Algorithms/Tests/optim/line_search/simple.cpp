#include <Algos/optim/line_search/simple.h>

#include <optim/simple.h>

#include <gtest/gtest.h>

TEST(LineSearchSimple, Simple)
{
    Simple fun;

    const double size = .1;
    optim::State state;
    optim::line_search::Simple lineSearch(size);
    Eigen::VectorXd direction(2);
    direction << 1, 2;
    state.setDirection(direction);

    Eigen::VectorXd origin(2);
    origin << 10, 22;

    Eigen::VectorXd result = origin + size * direction;
    ASSERT_EQ(result, lineSearch(origin, fun, state));
}

TEST(LineSearchSimple, SimpleSpecificStep)
{
    Simple fun;
    const double size = .1;
    optim::State state;
    state.setInitialStep(1);
    optim::line_search::Simple lineSearch(size);
    Eigen::VectorXd direction(2);
    direction << 1, 2;
    state.setDirection(direction);

    Eigen::VectorXd origin(2);
    origin << 10, 22;

    Eigen::VectorXd result = origin + direction;
    ASSERT_EQ(result, lineSearch(origin, fun, state));
}
