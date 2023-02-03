#include <Algos/Graph/partial_matching.h>

#include <gtest/gtest.h>

#include <cmath>

TEST(Graph_PartialMatching, no_match)
{
    std::vector<std::pair<double, double>> graph1{{0, 0}, {1, 1}, {2, 0}, {1, -1}, {100, 200}, {1000, 2000}, {-2, 0}};
    std::vector<std::pair<double, double>> graph2{{0, 0}, {-1, -1}, {6, 0}, {1, -1}, {-100, 200}, {-3, -1}};

    std::vector<std::pair<size_t, size_t>> initialGraph{{0, 1}, {1, 0}, {2, 3}};

    std::vector<std::pair<size_t, size_t>> fullGraph =
            astro::graph::matchPartialGraph(initialGraph, graph1, graph2, 0.0001);

    ASSERT_EQ(fullGraph.size(), 4);
    ASSERT_EQ(fullGraph[3].first, 6);
    ASSERT_EQ(fullGraph[3].second, 5);
}
