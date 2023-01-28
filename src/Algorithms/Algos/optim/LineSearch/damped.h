#pragma once

#include <Algos/Optim/state.h>

namespace optim
{
namespace line_search
{
/// Finds an optinal point with exponential dampening
class Damped
{
    double m_minStep;
    double m_dampedError;
    double m_stepSize;

public:
    Damped(double minStep, double dampedError, double stepSize = 1)
        : m_minStep(minStep)
        , m_dampedError(dampedError)
        , m_stepSize(stepSize)
    {
    }

    template<typename Function, typename State>
    typename State::Vector operator()(const typename State::Vector& x, const Function& fun, State& state) const
    {
        double stepSize = std::isnan(state.getInitialStep()) ? m_stepSize : state.getInitialStep();
        double currentValue = fun(x);
        typename State::Vector optimalPoint = x + stepSize * state.getDirection();
        double newValue = fun(optimalPoint);

        while (newValue > currentValue * (1 + m_dampedError))
        {
            stepSize /= 2;
            if (stepSize < m_minStep)
            {
                state.setStep(0);
                return x;
            }
            optimalPoint = x + stepSize * state.getDirection();
            newValue = fun(optimalPoint);
        }
        state.setStep(stepSize);
        return optimalPoint;
    }
};
} // namespace line_search
} // namespace optim
