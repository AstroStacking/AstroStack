#include <Algos/optim/step/conjugate_gradient.h>

#include <gtest/gtest.h>

#include <optim/powell.h>

namespace
{
class Function
{
public:
    double operator()(const Eigen::VectorXd& X) const { return std::pow(X(0) - 2, 2) + std::pow(2 * X(1) + 4, 2); }

    Eigen::VectorXd gradient(const Eigen::VectorXd& X) const
    {
        Eigen::VectorXd result(2);
        result << 2. * (X(0) - 2), 4 * (2 * X(1) + 4);

        return result;
    }
};

class Simple
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

TEST(ConjugateGradient, Simple)
{
    optim::State state;
    optim::step::ConjugateGradient<Simple> gradient;
    Function fun;

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

TEST(ConjugateGradient, CWConjugate)
{
    optim::State state;
    optim::step::ConjugateGradient<optim::step::CWConjugate> gradient;
    Function fun;

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

TEST(ConjugateGradient, DConjugate)
{
    optim::State state;
    optim::step::ConjugateGradient<optim::step::DConjugate> gradient;
    Function fun;

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

TEST(ConjugateGradient, DYConjugate)
{
    optim::State state;
    optim::step::ConjugateGradient<optim::step::DYConjugate> gradient;
    Function fun;

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

TEST(ConjugateGradient, FRConjugate)
{
    optim::State state;
    optim::step::ConjugateGradient<optim::step::FRConjugate> gradient;
    Function fun;

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

TEST(ConjugateGradient, PRPConjugate)
{
    optim::State state;
    optim::step::ConjugateGradient<optim::step::PRPConjugate> gradient;
    Function fun;

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

TEST(ConjugateGradient, FRPRPConjugate)
{
    optim::State state;
    optim::step::ConjugateGradient<optim::step::FRPRPConjugate> gradient;
    Function fun;

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

TEST(ConjugateGradient, HZConjugate)
{
    optim::State state;
    optim::step::ConjugateGradient<optim::step::HZConjugate> gradient;
    Function fun;

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
