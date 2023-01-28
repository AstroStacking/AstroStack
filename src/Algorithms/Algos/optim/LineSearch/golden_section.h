#pragma once

#include <Algos/Optim/state.h>

namespace optim
{
namespace line_search
{
/// Finds an optinal point with the Golden Section rule
class GoldenSection
{
    double m_minStep;
    double m_stepSize;
    const double m_goldenNumber = (std::sqrt(5) - 1) / 2;

public:
    GoldenSection(double minStep, double stepSize)
        : m_minStep(minStep)
        , m_stepSize(stepSize)
    {
    }

    template<typename Function, typename State>
    typename State::Vector operator()(const typename State::Vector& x, const Function& fun, State& state) const
    {
        const auto& direction = state.getDirection();

        double ak = 0;
        double bk = std::isnan(state.getInitialStep()) ? m_stepSize : state.getInitialStep();
        double v_ak = fun(x);
        double v_bk = fun(x + bk * direction);

        double uk = ak + m_goldenNumber * (bk - ak);
        double v_uk = fun(x + uk * direction);
        double lk = ak + (1 - m_goldenNumber) * (bk - ak);
        double v_lk = fun(x + lk * direction);

        while (true)
        {
            if (v_uk < v_lk)
            {
                if (bk - lk < m_minStep)
                {
                    state.setStep(uk);
                    return x + uk * direction;
                }
                ak = lk;
                v_ak = v_lk;
                lk = uk;
                v_lk = v_uk;
                uk = ak + m_goldenNumber * (bk - ak);
                v_uk = fun(x + uk * direction);
            }
            else
            {
                if (uk - ak < m_minStep)
                {
                    state.setStep(lk);
                    return x + lk * direction;
                }
                bk = uk;
                v_bk = v_uk;
                uk = lk;
                v_uk = v_lk;
                lk = ak + (1 - m_goldenNumber) * (bk - ak);
                v_lk = fun(x + lk * direction);
            }
        }
    }
};
} // namespace line_search
} // namespace optim
