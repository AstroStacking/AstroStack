#include <Algos/optim/criteria/composite.h>
#include <Algos/optim/criteria/criteria.h>
#include <Algos/optim/line_search/simple.h>
#include <Algos/optim/optimizer/standard.h>
#include <Algos/optim/step/gradient.h>
#include <Algos/optim/step/newton.h>

#include <gtest/gtest.h>

#include <optim/rosenbrock.h>

TEST(Optimizer, Rosenbrock_SimpleGradient)
{
    Rosenbrock function;
    auto optimizer = optim::optimizer::makeStandard(
            function, optim::criteria::makeOr(optim::criteria::Iteration(10000), optim::criteria::Monotony(0.00001)),
            optim::line_search::Simple(0.001), optim::step::Gradient());

    Eigen::VectorXd x0(2);
    x0 << -1.01, 1.01;

    Eigen::VectorXd solution(2);
    solution << 1, 1;

    ASSERT_TRUE(optimizer(x0).isApprox(solution, 0.1));
    ASSERT_NE(optimizer.getState().getStatus(), optim::Status::NOT_STOPPED);
    ASSERT_TRUE(optimizer.getState().getCurrentPoint().isApprox(solution, 0.1));
}

TEST(Optimizer, Rosenbrock_SimpleGradient2)
{
    Rosenbrock function;
    auto optimizer = optim::optimizer::makeStandard(function, optim::criteria::RelativeValue(0.0001),
                                                    optim::line_search::Simple(0.001), optim::step::Gradient());

    Eigen::VectorXd x0(2);
    x0 << -1.01, 1.01;

    Eigen::VectorXd solution(2);
    solution << 1, 1;

    ASSERT_TRUE(optimizer(x0).isApprox(solution, 0.1));
    ASSERT_NE(optimizer.getState().getStatus(), optim::Status::NOT_STOPPED);
    ASSERT_TRUE(optimizer.getState().getCurrentPoint().isApprox(solution, 0.1));
}

TEST(Optimizer, Rosenbrock_SimpleNewton)
{
    Rosenbrock function;
    auto optimizer = optim::optimizer::makeStandard(function, optim::criteria::RelativeValue(0.00001),
                                                    optim::line_search::Simple(1), optim::step::Newton());

    Eigen::VectorXd x0(2);
    x0 << -1.01, 1.01;

    Eigen::VectorXd solution(2);
    solution << 1, 1;

    ASSERT_TRUE(optimizer(x0).isApprox(solution));
    ASSERT_NE(optimizer.getState().getStatus(), optim::Status::NOT_STOPPED);
    ASSERT_TRUE(optimizer.getState().getCurrentPoint().isApprox(solution));
}
