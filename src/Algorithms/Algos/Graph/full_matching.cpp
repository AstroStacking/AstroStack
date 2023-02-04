#include "full_matching.h"

#include <Algos/Graph/distance.h>

#include <iostream>
#include <numeric>

namespace astro
{
namespace graph
{
namespace
{
template<typename Fun>
void comb(const std::vector<size_t>& sofar, const std::vector<size_t>& rest, size_t n, Fun& fun)
{
    if (n == 0)
    {
        fun(sofar);
    }
    else
    {
        for (size_t i = 0; i < rest.size(); i++)
        {
            std::vector<size_t> newCombination = sofar;
            newCombination.push_back(rest[i]);
            std::vector<size_t> newRest = rest;
            newRest.erase(newRest.begin() + i);

            comb(newCombination, newRest, n - 1, fun);
            if (fun.stop())
            {
                break;
            }
        }
    }
}

std::vector<double> extractDistance(const std::vector<double>& distanceMatrix, const std::vector<size_t>& permutation,
                                    size_t size)
{
    std::vector<double> result(permutation.size() * permutation.size());
    for (size_t i = 0; i < permutation.size(); ++i)
    {
        for (size_t j = 0; j < permutation.size(); ++j)
        {
            result[i * permutation.size() + j] = distanceMatrix[permutation[i] * size + permutation[j]];
        }
    }
    return result;
}

/// Match a subset of points with another graph
class PartialGraphMatch
{
    const std::vector<double>& m_distanceMatrix1;
    const std::vector<double>& m_distanceMatrix2;

    const size_t m_sizeGraph2;
    const double m_maxRatio;

    std::vector<size_t> m_bestTrial;
    double m_bestRatio{std::numeric_limits<double>::max()};

public:
    PartialGraphMatch(const std::vector<double>& distanceMatrix1, const std::vector<double>& distanceMatrix2,
                      size_t sizeGraph2, double maxRatio)
        : m_distanceMatrix1(distanceMatrix1)
        , m_distanceMatrix2(distanceMatrix2)
        , m_sizeGraph2(sizeGraph2)
        , m_maxRatio(maxRatio)
    {
    }

    void operator()(const std::vector<size_t>& permutation)
    {
        std::vector<double> partialDistanceMatrix = extractDistance(m_distanceMatrix2, permutation, m_sizeGraph2);
        double d = 0;
        for (size_t i = 0; i < partialDistanceMatrix.size(); ++i)
        {
            if (m_distanceMatrix1[i] != 0)
            {
                d = std::max(d, std::abs(partialDistanceMatrix[i] / m_distanceMatrix1[i] - 1));
            }
        }

        if (m_bestRatio > d)
        {
            m_bestRatio = d;
            m_bestTrial = permutation;
        }
    }

    bool stop() const { return false; }


    std::vector<size_t> best() const
    {
        if (!m_bestTrial.empty() && m_bestRatio < m_maxRatio)
        {
            return m_bestTrial;
        }
        throw std::runtime_error("Could not find partial graph matches");
    }
};

/// Matches two sides of a matrix
class FullGraphMatch
{
    const std::vector<double> m_distanceMatrix1;
    const std::vector<double> m_distanceMatrix2;

    std::vector<std::pair<size_t, size_t>> m_bestTrial;
    const size_t m_sizeGraph;
    const size_t m_sizeGraph1;
    const size_t m_sizeGraph2;
    const double m_maxRatio;

public:
    FullGraphMatch(const std::vector<std::pair<double, double>>& graph1,
                   const std::vector<std::pair<double, double>>& graph2, size_t sizeGraph, double maxRatio)
        : m_distanceMatrix1(distanceMatrix({graph1.begin(), graph1.begin() + sizeGraph * 2}))
        , m_distanceMatrix2(distanceMatrix({graph2.begin(), graph2.begin() + sizeGraph * 2}))
        , m_sizeGraph(sizeGraph)
        , m_sizeGraph1(graph1.size())
        , m_sizeGraph2(graph2.size())
        , m_maxRatio(maxRatio)
    {
    }

    void operator()(const std::vector<size_t>& permutation)
    {
        std::vector<size_t> indicesGraph2(m_sizeGraph * 2);
        std::iota(indicesGraph2.begin(), indicesGraph2.end(), 0);

        std::vector<double> partialDistanceMatrix = extractDistance(m_distanceMatrix1, permutation, m_sizeGraph * 2);
        PartialGraphMatch matching(partialDistanceMatrix, m_distanceMatrix2, m_sizeGraph * 2, m_maxRatio);
        comb(std::vector<size_t>(), indicesGraph2, m_sizeGraph, matching);

        try
        {
            matching.best();
        }
        catch (const std::runtime_error& e)
        {
            return;
        }
        m_bestTrial.resize(permutation.size());
        for (size_t i = 0; i < permutation.size(); ++i)
        {
            m_bestTrial[i] = std::make_pair(permutation[i], matching.best()[i]);
        }
    }

    bool stop() const { return !m_bestTrial.empty(); }

    std::vector<std::pair<size_t, size_t>> best() const
    {
        if (!m_bestTrial.empty())
        {
            return m_bestTrial;
        }
        throw std::runtime_error("Could not find graph matches");
    }
};
} // namespace

std::vector<std::pair<size_t, size_t>> matchFullGraph(const std::vector<std::pair<double, double>>& graph1,
                                                      const std::vector<std::pair<double, double>>& graph2,
                                                      size_t graphSize, double maxRatio)
{
    std::vector<size_t> indicesGraph1(graphSize * 2);
    std::iota(indicesGraph1.begin(), indicesGraph1.end(), 0);

    FullGraphMatch matching(graph1, graph2, graphSize, maxRatio);
    comb(std::vector<size_t>(), indicesGraph1, graphSize, matching);

    return matching.best();
}
} // namespace graph
} // namespace astro
