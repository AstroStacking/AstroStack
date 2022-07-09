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

        if(state.getDirection().size() > 0)
        {
            double coeff = Conjugated::conjugate(gradient, state.getGradient(), state.getDirection());
            state.setGradient(gradient);
            state.setData("conjugate_coefficient", coeff);
            return - gradient + coeff * state.getDirection();
        }
        state.setData("conjugate_coefficient", 0);
        state.setGradient(gradient);
        return -gradient;

    }
};
} // namespace step
} // namespace optim
