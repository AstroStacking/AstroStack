#include "maxstacking.h"

#include <Algos/Filters/Stackers/max.h>
#include <Algos/Filters/multifunctorimagefilter.h>


namespace astro
{
namespace processing
{
void maxStacking(const H5::DataSet& inputs, H5::DataSet& output)
{
    H5::DataSpace inputDataspace = inputs.getSpace();
    int ndimsInput = inputDataspace.getSimpleExtentNdims();
    if (ndimsInput != 4)
    {
        throw std::runtime_error("Wrong number of input dimensions");
    }
    hsize_t dimsInput[4];
    ndimsInput = inputDataspace.getSimpleExtentDims(dimsInput, NULL);
    if (dimsInput[3] != 3)
    {
        throw std::runtime_error("Wrong pixel output dimension");
    }
    H5::DataSpace outputDataspace = output.getSpace();
    int ndimsOutput = outputDataspace.getSimpleExtentNdims();
    if (ndimsOutput != 3)
    {
        throw std::runtime_error("Wrong number of output dimensions");
    }
    hsize_t dimsOutput[3];
    ndimsOutput = outputDataspace.getSimpleExtentDims(dimsOutput, NULL);
    if (dimsOutput[2] != 3)
    {
        throw std::runtime_error("Wrong pixel output dimension");
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
            float value = 0;
            for (size_t k = 0; k < dimsInput[0]; ++k)
            {
                value = std::max(value, inputSlab[k * outputSlab.size() + j]);
            }
            outputSlab[j] = value;
        }
        hsize_t startOutput[4]{i, 0, 0};
        outputDataspace.selectHyperslab(H5S_SELECT_SET, countOutput, startOutput);
        output.write(outputSlab.data(), H5::PredType::NATIVE_FLOAT, memoryOutputSpace, outputDataspace);
    }
}
} // namespace processing
} // namespace astro
