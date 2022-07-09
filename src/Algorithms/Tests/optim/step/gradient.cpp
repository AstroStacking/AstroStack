#include <Algos/optim/step/gradient.h>

#include <gtest/gtest.h>

#include <optim/powell.h>

TEST(GradientStep, Powell)
{
    optim::State state;
    optim::step::Gradient gradient;
    Powell fun;

    Eigen::VectorXd x(4);
    x << 0, 0, 0, 0;
    ASSERT_EQ(gradient(x, state, fun), -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));

    x << 1, 1, 0, 0;
    ASSERT_EQ(gradient(x, state, fun), -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
}
