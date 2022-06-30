#pragma once

#include <Algos/optim/state.h>

namespace optim
{
namespace criteria
{
/// Combines 2 criteria and returns true if both return true
template<typename Criterion1, typename Criterion2>
class And
{
    Criterion1 m_criterion1;
    Criterion2 m_criterion2;

public:
    explicit And(Criterion1 criterion1, Criterion2 criterion2)
        : m_criterion1(std::move(criterion1))
        , m_criterion2(std::move(criterion2))
    {
    }

    bool operator()(State& state) const { return m_criterion1(state) && m_criterion2(state); }
};

template<typename Criterion1, typename Criterion2>
And<Criterion1, Criterion2> makeAnd(Criterion1 criterion1, Criterion2 criterion2)
{
    return And<Criterion1, Criterion2>(std::move(criterion1), std::move(criterion2));
}

/// Combines 2 criteria and returns true if any returns true
template<typename Criterion1, typename Criterion2>
class Or
{
    Criterion1 m_criterion1;
    Criterion2 m_criterion2;

public:
    explicit Or(Criterion1 criterion1, Criterion2 criterion2)
        : m_criterion1(std::move(criterion1))
        , m_criterion2(std::move(criterion2))
    {
    }

    bool operator()(State& state) const { return m_criterion1(state) || m_criterion2(state); }
};

template<typename Criterion1, typename Criterion2>
Or<Criterion1, Criterion2> makeOr(Criterion1 criterion1, Criterion2 criterion2)
{
    return Or<Criterion1, Criterion2>(std::move(criterion1), std::move(criterion2));
}
} // namespace criteria
} // namespace optim
