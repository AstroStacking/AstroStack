#pragma once

#include <IO/io.h>

#include <cstddef>

namespace astro
{
namespace image_statistics
{
template<typename... Stats>
class ImageStatistics;

template<typename... Stats>
class Traits
{
};

template<typename Stat>
class Traits<Stat>
{
public:
    static constexpr size_t getNbStats() { return Stat::getNbStats(); }

    template<typename... AllStats>
    static void process(ImageStatistics<AllStats...>& instance, double value, int32_t index,
                        ScalarIntegerImageType::IndexType position)
    {
        static_cast<Stat&>(instance).process(value, index, position);
    }

    template<typename... AllStats>
    static void compute(ImageStatistics<AllStats...>& instance, size_t m_entries)
    {
        static_cast<Stat&>(instance).compute();
    }
};


template<typename Stat, typename... Stats>
class Traits<Stat, Stats...>
{
public:
    static constexpr size_t getNbStats() { return Stat::getNbStats() + Traits<Stats...>::getNbStats(); }

    template<typename... AllStats>
    static void process(ImageStatistics<AllStats...>& instance, double value, int32_t index,
                        ScalarIntegerImageType::IndexType position)
    {
        static_cast<Stat&>(instance).process(value, index, position);
    }

    template<typename... AllStats>
    static void compute(ImageStatistics<AllStats...>& instance, size_t m_entries)
    {
        static_cast<Stat&>(instance).compute();
        for (size_t i = 0; i < m_entries; ++i)
        {
            for (size_t stat = 0; stat < getNbStats(); ++stat)
            {
                instance.setData(static_cast<Stat&>(instance).getData(i, stat),
                                 i * m_entries + m_entries - getNbStats() + stat);
            }
        }
    }
};

} // namespace image_statistics
} // namespace astro
