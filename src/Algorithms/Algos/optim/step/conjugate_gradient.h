#pragma once

#include <Algos/optim/state.h>

namespace optim
{
namespace step
{
/// Computes a direction based on the gradient
template<typename Conjugated>
class ConjugateGradient
{
public:
    ConjugateGradient() = default;

    template<typename Function>
    Eigen::VectorXd operator()(const Eigen::VectorXd& x, State& state, const Function& fun) const
    {
        Eigen::VectorXd gradient = fun.gradient(x);

        if (state.getDirection().size() > 0)
        {
            double coeff = Conjugated::conjugate(gradient, state.getGradient(), state.getDirection());
            state.setGradient(gradient);
            state.setData("conjugate_coefficient", coeff);
            return -gradient + coeff * state.getDirection();
        }
        state.setData("conjugate_coefficient", 0);
        state.setGradient(gradient);
        return -gradient;
    }
};

/// The Crowder-Wolfe or Hestenes-Stiefel formula
class CWConjugate
{
public:
    static double conjugate(const Eigen::VectorXd& newGradient, const Eigen::VectorXd& oldGradient,
                            const Eigen::VectorXd& direction)
    {
        return static_cast<double>(newGradient.transpose() * (newGradient - oldGradient)) /
               static_cast<double>(direction.transpose() * (newGradient - oldGradient));
    }
};

/// The Dixon formula
class DConjugate
{
public:
    static double conjugate(const Eigen::VectorXd& newGradient, const Eigen::VectorXd& oldGradient,
                            const Eigen::VectorXd& direction)
    {
        return -static_cast<double>(newGradient.transpose() * newGradient) /
               static_cast<double>(direction.transpose() * oldGradient);
    }
};

/// The Dai Yan formula
class DYConjugate
{
public:
    static double conjugate(const Eigen::VectorXd& newGradient, const Eigen::VectorXd& oldGradient,
                            const Eigen::VectorXd& direction)
    {
        return static_cast<double>(newGradient.transpose() * newGradient) /
               static_cast<double>(direction.transpose() * (newGradient - oldGradient));
    }
};
} // namespace step
} // namespace optim
