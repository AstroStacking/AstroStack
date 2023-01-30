#pragma once

#include <Algos/config.h>

#include <vector>

namespace astro
{
namespace graph
{
ASTRO_ALGORITHMS_EXPORT std::vector<double> distanceMatrix(const std::vector<std::pair<double, double>>& points);
} // namespace graph
} // namespace astro
