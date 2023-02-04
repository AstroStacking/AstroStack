#include "register.h"

#include <itkAffineTransform.h>
#include <itkBSplineTransform.h>
#include <itkLandmarkBasedTransformInitializer.h>
#include <itkResampleImageFilter.h>

namespace astro
{
namespace processing
{

namespace
{
template<typename TransformType>
ImageTypePtr registerImages(const ImageTypePtr& ref, const ImageTypePtr& target,
                            const std::vector<std::pair<double, double>> refStars,
                            const std::vector<std::pair<double, double>> targetStars, double defaultValue)
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
        LandmarkPointType point;
        point[0] = pair.first;
        point[1] = pair.second;
        fixedLandmarks.push_back(point);
    }
    for (auto pair : targetStars)
    {
        LandmarkPointType point;
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

    std::cout << transform << std::endl;

    using ResampleFilterType = itk::ResampleImageFilter<ImageType, ImageType, double>;
    auto resampleFilter = ResampleFilterType::New();
    resampleFilter->SetInput(ref);
    resampleFilter->SetTransform(transform);
    resampleFilter->SetUseReferenceImage(true);
    resampleFilter->SetReferenceImage(target);
    resampleFilter->SetDefaultPixelValue(defaultValue);
    resampleFilter->Update();

    return resampleFilter->GetOutput();
}
} // namespace

ImageTypePtr registerImages(const ImageTypePtr& ref, const ImageTypePtr& target,
                            const std::vector<std::pair<double, double>> refStars,
                            const std::vector<std::pair<double, double>> targetStars, double defaultValue)
{
    return registerImages<itk::AffineTransform<double, Dimension>>(ref, target, refStars, targetStars, defaultValue);
}

ImageTypePtr registerImagesHighQuality(const ImageTypePtr& ref, const ImageTypePtr& target,
                                       const std::vector<std::pair<double, double>> refStars,
                                       const std::vector<std::pair<double, double>> targetStars, double defaultValue)
{
    return registerImages<itk::BSplineTransform<double, Dimension>>(ref, target, refStars, targetStars, defaultValue);
}

} // namespace processing
} // namespace astro
