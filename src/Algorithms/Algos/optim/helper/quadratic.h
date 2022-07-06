#pragma once

#include <Algos/optim/state.h>

#include <unsupported/Eigen/CXX11/Tensor>

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
        auto result = m_fun(m_X, parameters);
        Eigen::VectorXd accu(result.front().size());
        for(size_t i = 0; i < result.size(); ++i)
        {
            accu = accu + Eigen::pow((m_Y.col(i) - result[i]).array(), 2).matrix();
        }

        return accu.sum();
    }

    Eigen::VectorXd gradient(const Eigen::VectorXd& parameters) const
    {
        auto result = m_fun(m_X, parameters);
        auto resultGradient = m_fun.gradient(m_X, parameters);

        Eigen::VectorXd accu(parameters.size());
        for(size_t i = 0; i < result.size(); ++i)
        {
            accu = accu - 2 * (resultGradient[i] * (m_Y.col(i) - result[i]).transpose()).rowwise().sum();
        }
        return accu;
    }

    Eigen::MatrixXd firstHessian(const Eigen::VectorXd& parameters) const
    {
        auto result = m_fun(m_X, parameters);
        auto resultHessian = m_fun.hessian(m_X, parameters);
        
        Eigen::MatrixXd accu(parameters.size(), parameters.size());
        for(size_t i = 0; i < result.size(); ++i)
        {
            Eigen::VectorXd diff = (m_Y.col(i) - result[i]);
            
            for(size_t j = 0; j < diff.size(); ++j)
            {
                accu = resultHessian[i][j] * diff(j);
            }
        }

        return accu;
    }

    Eigen::MatrixXd secondHessian(const Eigen::VectorXd& parameters) const
    {
        auto resultGradient = m_fun.gradient(m_X, parameters);

        Eigen::MatrixXd accu(parameters.size(), parameters.size());
        for(size_t i = 0; i < resultGradient.size(); ++i)
        {
            accu = accu + resultGradient[i] * resultGradient[i].transpose();
        }
        return accu;
    }

    Eigen::MatrixXd hessian(const Eigen::VectorXd& parameters) const
    {
        return 2 * (firstHessian(parameters) + secondHessian(parameters));
    }
};
} // namespace helper
} // namespace optim
