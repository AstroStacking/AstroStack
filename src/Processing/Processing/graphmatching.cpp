#include "graphmatching.h"

#include <Algos/Graph/full_matching.h>
#include <Algos/Graph/partial_matching.h>

namespace astro
{
namespace processing
{
std::vector<std::pair<size_t, size_t>> graphmatching(const std::vector<std::pair<double, double>>& graph1,
                                                     const std::vector<std::pair<double, double>>& graph2,
                                                     int fullGraphMatch, double maxRatio)
{
    std::vector<std::pair<size_t, size_t>> graph =
            astro::graph::matchFullGraph(graph1, graph2, fullGraphMatch, maxRatio);
    return astro::graph::matchPartialGraph(graph, graph1, graph2, maxRatio);
}
} // namespace processing
} // namespace astro
