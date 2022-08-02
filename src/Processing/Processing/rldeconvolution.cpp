#include "rldeconvolution.h"

#include <itkComposeImageFilter.h>
#include <itkImageDuplicator.h>
#include <itkRichardsonLucyDeconvolutionImageFilter.h>
#include <itkVectorIndexSelectionCastImageFilter.h>

namespace astro
{
namespace processing
{
AstroImage RLDeconvolution(AstroImage img, int filterSize, float sigma)
{
    ScalarImageTypePtr kernel = ScalarImageType::New();
    ScalarImageType::SizeType size;
    size[0] = size[1] = 1 + 2 * filterSize;
    ScalarImageType::IndexType start;
    start[0] = start[1] = 0;
    ScalarImageType::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);
    kernel->SetRegions(region);
    kernel->Allocate();

    ScalarImageType::IndexType index;
    for (int j = 0; j < size[1]; ++j)
    {
        index.SetElement(1, j);
        for (int i = 0; i < size[0]; ++i)
        {
            index.SetElement(0, i);
            kernel->SetPixel(index,
                             std::exp(-((i - filterSize) * (i - filterSize) + (j - filterSize) * (j - filterSize)) /
                                      (sigma * sigma)));
        }
    }

    using IndexSelectionType = itk::VectorIndexSelectionCastImageFilter<ImageType, ScalarImageType>;
    using DeconvolutionFilterType = itk::RichardsonLucyDeconvolutionImageFilter<ScalarImageType>;
    using ComposeFilterType = itk::ComposeImageFilter<ScalarImageType, ImageType>;

    auto indexSelectionFilter0 = IndexSelectionType::New();
    auto indexSelectionFilter1 = IndexSelectionType::New();
    auto indexSelectionFilter2 = IndexSelectionType::New();

    indexSelectionFilter0->SetIndex(0);
    indexSelectionFilter0->SetInput(img.getImg());
    indexSelectionFilter0->Update();
    indexSelectionFilter1->SetIndex(1);
    indexSelectionFilter1->SetInput(img.getImg());
    indexSelectionFilter1->Update();
    indexSelectionFilter2->SetIndex(2);
    indexSelectionFilter2->SetInput(img.getImg());
    indexSelectionFilter2->Update();

    auto deconvolutionFilter0 = DeconvolutionFilterType::New();
    auto deconvolutionFilter1 = DeconvolutionFilterType::New();
    auto deconvolutionFilter2 = DeconvolutionFilterType::New();

    deconvolutionFilter0->SetInput(indexSelectionFilter0->GetOutput());
    deconvolutionFilter0->SetKernelImage(kernel);
    deconvolutionFilter0->NormalizeOn();

    deconvolutionFilter1->SetInput(indexSelectionFilter1->GetOutput());
    deconvolutionFilter1->SetKernelImage(kernel);
    deconvolutionFilter1->NormalizeOn();
    deconvolutionFilter2->SetInput(indexSelectionFilter2->GetOutput());
    deconvolutionFilter2->SetKernelImage(kernel);
    deconvolutionFilter2->NormalizeOn();

    auto composeFilter = ComposeFilterType::New();
    composeFilter->SetInput1(deconvolutionFilter0->GetOutput());
    composeFilter->SetInput2(deconvolutionFilter1->GetOutput());
    composeFilter->SetInput3(deconvolutionFilter2->GetOutput());

    composeFilter->Update();
    img.setImg(composeFilter->GetOutput());

    return img;
}
} // namespace processing
} // namespace astro
