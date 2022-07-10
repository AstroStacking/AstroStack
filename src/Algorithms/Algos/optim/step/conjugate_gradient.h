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

    template<typename Function, typename State>
    typename State::Vector operator()(const typename State::Vector& x, State& state, const Function& fun) const
    {
        typename State::Vector gradient = fun.gradient(x);

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
    template<typename Vector>
    static double conjugate(const Vector& newGradient, const Vector& oldGradient,
                            const Vector& direction)
    {
        return static_cast<double>(newGradient.transpose() * (newGradient - oldGradient)) /
               static_cast<double>(direction.transpose() * (newGradient - oldGradient));
    }
};

/// The Dixon formula
class DConjugate
{
public:
    template<typename Vector>
    static double conjugate(const Vector& newGradient, const Vector& oldGradient,
                            const Vector& direction)
    {
        return -static_cast<double>(newGradient.transpose() * newGradient) /
               static_cast<double>(direction.transpose() * oldGradient);
    }
};

/// The Dai Yan formula
class DYConjugate
{
public:
    template<typename Vector>
    static double conjugate(const Vector& newGradient, const Vector& oldGradient,
                            const Vector& direction)
    {
        return static_cast<double>(newGradient.transpose() * newGradient) /
               static_cast<double>(direction.transpose() * (newGradient - oldGradient));
    }
};

/// The Fletcher Reeves formula, needs an exact line search for convergence or the strong Wolfe-Powell rules for an inexact line search
class FRConjugate
{
public:
    template<typename Vector>
    static double conjugate(const Vector& newGradient, const Vector& oldGradient,
                            const Vector& direction)
    {
        return static_cast<double>(newGradient.transpose() * newGradient) /
               static_cast<double>(oldGradient.transpose() * oldGradient);
    }
};

/// The Polak-Ribiere-Polyak formula, can restart automatically, but needs an exact line search with a uniformely convex function to globally converge
class PRPConjugate
{
public:
    template<typename Vector>
    static double conjugate(const Vector& newGradient, const Vector& oldGradient,
                            const Vector& direction)
    {
        return static_cast<double>(newGradient.transpose() * (newGradient - oldGradient)) /
               static_cast<double>(oldGradient.transpose() * oldGradient);
    }
};

/// The Fletcher-Reeves modified Polak-Ribiere-Polyak formula, can restart automatically and has the advantages of the PRP gradient and of the FR gradient
class FRPRPConjugate
{
public:
    template<typename Vector>
    static double conjugate(const Vector& newGradient, const Vector& oldGradient,
                            const Vector& direction)
    {
        double beta = PRPConjugate::conjugate(newGradient, oldGradient, direction);
        double betafr = FRConjugate::conjugate(newGradient, oldGradient, direction);
        if (beta < -betafr)
        {
            beta = -betafr;
        }
        else if (beta > betafr)
        {
            beta = betafr;
        }
        return beta;
    }
};

/// The Hager-Zhang formula, has good convergence capabilities (same as the FR-PRP gradient)
class HZConjugate
{
public:
    template<typename Vector>
    static double conjugate(const Vector& newGradient, const Vector& oldGradient,
                            const Vector& direction)
    {
        auto yk = (newGradient - oldGradient);
        return static_cast<double>((yk - (2 * yk.norm() / static_cast<double>(yk.transpose() * direction)) * direction)
                                           .transpose() *
                                   newGradient) /
               static_cast<double>(yk.transpose() * direction);
    }
};
} // namespace step
} // namespace optim
