#include "full_matching.h"

#include <numeric>

namespace astro
{
namespace graph
{
namespace
{
void comb(const std::vector<size_t>& sofar, const std::vector<size_t>& rest, int n)
{
    if (n == 0)
    {
        // process
    }
    else
    {
        for (size_t i = 0; i < rest.size(); i++)
        {
            std::vector<size_t> newCombination = sofar;
            newCombination.push_back(rest[i]);
            std::vector<size_t> newRest = sofar;
            newRest.erase(newRest.begin() + i);

            comb(newCombination, newRest, n - 1);
        }
    }
}

} // namespace

std::vector<std::pair<size_t, size_t>> matchFullGraph(const std::vector<std::pair<double, double>>& graph1,
                                                      const std::vector<std::pair<double, double>>& graph2,
                                                      size_t minFullGraph)
{
    std::vector<size_t> indicesGraph1(minFullGraph * 2);
    std::iota(indicesGraph1.begin(), indicesGraph1.end(), 0);
    std::vector<size_t> indicesGraph2 = indicesGraph1;

    std::vector<double> distanceMatrix1 = distanceMatrix({graph1.begin(), graph1.begin() + minFullGraph * 2});
    std::vector<double> distanceMatrix2 = distanceMatrix({graph2.begin(), graph2.begin() + minFullGraph * 2});

    throw std::runtime_error("Could not find graph matches");
}

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
