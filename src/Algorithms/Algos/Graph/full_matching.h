#pragma once

#include <Algos/config.h>

#include <vector>

namespace astro
{
namespace graph
{
ASTRO_ALGORITHMS_EXPORT std::vector<std::pair<size_t, size_t>>
matchFullGraph(const std::vector<std::pair<double, double>>& graph1,
               const std::vector<std::pair<double, double>>& graph2, size_t minFullGraph, double maxRatio);
} // namespace graph
} // namespace astro
