#pragma once

#include <Algos/optim/state.h>

#include <iostream>

namespace optim
{
namespace optimizer
{
/// Standard optimizer based on a criterionm line search and step
template<typename Function, typename Criterion, typename LineSearch, typename Step>
class Standard
{
    State<Function::Size> m_state;
    const Function& m_fun;
    Criterion m_criterion;
    LineSearch m_linesearch;
    Step m_step;

    void iterate()
    {
        m_state.setDirection(m_step(m_state.getCurrentPoint(), m_state, m_fun));
        m_state.setCurrentPoint(m_linesearch(m_state.getCurrentPoint(), m_fun, m_state));
        m_state.setCurrentValue(m_fun(m_state.getCurrentPoint()));
    }

public:
    Standard(const Function& fun, Criterion criterion, LineSearch linesearch, Step step)
        : m_fun(fun)
        , m_criterion(std::move(criterion))
        , m_linesearch(std::move(linesearch))
        , m_step(std::move(step))
    {
    }

    typename State<Function::Size>::Vector operator()(const typename State<Function::Size>::Vector& x0)
    {
        m_state.setCurrentPoint(x0);
        m_state.setCurrentValue(m_fun(x0));

        do
        {
            iterate();
            m_state.increaseIteration();
        } while (!m_criterion(m_state));
        return m_state.getCurrentPoint();
    }

    const State<Function::Size>& getState() const { return m_state; }
};

template<typename Function, typename Criterion, typename LineSearch, typename Step>
Standard<Function, Criterion, LineSearch, Step> makeStandard(const Function& fun, Criterion criterion,
                                                             LineSearch linesearch, Step step)
{
    return Standard<Function, Criterion, LineSearch, Step>(fun, std::move(criterion), std::move(linesearch),
                                                           std::move(step));
}
} // namespace optimizer
} // namespace optim
