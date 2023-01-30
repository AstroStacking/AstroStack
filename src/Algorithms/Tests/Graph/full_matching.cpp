#include <Algos/Graph/full_matching.h>

#include <gtest/gtest.h>

#include <cmath>

TEST(Graph_FullMatching, no_match)
{
    std::vector<std::pair<double, double>> graph1{{0, 0}, {1, 1}, {2, 0}};
    std::vector<std::pair<double, double>> graph2{{0, 0}, {1, 1}, {0, 2}};

    ASSERT_THROW(astro::graph::matchFullGraph(graph1, graph2, 3, 0.001), std::runtime_error);
}

TEST(Graph_FullMatching, some_match)
{
    std::vector<std::pair<double, double>> graph1{{0, 0}, {1, 1}, {2, 0}, {1, -1}, {100, 200}, {1000, 2000}};
    std::vector<std::pair<double, double>> graph2{{0, 0}, {-1, -1}, {6, 0}, {1, -1}, {-100, 200}, {1000, -2000}};

    auto result = astro::graph::matchFullGraph(graph1, graph2, 3, 0.001);

    ASSERT_EQ(result.size(), 3);
    ASSERT_EQ(result[0].first, 0);
    ASSERT_EQ(result[0].second, 1);
    ASSERT_EQ(result[1].first, 1);
    ASSERT_EQ(result[1].second, 0);
    ASSERT_EQ(result[2].first, 2);
    ASSERT_EQ(result[2].second, 3);
}
