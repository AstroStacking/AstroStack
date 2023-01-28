#include <Algos/Graph/full_matching.h>

#include <gtest/gtest.h>

TEST(Graph_FullMatching, no_match)
{
    std::vector<std::pair<double, double>> graph1{{0, 0}, {1, 1}, {2, 0}};
    std::vector<std::pair<double, double>> graph2{{0, 0}, {1, 1}, {0, 2}};
    
    ASSERT_THROW(astro::graph::matchFullGraph(graph1, graph2, 3), std::runtime_error);
}

TEST(Graph_FullMatching, some_match)
{
    std::vector<std::pair<double, double>> graph1{{0, 0}, {1, 1}, {2, 0}, {1, -1}};
    std::vector<std::pair<double, double>> graph2{{0, 0}, {-1, -1}, {2, 0}, {1, -1}};
    
    auto result = astro::graph::matchFullGraph(graph1, graph2, 3);
    
    ASSERT_EQ(result.size(), 3);
}
