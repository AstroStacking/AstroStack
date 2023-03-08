#include "stacking.h"

#include <Algos/Filters/Stackers/max.h>
#include <Algos/Filters/Stackers/median.h>
#include <Algos/Filters/Stackers/robustmean.h>
#include <Algos/Filters/multifunctorimagefilter.h>
#include <IO/traits.h>

namespace astro
{
namespace processing
{
template<typename Stacker>
void stacking(const H5::DataSet& inputs, H5::DataSet& output, const Stacker& stacker,
              std::optional<std::function<void(int)>> updateTask)
{
    H5::DataSpace inputDataspace = inputs.getSpace();
    int ndimsInput = inputDataspace.getSimpleExtentNdims();
    if (ndimsInput != Traits<ImageType>::NbDimensions + 1)
    {
        throw std::runtime_error("Wrong number of input dimensions, got " + std::to_string(ndimsInput) +
                                 " instead of " + std::to_string(Traits<ImageType>::NbDimensions + 1));
    }
    hsize_t dimsInput[Traits<ImageType>::NbDimensions + 1];
    ndimsInput = inputDataspace.getSimpleExtentDims(dimsInput, nullptr);
    if (dimsInput[Traits<ImageType>::NbDimensions] != Traits<ImageType>::LastDim)
    {
        throw std::runtime_error("Wrong pixel input dimension, got " +
                                 std::to_string(dimsInput[Traits<ImageType>::NbDimensions]) + " instead of " +
                                 std::to_string(Traits<ImageType>::LastDim));
    }
    H5::DataSpace outputDataspace = output.getSpace();
    int ndimsOutput = outputDataspace.getSimpleExtentNdims();
    if (ndimsOutput != Traits<ImageType>::NbDimensions)
    {
        throw std::runtime_error("Wrong number of output dimensions, got " + std::to_string(ndimsOutput) +
                                 " instead of " + std::to_string(Traits<ImageType>::NbDimensions));
    }
    hsize_t dimsOutput[Traits<ImageType>::NbDimensions];
    ndimsOutput = outputDataspace.getSimpleExtentDims(dimsOutput, nullptr);
    if (dimsOutput[Traits<ImageType>::NbDimensions - 1] != Traits<ImageType>::LastDim)
    {
        throw std::runtime_error("Wrong pixel output dimension, got " +
                                 std::to_string(dimsOutput[Traits<ImageType>::NbDimensions - 1]) + " instead of " +
                                 std::to_string(Traits<ImageType>::LastDim));
    }

    if (dimsInput[2] != dimsOutput[1])
    {
        throw std::runtime_error("Input dimension 2 doesn't match output dimension 1, got " +
                                 std::to_string(dimsInput[2]) + " and " + std::to_string(dimsOutput[1]));
    }
    if (dimsInput[3] != dimsOutput[2])
    {
        throw std::runtime_error("Input dimension 3 doesn't match output dimension 2, got " +
                                 std::to_string(dimsInput[3]) + " and " + std::to_string(dimsOutput[2]));
    }

    hsize_t countInput[4]{dimsInput[0], 1, dimsInput[2], dimsInput[3]};
    std::vector<UnderlyingPixelType> inputSlab(dimsInput[0] * dimsInput[2] * dimsInput[3]);

    hsize_t countOutput[3]{1, dimsOutput[1], dimsOutput[2]};
    std::vector<UnderlyingPixelType> outputSlab(dimsOutput[1] * dimsOutput[2]);

    hsize_t dimInput[] = {static_cast<hsize_t>(inputSlab.size())};
    H5::DataSpace memoryInputSpace(1, dimInput);
    hsize_t dimOuput[] = {static_cast<hsize_t>(outputSlab.size())};
    H5::DataSpace memoryOutputSpace(1, dimOuput);

    for (hsize_t i = 0; i < dimsInput[1]; ++i)
    {
        hsize_t startInput[4]{0, i, 0, 0};
        inputDataspace.selectHyperslab(H5S_SELECT_SET, countInput, startInput);
        inputs.read(inputSlab.data(), H5::PredType::NATIVE_FLOAT, memoryInputSpace, inputDataspace);

        for (size_t j = 0; j < outputSlab.size(); ++j)
        {
            std::vector<float> values;
            for (size_t k = 0; k < dimsInput[0]; ++k)
            {
                values.push_back(inputSlab[k * outputSlab.size() + j]);
            }
            outputSlab[j] = stacker(values);
        }
        hsize_t startOutput[4]{i, 0, 0};
        outputDataspace.selectHyperslab(H5S_SELECT_SET, countOutput, startOutput);
        output.write(outputSlab.data(), H5::PredType::NATIVE_FLOAT, memoryOutputSpace, outputDataspace);
        if (updateTask)
        {
            (*updateTask)(i);
        }
    }
}

template void stacking<astro::filters::stackers::Max<float>>(const H5::DataSet& inputs, H5::DataSet& output,
                                                             const astro::filters::stackers::Max<float>& stacker,
                                                             std::optional<std::function<void(int)>> updateTask);
template void stacking<astro::filters::stackers::Median<float>>(const H5::DataSet& inputs, H5::DataSet& output,
                                                                const astro::filters::stackers::Median<float>& stacker,
                                                                std::optional<std::function<void(int)>> updateTask);
template void
stacking<astro::filters::stackers::RobustMean<float>>(const H5::DataSet& inputs, H5::DataSet& output,
                                                      const astro::filters::stackers::RobustMean<float>& stacker,
                                                      std::optional<std::function<void(int)>> updateTask);
} // namespace processing
} // namespace astro
