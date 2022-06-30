#pragma once

#include <cstddef>

namespace optim
{
namespace criteria
{
enum class StopReason
{
    NOT_STOPPED,
    ITERATION_MAX_REACHED
};

class Iteration
{
    size_t m_iterationMax;

public:
    explicit Iteration(size_t iterationMax)
        : m_iterationMax(iterationMax)
    {
    }

    template<typename State>
    bool operator()(State& state) const
    {
        bool result = state.iteration() >= m_iterationMax;
        if (result)
        {
            state.setStopReason(StopReason::ITERATION_MAX_REACHED);
        }
        return result;
    }
};
} // namespace criteria
} // namespace optim
