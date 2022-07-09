#pragma once

#include <Algos/optim/state.h>

#include <iostream>

namespace optim
{
namespace line_search
{
/// Finds an optinal point with exponential dampening
class Damped
{
    double m_minStep;
    double m_dampedError;
    double m_length;

public:
    Damped(double minStep, double dampedError, double length=1)
        : m_minStep(minStep), m_dampedError(dampedError), m_length(length)
    {
    }

    template<typename Function>
    Eigen::VectorXd operator()(const Eigen::VectorXd& x, const Function& fun, State& state) const
    {
        double stepSize = std::isnan(state.getInitialStep()) ? m_length : state.getInitialStep();
        double currentValue = fun(x);
        Eigen::VectorXd optimalPoint = x + stepSize * state.getDirection();
        double newValue = fun(optimalPoint);
        
        while(newValue > currentValue * (1 + m_dampedError))
        {
            stepSize /= 2;
            if(stepSize < m_minStep)
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
