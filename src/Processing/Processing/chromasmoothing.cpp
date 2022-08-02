#include "chromasmoothing.h"

#include <Algos/Filters/Convertors/HSL2RGB.h>
#include <Algos/Filters/Convertors/RGB2HSL.h>
#include <Algos/Filters/mergehslfilter.h>

#include <itkCastImageFilter.h>
#include <itkComposeImageFilter.h>
#include <itkDiscreteGaussianImageFilter.h>
#include <itkImageAdaptor.h>
#include <itkVectorIndexSelectionCastImageFilter.h>


namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT AstroImage chromaSmoothing(AstroImage img, float variance)
{

    using IndexSelectionType = itk::VectorIndexSelectionCastImageFilter<ImageType, ScalarImageType>;
    using filterType = itk::DiscreteGaussianImageFilter<ScalarImageType, ScalarImageType>;
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

    // Create and setup a Gaussian filter
    auto gaussianFilter0 = filterType::New();
    auto gaussianFilter1 = filterType::New();
    auto gaussianFilter2 = filterType::New();
    gaussianFilter0->SetInput(indexSelectionFilter0->GetOutput());
    gaussianFilter0->SetVariance(variance);
    gaussianFilter0->Update();
    gaussianFilter1->SetInput(indexSelectionFilter1->GetOutput());
    gaussianFilter1->SetVariance(variance);
    gaussianFilter1->Update();
    gaussianFilter2->SetInput(indexSelectionFilter2->GetOutput());
    gaussianFilter2->SetVariance(variance);
    gaussianFilter2->Update();

    auto composeFilter = ComposeFilterType::New();
    composeFilter->SetInput1(gaussianFilter0->GetOutput());
    composeFilter->SetInput2(gaussianFilter1->GetOutput());
    composeFilter->SetInput3(gaussianFilter2->GetOutput());
    composeFilter->Update();

    using RGB2HSLConvertor = itk::ImageAdaptor<ImageType, filters::convertors::HSLPixelAccessor>;
    auto originalImg = RGB2HSLConvertor::New();
    auto smoothImg = RGB2HSLConvertor::New();
    originalImg->SetImage(img.getImg());
    smoothImg->SetImage(composeFilter->GetOutput());
    originalImg->Update();
    smoothImg->Update();

    using MergeFilter = astro::filters::MergeHSLFilter<RGB2HSLConvertor, RGB2HSLConvertor, ImageType>;
    auto mergeFilter = MergeFilter::New();
    mergeFilter->SetInput1(smoothImg);
    mergeFilter->SetInput2(originalImg);
    mergeFilter->Update();

    using HSL2RGBConvertor = itk::ImageAdaptor<ImageType, filters::convertors::RGBPixelAccessor>;
    auto finalImg = HSL2RGBConvertor::New();
    finalImg->SetImage(mergeFilter->GetOutput());
    finalImg->Update();

    using CastFilterType = itk::CastImageFilter<HSL2RGBConvertor, ImageType>;
    auto castFilter = CastFilterType::New();
    castFilter->SetInput(finalImg);
    castFilter->Update();

    return img;
}
} // namespace processing
} // namespace astro
