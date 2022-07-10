#pragma once

#include <Algos/optim/state.h>

#include <unsupported/Eigen/CXX11/Tensor>

#include <iostream>

namespace optim
{
namespace helper
{
/// Helper class to do a least square optimization
template<typename Function, int Size = Eigen::Dynamic>
class Quadratic
{
    using Matrix = Eigen::Matrix<double, Size, Size>;
    using Vector = Eigen::Matrix<double, Size, 1>;
    const Function& m_fun;
    const Matrix& m_X;
    const Matrix& m_Y;

public:
    Quadratic(const Function& fun, const Matrix& X, const Matrix& Y)
        : m_fun(fun)
        , m_X(X)
        , m_Y(Y)
    {
    }

    double operator()(const Vector& parameters) const
    {
        auto result = m_fun(m_X, parameters);
        Vector accu = Vector::Zero(result.front().size());
        for (size_t i = 0; i < result.size(); ++i)
        {
            accu = accu + Eigen::pow((m_Y.col(i) - result[i]).array(), 2).matrix();
        }

        return accu.sum();
    }

    Vector gradient(const Vector& parameters) const
    {
        auto result = m_fun(m_X, parameters);
        auto resultGradient = m_fun.gradient(m_X, parameters);

        Vector accu = Vector::Zero(parameters.size());
        for (size_t i = 0; i < result.size(); ++i)
        {
            accu = accu - 2 * (resultGradient[i] * (m_Y.col(i) - result[i]).transpose()).rowwise().sum();
        }
        return accu;
    }

    Matrix firstHessian(const Vector& parameters) const
    {
        auto result = m_fun(m_X, parameters);
        auto resultHessian = m_fun.hessian(m_X, parameters);

        Matrix accu = Matrix::Zero(parameters.size(), parameters.size());
        for (size_t i = 0; i < result.size(); ++i)
        {
            Vector diff = (m_Y.col(i) - result[i]);

            for (size_t j = 0; j < diff.size(); ++j)
            {
                accu = resultHessian[i][j] * diff(j);
            }
        }

        return accu;
    }

    Matrix secondHessian(const Vector& parameters) const
    {
        auto resultGradient = m_fun.gradient(m_X, parameters);

        Matrix accu = Matrix::Zero(parameters.size(), parameters.size());
        for (size_t i = 0; i < resultGradient.size(); ++i)
        {
            accu = accu + resultGradient[i] * resultGradient[i].transpose();
        }
        return accu;
    }

    Matrix hessian(const Vector& parameters) const
    {
        return 2 * (firstHessian(parameters) + secondHessian(parameters));
    }
};
} // namespace helper
} // namespace optim
