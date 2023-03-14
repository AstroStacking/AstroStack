#include "register.h"
#include <Processing/grey.h>

#include <itkAffineTransform.h>
#include <itkBSplineTransform.h>
#include <itkImageRegistrationMethodv4.h>
#include <itkLandmarkBasedTransformInitializer.h>
#include <itkMeanSquaresImageToImageMetricv4.h>
#include <itkRegularStepGradientDescentOptimizerv4.h>
#include <itkResampleImageFilter.h>
#include <itkTranslationTransform.h>

namespace astro
{
namespace processing
{
namespace
{
template<typename TransformType>
ImageTypePtr registerImages(const ImageTypePtr& fix, const ImageTypePtr& moving,
                            const std::vector<std::pair<double, double>> fixStars,
                            const std::vector<std::pair<double, double>> movingStars, double defaultValue)
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

    for (auto pair : fixStars)
    {
        LandmarkPointType point;
        point[0] = pair.first;
        point[1] = pair.second;
        fixedLandmarks.push_back(point);
    }
    for (auto pair : movingStars)
    {
        LandmarkPointType point;
        point[0] = pair.first;
        point[1] = pair.second;
        movingLandmarks.push_back(point);
    }

    landmarkBasedTransformInitializer->SetBSplineNumberOfControlPoints(4);
    landmarkBasedTransformInitializer->SetReferenceImage(fix);
    landmarkBasedTransformInitializer->SetFixedLandmarks(fixedLandmarks);
    landmarkBasedTransformInitializer->SetMovingLandmarks(movingLandmarks);

    auto transform = TransformType::New();
    transform->SetIdentity();
    landmarkBasedTransformInitializer->SetTransform(transform);
    landmarkBasedTransformInitializer->InitializeTransform();
    ImageType::SpacingType oldSpacing = moving->GetSpacing();
    ImageType::SpacingType spacing{{1, 1}};
    fix->SetSpacing(spacing);
    moving->SetSpacing(spacing);

    using ResampleFilterType = itk::ResampleImageFilter<ImageType, ImageType, double>;
    auto resampleFilter = ResampleFilterType::New();
    resampleFilter->SetInput(moving);
    resampleFilter->SetTransform(transform);
    resampleFilter->SetUseReferenceImage(true);
    resampleFilter->SetReferenceImage(fix);
    resampleFilter->SetDefaultPixelValue(defaultValue);
    resampleFilter->Update();

    auto output = resampleFilter->GetOutput();
    output->SetSpacing(oldSpacing);
    return output;
}
} // namespace

ImageTypePtr registerImages(const ImageTypePtr& fix, const ImageTypePtr& moving,
                            const std::vector<std::pair<double, double>> fixStars,
                            const std::vector<std::pair<double, double>> movingStars, double defaultValue)
{
    return registerImages<itk::AffineTransform<double, Dimension>>(fix, moving, fixStars, movingStars, defaultValue);
}

ImageTypePtr registerImagesBSpline(const ImageTypePtr& fix, const ImageTypePtr& moving,
                                   const std::vector<std::pair<double, double>> fixStars,
                                   const std::vector<std::pair<double, double>> movingStars, double defaultValue)
{
    return registerImages<itk::BSplineTransform<double, Dimension>>(fix, moving, fixStars, movingStars, defaultValue);
}
} // namespace processing
} // namespace astro
