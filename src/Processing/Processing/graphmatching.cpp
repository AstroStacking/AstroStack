#include "graphmatching.h"

#include <IO/hdf5.h>
#include <IO/traits.h>

namespace astro
{
namespace processing
{
namespace
{
constexpr int MAX_ITERATIONS{50};
}

void graphmatching(const H5::DataSet& input, H5::Group& output, const std::string& dataset, int32_t minStars,
                   int32_t maxStars)
{
}
} // namespace processing
} // namespace astro

/*
def match_graph(self, coords0, coords1):
    dist0 = sp.spatial.distance_matrix(coords0, coords0)
    dist1 = sp.spatial.distance_matrix(coords1, coords1)
    best = 1
    bestTrial = []
    
    for trial in itertools.permutations(list(range(len(coords1))), len(coords0)):
        d = dist1[trial, :]
        d = d[:, trial]
        ratio = dist0 / d
        candidate = np.nanmax(np.abs(ratio-1))
        if candidate < best:
            best = candidate
            bestTrial = [(i, j) for i, j in enumerate(trial)]

    if best < self.args.distance_ratio:
        return bestTrial
    return []

def find_full_graph_match(self, coords0, coords1, minFullGraph):
    for trial in itertools.permutations(list(range(minFullGraph+1)), minFullGraph):
        mainGraph = self.match_graph(coords0[trial, :], coords1[:minFullGraph*2])
        if len(mainGraph) > minFullGraph/2:
            return [trial[i] for i, j in mainGraph], [j for i, j in mainGraph]
            
    return [], []
*/
