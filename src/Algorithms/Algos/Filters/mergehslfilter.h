/**
 * \file mergehslfilter.h
 */

#pragma once

#include <Algos/config.h>

#include <itkBinaryGeneratorImageFilter.h>
#include <itkMacro.h>
#include <itkNumericTraits.h>

namespace astro
{
namespace filters
{
template<typename TInput1, typename TInput2 = TInput1, typename TOutput = TInput1>
class MergeHSL
{
public:
    MergeHSL() = default;
    ~MergeHSL() = default;
    bool operator==(const MergeHSL&) const { return true; }
    ITK_UNEQUAL_OPERATOR_MEMBER_FUNCTION(MergeHSL);

    TOutput operator()(const TInput1& A, const TInput2& B) const
    {
        TOutput output = A;
        output[2] = B[2];
        return output;
    }
};

template<typename TInputImage1, typename TInputImage2 = TInputImage1, typename TOutputImage = TInputImage1>
class MergeHSLFilter : public itk::BinaryGeneratorImageFilter<TInputImage1, TInputImage2, TOutputImage>
{
public:
    ITK_DISALLOW_COPY_AND_MOVE(MergeHSLFilter);

    using Self = MergeHSLFilter;
    using Superclass = itk::BinaryGeneratorImageFilter<TInputImage1, TInputImage2, TOutputImage>;

    using Pointer = itk::SmartPointer<Self>;
    using ConstPointer = itk::SmartPointer<const Self>;
    using FunctorType = MergeHSL<typename TInputImage1::PixelType, typename TInputImage2::PixelType,
                                 typename TOutputImage::PixelType>;

    itkNewMacro(Self);

    itkTypeMacro(MergeHSLFilter, BinaryGeneratorImageFilter);

protected:
    MergeHSLFilter()
    {
#if !defined(ITK_WRAPPING_PARSER)
        Superclass::SetFunctor(FunctorType());
#endif
    }

    ~MergeHSLFilter() override = default;
};
} // namespace filters

} // namespace astro
