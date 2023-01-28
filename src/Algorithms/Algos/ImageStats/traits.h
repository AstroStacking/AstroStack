#pragma once

#include <cstddef>

namespace astro
{
namespace image_statistics
{
template<typename... Stats>
class Traits
{
};

template<typename Stat>
class Traits<Stat>
{
public:
    static constexpr size_t getNbStats() { return Stat::getNbStats(); }
};


template<typename Stat, typename... Stats>
class Traits<Stat, Stats...>
{
public:
    static constexpr size_t getNbStats() { return Stat::getNbStats() + Traits<Stats...>::getNbStats(); }
};

} // namespace image_statistics
} // namespace astro
