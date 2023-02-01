#include "partial_matching.h"

#include <Algos/Graph/distance.h>

#include <iostream>
#include <numeric>

namespace astro
{
namespace graph
{
std::vector<std::pair<size_t, size_t>> matchPartialGraph(std::vector<std::pair<size_t, size_t>> initialGraph,
                                                         const std::vector<std::pair<double, double>>& graph1,
                                                         const std::vector<std::pair<double, double>>& graph2,
                                                         size_t graphSize, double maxRatio)
{
    return initialGraph;
}
/*
 
 def find_partial_graph_match(self, matchList, coords0, coords1):
     originalSet = set(matchList[0])
     matchSet = set(matchList[1])

     fulldist0 = sp.spatial.distance_matrix(coords0, coords0)
     fulldist1 = sp.spatial.distance_matrix(coords1, coords1)

     for i in range(len(coords0)):
         if i in originalSet:
             continue
         best = 1
         bestTrial = []
         print(matchList)
         dist0 = fulldist0[i, matchList[0]]
         for j in range(0, len(coords1)):
             if j in matchSet:
                 continue
             dist1 = fulldist1[j, matchList[1]]
             ratio = dist0/dist1
             candidate = np.nanmax(np.abs(ratio-1))
             if candidate < best:
                 best = candidate
                 bestTrial = (i, j)
         
         if best < self.args.distance_ratio:
             matchList[0].append(bestTrial[0])
             matchList[1].append(bestTrial[1])
             originalSet.add(bestTrial[0])
             matchSet.add(bestTrial[1])

     return matchList

 */
} // namespace graph
} // namespace astro
