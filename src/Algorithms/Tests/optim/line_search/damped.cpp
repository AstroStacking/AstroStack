#include <Algos/optim/line_search/damped.h>

#include <optim/simple.h>

#include <gtest/gtest.h>

TEST(LineSearchDamped, Damped)
{
    Simple fun;
    
    const double size = .1;
    optim::State state;
    optim::line_search::Damped lineSearch(0.0001, 0.1);
    Eigen::VectorXd direction(2);
    direction << 1, 1;
    state.setDirection(direction);

    Eigen::VectorXd origin(2);
    origin << 0, 0;

    ASSERT_TRUE(lineSearch(origin, fun, state).isApprox(0.125 * direction));
    ASSERT_EQ(state.getStep(), 0.125);
}
