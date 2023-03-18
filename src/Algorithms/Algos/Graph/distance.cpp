#include "distance.h"

#include <iostream>
#include <numeric>

namespace astro
{
namespace graph
{
std::vector<double> distanceMatrix(const std::vector<std::pair<double, double>>& points)
{
    std::vector<double> distance(points.size() * points.size());
    for (size_t i = 0; i < points.size(); ++i)
    {
        for (size_t j = 0; j < points.size(); ++j)
        {
            distance[i * points.size() + j] =
                    std::sqrt((points[i].first - points[j].first) * (points[i].first - points[j].first) +
                              (points[i].second - points[j].second) * (points[i].second - points[j].second));
        }
    }

    return distance;
}
} // namespace graph
} // namespace astro
