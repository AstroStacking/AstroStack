#include <Algos/Optim/Step/conjugate_gradient.h>

#include <Optim/simple.h>

#include <gtest/gtest.h>

namespace
{
class Dud
{
public:
    static double conjugate(const Eigen::VectorXd& newGradient, const Eigen::VectorXd& oldGradient,
                            const Eigen::VectorXd& direction)
    {
        return 0;
    }
};

constexpr double ARTIFICIAL_FIRST_STEP = 0.178571428571;

} // namespace

TEST(ConjugateGradientStep, Simple)
{
    optim::State state;
    optim::step::ConjugateGradient<Dud> gradient;
    Simple fun;

    Eigen::VectorXd x(2);
    x << 0, 0;
    ASSERT_EQ(gradient(x, state, fun), -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_EQ(state.getData("conjugate_coefficient"), 0);

    x << 1, 1;
    ASSERT_EQ(gradient(x, state, fun), -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_EQ(state.getData("conjugate_coefficient"), 0);
}

TEST(ConjugateGradientStep, CWConjugate)
{
    optim::State state;
    optim::step::ConjugateGradient<optim::step::CWConjugate> gradient;
    Simple fun;

    Eigen::VectorXd x(2);
    x << 0, 0;
    auto step = gradient(x, state, fun);
    ASSERT_EQ(step, -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_EQ(state.getData("conjugate_coefficient"), 0);

    x = ARTIFICIAL_FIRST_STEP * step;
    state.setDirection(x);
    auto newStep = gradient(x, state, fun);
    ASSERT_NE(newStep, -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_NE(state.getData("conjugate_coefficient"), 0);
    ASSERT_LT(fun(x + 0.01 * newStep), fun(x));
}

TEST(ConjugateGradientStep, DConjugate)
{
    optim::State state;
    optim::step::ConjugateGradient<optim::step::DConjugate> gradient;
    Simple fun;

    Eigen::VectorXd x(2);
    x << 0, 0;
    auto step = gradient(x, state, fun);
    ASSERT_EQ(step, -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_EQ(state.getData("conjugate_coefficient"), 0);

    x = ARTIFICIAL_FIRST_STEP * step;
    state.setDirection(x);
    auto newStep = gradient(x, state, fun);
    ASSERT_NE(newStep, -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_NE(state.getData("conjugate_coefficient"), 0);
    ASSERT_LT(fun(x + 0.001 * newStep), fun(x));
}

TEST(ConjugateGradientStep, DYConjugate)
{
    optim::State state;
    optim::step::ConjugateGradient<optim::step::DYConjugate> gradient;
    Simple fun;

    Eigen::VectorXd x(2);
    x << 0, 0;
    auto step = gradient(x, state, fun);
    ASSERT_EQ(step, -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_EQ(state.getData("conjugate_coefficient"), 0);

    x = ARTIFICIAL_FIRST_STEP * step;
    state.setDirection(x);
    auto newStep = gradient(x, state, fun);
    ASSERT_NE(newStep, -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_NE(state.getData("conjugate_coefficient"), 0);
    ASSERT_LT(fun(x + 0.01 * newStep), fun(x));
}

TEST(ConjugateGradientStep, FRConjugate)
{
    optim::State state;
    optim::step::ConjugateGradient<optim::step::FRConjugate> gradient;
    Simple fun;

    Eigen::VectorXd x(2);
    x << 0, 0;
    auto step = gradient(x, state, fun);
    ASSERT_EQ(step, -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_EQ(state.getData("conjugate_coefficient"), 0);

    x = ARTIFICIAL_FIRST_STEP * step;
    state.setDirection(x);
    auto newStep = gradient(x, state, fun);
    ASSERT_NE(newStep, -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_NE(state.getData("conjugate_coefficient"), 0);
    ASSERT_LT(fun(x + 0.01 * newStep), fun(x));
}

TEST(ConjugateGradientStep, PRPConjugate)
{
    optim::State state;
    optim::step::ConjugateGradient<optim::step::PRPConjugate> gradient;
    Simple fun;

    Eigen::VectorXd x(2);
    x << 0, 0;
    auto step = gradient(x, state, fun);
    ASSERT_EQ(step, -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_EQ(state.getData("conjugate_coefficient"), 0);

    x = ARTIFICIAL_FIRST_STEP * step;
    state.setDirection(x);
    auto newStep = gradient(x, state, fun);
    ASSERT_NE(newStep, -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_NE(state.getData("conjugate_coefficient"), 0);
    ASSERT_LT(fun(x + 0.01 * newStep), fun(x));
}

TEST(ConjugateGradientStep, FRPRPConjugate)
{
    optim::State state;
    optim::step::ConjugateGradient<optim::step::FRPRPConjugate> gradient;
    Simple fun;

    Eigen::VectorXd x(2);
    x << 0, 0;
    auto step = gradient(x, state, fun);
    ASSERT_EQ(step, -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_EQ(state.getData("conjugate_coefficient"), 0);

    x = ARTIFICIAL_FIRST_STEP * step;
    state.setDirection(x);
    auto newStep = gradient(x, state, fun);
    ASSERT_NE(newStep, -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_NE(state.getData("conjugate_coefficient"), 0);
    ASSERT_LT(fun(x + 0.01 * newStep), fun(x));
}

TEST(ConjugateGradientStep, HZConjugate)
{
    optim::State state;
    optim::step::ConjugateGradient<optim::step::HZConjugate> gradient;
    Simple fun;

    Eigen::VectorXd x(2);
    x << 0, 0;
    auto step = gradient(x, state, fun);
    ASSERT_EQ(step, -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_EQ(state.getData("conjugate_coefficient"), 0);

    x = ARTIFICIAL_FIRST_STEP * step;
    state.setDirection(x);
    auto newStep = gradient(x, state, fun);
    ASSERT_NE(newStep, -fun.gradient(x));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
    ASSERT_NE(state.getData("conjugate_coefficient"), 0);
    ASSERT_LT(fun(x + 0.01 * newStep), fun(x));
}
