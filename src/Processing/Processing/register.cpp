#include "register.h"

#include <itkAffineTransform.h>
#include <itkBSplineTransform.h>
#include <itkComposeImageFilter.h>
#include <itkLandmarkBasedTransformInitializer.h>
#include <itkResampleImageFilter.h>
#include <itkVectorIndexSelectionCastImageFilter.h>

namespace astro
{
namespace processing
{

namespace
{
template<typename TransformType>
ImageTypePtr registerImages(const ImageTypePtr& ref, const ImageTypePtr& target,
                            const std::vector<std::pair<double, double>> refStars,
                            const std::vector<std::pair<double, double>> targetStars)
{
    using LandmarkBasedTransformInitializerType =
            itk::LandmarkBasedTransformInitializer<TransformType, ImageType, ImageType>;

    typename LandmarkBasedTransformInitializerType::Pointer landmarkBasedTransformInitializer =
            LandmarkBasedTransformInitializerType::New();
    //  Create source and target landmarks.
    using LandmarkContainerType = typename LandmarkBasedTransformInitializerType::LandmarkPointContainer;
    using LandmarkPointType = typename LandmarkBasedTransformInitializerType::LandmarkPointType;

    LandmarkContainerType fixedLandmarks;
    LandmarkContainerType movingLandmarks;

    for (auto pair : refStars)
    {
        itk::Point<double, 2> point;
        point[0] = pair.first;
        point[1] = pair.second;
        fixedLandmarks.push_back(point);
    }
    for (auto pair : targetStars)
    {
        itk::Point<double, 2> point;
        point[0] = pair.first;
        point[1] = pair.second;
        movingLandmarks.push_back(point);
    }

    landmarkBasedTransformInitializer->SetFixedLandmarks(fixedLandmarks);
    landmarkBasedTransformInitializer->SetMovingLandmarks(movingLandmarks);

    auto transform = TransformType::New();

    transform->SetIdentity();
    landmarkBasedTransformInitializer->SetTransform(transform);
    landmarkBasedTransformInitializer->InitializeTransform();

    using IndexSelectionType = itk::VectorIndexSelectionCastImageFilter<ImageType, ScalarImageType>;

    auto indexSelectionFilter0 = IndexSelectionType::New();
    auto indexSelectionFilter1 = IndexSelectionType::New();
    auto indexSelectionFilter2 = IndexSelectionType::New();

    indexSelectionFilter0->SetIndex(0);
    indexSelectionFilter0->SetInput(target);
    indexSelectionFilter0->Update();
    indexSelectionFilter1->SetIndex(1);
    indexSelectionFilter1->SetInput(target);
    indexSelectionFilter1->Update();
    indexSelectionFilter2->SetIndex(2);
    indexSelectionFilter2->SetInput(target);
    indexSelectionFilter2->Update();

    using ResampleFilterType = itk::ResampleImageFilter<ScalarImageType, ScalarImageType, double>;
    auto resampleFilter0 = ResampleFilterType::New();
    resampleFilter0->SetInput(indexSelectionFilter0->GetOutput());
    resampleFilter0->SetTransform(transform);
    resampleFilter0->SetUseReferenceImage(true);
    resampleFilter0->SetReferenceImage(ref);
    resampleFilter0->SetDefaultPixelValue(std::numeric_limits<double>::quiet_NaN());
    auto resampleFilter1 = ResampleFilterType::New();
    resampleFilter1->SetInput(indexSelectionFilter1->GetOutput());
    resampleFilter1->SetTransform(transform);
    resampleFilter1->SetUseReferenceImage(true);
    resampleFilter1->SetReferenceImage(ref);
    resampleFilter1->SetDefaultPixelValue(std::numeric_limits<double>::quiet_NaN());
    auto resampleFilter2 = ResampleFilterType::New();
    resampleFilter2->SetInput(indexSelectionFilter2->GetOutput());
    resampleFilter2->SetTransform(transform);
    resampleFilter2->SetUseReferenceImage(true);
    resampleFilter2->SetReferenceImage(ref);
    resampleFilter2->SetDefaultPixelValue(std::numeric_limits<double>::quiet_NaN());

    using ComposeFilterType = itk::ComposeImageFilter<ScalarImageType, ImageType>;

    auto composeFilter = ComposeFilterType::New();
    composeFilter->SetInput1(resampleFilter0->GetOutput());
    composeFilter->SetInput2(resampleFilter1->GetOutput());
    composeFilter->SetInput3(resampleFilter2->GetOutput());
    composeFilter->Update();

    return composeFilter->GetOutput();
}
} // namespace

ImageTypePtr registerImages(const ImageTypePtr& ref, const ImageTypePtr& target,
                            const std::vector<std::pair<double, double>> refStars,
                            const std::vector<std::pair<double, double>> targetStars)
{
    return registerImages<itk::AffineTransform<double, Dimension>>(ref, target, refStars, targetStars);
}

ImageTypePtr registerImagesHighQuality(const ImageTypePtr& ref, const ImageTypePtr& target,
                                       const std::vector<std::pair<double, double>> refStars,
                                       const std::vector<std::pair<double, double>> targetStars)
{
    return registerImages<itk::BSplineTransform<double, Dimension>>(ref, target, refStars, targetStars);
}

} // namespace processing
} // namespace astro
