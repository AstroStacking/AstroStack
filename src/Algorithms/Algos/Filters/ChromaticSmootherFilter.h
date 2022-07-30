/**
 * \file ChromaticSmootherFilter.h
 */

#pragma once

#include <itkBinaryGeneratorImageFilter.h>
#include <itkNumericTraits.h>

namespace astro
{
template<typename TInputImage1, typename TInputImage2 = TInputImage1, typename TOutputImage = TInputImage1>
class ASTRO_ALGORITHMS_EXPORT ChromaticSmootherFilter
    : public itk::BinaryGeneratorImageFilter<TInputImage1, TInputImage2, TOutputImage>
{
public:
    ITK_DISALLOW_COPY_AND_MOVE(ChromaticSmootherFilter);

    using Self = ChromaticSmootherFilter;
    using Superclass = itk::BinaryGeneratorImageFilter<TInputImage1, TInputImage2, TOutputImage>;

    using Pointer = itk::SmartPointer<Self>;
    using ConstPointer = itk::SmartPointer<const Self>;
    using FunctorType = Functor::OR<typename TInputImage1::PixelType, typename TInputImage2::PixelType,
                                    typename TOutputImage::PixelType>;

    itkNewMacro(Self);

    itkTypeMacro(ChromaticSmootherFilter, BinaryGeneratorImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
    // Begin concept checking
    itkConceptMacro(Input1Input2OutputBitwiseOperatorsCheck,
                    (itk::Concept::BitwiseOperators<typename TInputImage1::PixelType, typename TInputImage2::PixelType,
                                                    typename TOutputImage::PixelType>) );
    // End concept checking
#endif

protected:
    ChromaticSmootherFilter()
    {
#if !defined(ITK_WRAPPING_PARSER)
        Superclass::SetFunctor(FunctorType());
#endif
    }

    ~ChromaticSmootherFilter() override = default;
};
} // namespace astro
