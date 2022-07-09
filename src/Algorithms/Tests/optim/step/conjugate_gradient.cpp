#include <Algos/optim/step/conjugate_gradient.h>

#include <gtest/gtest.h>

#include <optim/powell.h>

namespace
{
class Simple
{
public:
    static double conjugate(const Eigen::VectorXd& newGadient, const Eigen::VectorXd& oldGadient, const Eigen::VectorXd& direction)
    {
        return 0;
    }
};

}

TEST(ConjugateGradient, Powell)
{
    optim::State state;
    optim::step::ConjugateGradient<Simple> gradient;
    Powell fun;

    Eigen::VectorXd x(4);
    x << 0, 0, 0, 0;
    ASSERT_EQ(gradient(x, state, fun), -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));

    x << 1, 1, 0, 0;
    ASSERT_EQ(gradient(x, state, fun), -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
}
