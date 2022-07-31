#pragma once

#include <Algos/optim/state.h>

#include <unsupported/Eigen/CXX11/Tensor>

#include <iostream>

namespace optim
{
namespace helper
{
/// Helper class to do a least square optimization
template<typename Function, int PSize = Eigen::Dynamic, int XSize = Eigen::Dynamic, int YSize = Eigen::Dynamic>
class Quadratic
{
    using MatrixX = Eigen::Matrix<double, XSize, Eigen::Dynamic>;
    using MatrixY = Eigen::Matrix<double, YSize, Eigen::Dynamic>;
    using MatrixP = Eigen::Matrix<double, PSize, PSize>;
    using VectorX = Eigen::Matrix<double, XSize, 1>;
    using VectorY = Eigen::Matrix<double, YSize, 1>;
    using VectorP = Eigen::Matrix<double, PSize, 1>;
    const Function& m_fun;
    const MatrixX& m_X;
    const MatrixY& m_Y;

public:
    static constexpr int Size = PSize;

    Quadratic(const Function& fun, const MatrixX& X, const MatrixY& Y)
        : m_fun(fun)
        , m_X(X)
        , m_Y(Y)
    {
    }

    double operator()(const VectorP& parameters) const
    {
        auto result = m_fun(m_X, parameters);
        VectorY accu = VectorY::Zero(result.front().size());
        for (size_t i = 0; i < result.size(); ++i)
        {
            accu = accu + Eigen::pow((m_Y.col(i) - result[i]).array(), 2).matrix();
        }

        return accu.sum();
    }

    VectorP gradient(const VectorP& parameters) const
    {
        auto result = m_fun(m_X, parameters);
        auto resultGradient = m_fun.gradient(m_X, parameters);

        VectorP accu = VectorP::Zero(parameters.size());
        for (size_t i = 0; i < result.size(); ++i)
        {
            accu = accu - 2 * (resultGradient[i] * (m_Y.col(i) - result[i])).rowwise().sum();
        }
        return accu;
    }

    MatrixP firstHessian(const VectorP& parameters) const
    {
        auto result = m_fun(m_X, parameters);
        auto resultHessian = m_fun.hessian(m_X, parameters);

        MatrixP accu = MatrixP::Zero(parameters.size(), parameters.size());
        for (size_t i = 0; i < result.size(); ++i)
        {
            VectorY diff = (m_Y.col(i) - result[i]);

            for (size_t j = 0; j < diff.size(); ++j)
            {
                accu = resultHessian[i][j] * diff(j);
            }
        }

        return accu;
    }

    MatrixP secondHessian(const VectorP& parameters) const
    {
        auto resultGradient = m_fun.gradient(m_X, parameters);

        MatrixP accu = MatrixP::Zero(parameters.size(), parameters.size());
        for (size_t i = 0; i < resultGradient.size(); ++i)
        {
            accu = accu + resultGradient[i] * resultGradient[i].transpose();
        }
        return accu;
    }

    MatrixP hessian(const VectorP& parameters) const
    {
        return 2 * (firstHessian(parameters) + secondHessian(parameters));
    }
};
} // namespace helper
} // namespace optim
