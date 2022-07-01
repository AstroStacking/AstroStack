#include <Algos/optim/line_search/simple.h>

#include <gtest/gtest.h>

TEST(LineSearch, Simple)
{
    const double size = .1;
    optim::State state;
    optim::line_search::Simple lineSearch(size);
    Eigen::VectorXd direction(2);
    direction << 1, 2;
    state.setDirection(direction);

    Eigen::VectorXd origin(2);
    origin << 10, 22;
    
    Eigen::VectorXd result = origin + size * direction;
    ASSERT_EQ(result, lineSearch(origin, state));
}

TEST(LineSearch, SimpleSpecificStep)
{
    const double size = .1;
    optim::State state;
    state.setStep(1);
    optim::line_search::Simple lineSearch(size);
    Eigen::VectorXd direction(2);
    direction << 1, 2;
    state.setDirection(direction);

    Eigen::VectorXd origin(2);
    origin << 10, 22;
    
    Eigen::VectorXd result = origin + direction;
    ASSERT_EQ(result, lineSearch(origin, state));
}
