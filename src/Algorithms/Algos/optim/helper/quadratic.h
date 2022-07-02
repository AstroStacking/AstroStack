#pragma once

#include <Algos/optim/state.h>

#include <iostream>

namespace optim
{
namespace helper
{
/// Helper class to do a least square optimization
template<typename Function>
class Quadratic
{
    const Function& m_fun;
    const Eigen::MatrixXd& m_X;
    const Eigen::MatrixXd& m_Y;

public:
    Quadratic(const Function& fun, const Eigen::MatrixXd& X, const Eigen::MatrixXd& Y)
        : m_fun(fun)
        , m_X(X)
        , m_Y(Y)
    {
    }

    double operator()(const Eigen::VectorXd& parameters) const
    {
        return Eigen::pow((m_fun(m_X, parameters) - m_Y).array(), 2).sum();
    }

    Eigen::VectorXd gradient(const Eigen::VectorXd& parameters) const
    {
        return 2 * (m_fun.gradient(m_X, parameters) * (m_fun(m_X, parameters) - m_Y).transpose());
    }
};
} // namespace helper
} // namespace optim
