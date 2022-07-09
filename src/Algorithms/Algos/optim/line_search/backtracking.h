#pragma once

#include <Algos/optim/state.h>

namespace optim
{
namespace line_search
{
/// Finds an optinal point enforcing Armijo rule
class Backtracking
{
    double m_rho;
    double m_stepSize;
    double m_alphaFactor;

public:
    /**
     * @param rho
     * @param alphaStep
     * @param alphaFactor
     */
    Backtracking(double rho = .1, double stepSize = 1, double alphaFactor = .5)
        : m_rho(rho)
        , m_stepSize(stepSize)
        , m_alphaFactor(alphaFactor)
    {
    }

    template<typename Function>
    Eigen::VectorXd operator()(const Eigen::VectorXd& x, const Function& fun, State& state) const
    {
        double stepSize = std::isnan(state.getInitialStep()) ? m_stepSize : state.getInitialStep();
        double originValue = fun(x);
        double norm = state.getGradient().transpose() * state.getDirection();

        while (true)
        {
            double ftemp = fun(x + stepSize * state.getDirection());
            if (ftemp <= originValue + m_rho * stepSize * norm)
            {
                state.setStep(stepSize);
                return x + stepSize * state.getDirection();
            }
            stepSize *= m_alphaFactor;
        }
    }
};
} // namespace line_search
} // namespace optim
