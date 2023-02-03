#include "partial_matching.h"

#include <Algos/Graph/distance.h>

#include <iostream>
#include <numeric>
#include <unordered_set>


namespace astro
{
namespace graph
{
std::vector<std::pair<size_t, size_t>> matchPartialGraph(std::vector<std::pair<size_t, size_t>> initialGraph,
                                                         const std::vector<std::pair<double, double>>& graph1,
                                                         const std::vector<std::pair<double, double>>& graph2,
                                                         double maxRatio)
{
    std::unordered_set<size_t> originalSet;
    std::unordered_set<size_t> matchSet;

    for (auto pair : initialGraph)
    {
        originalSet.insert(pair.first);
        matchSet.insert(pair.second);
    }

    std::vector<double> distanceMatrix1 = distanceMatrix(graph1);
    std::vector<double> distanceMatrix2 = distanceMatrix(graph2);

    for (size_t i = 0; i < graph1.size(); ++i)
    {
        if (originalSet.count(i) != 0)
        {
            continue;
        }

        double best = std::numeric_limits<double>::infinity();
        std::pair<size_t, size_t> bestTrial{};

        for (size_t j = 0; j < graph2.size(); ++j)
        {
            if (matchSet.count(j) != 0)
            {
                continue;
            }

            double maxRatio = -std::numeric_limits<double>::infinity();
            for (size_t k = 0; k < initialGraph.size(); ++k)
            {
                double ratio = std::abs(distanceMatrix1[initialGraph[k].first * graph1.size() + i] /
                                                distanceMatrix2[initialGraph[k].second * graph2.size() + j] -
                                        1);
                maxRatio = std::max(maxRatio, ratio);
            }

            if (maxRatio < best)
            {
                best = maxRatio;
                bestTrial = std::make_pair(i, j);
            }
        }
        if (best < maxRatio)
        {
            initialGraph.push_back(bestTrial);
        }
    }

    return initialGraph;
}
} // namespace graph
} // namespace astro
