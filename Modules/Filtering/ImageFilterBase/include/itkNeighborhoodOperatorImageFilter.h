/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkNeighborhoodOperatorImageFilter_h
#define __itkNeighborhoodOperatorImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkNeighborhoodOperator.h"
#include "itkImage.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"

namespace itk
{
/** \class NeighborhoodOperatorImageFilter
 * \brief Applies a single NeighborhoodOperator to an image region.
 *
 * This filter calculates successive inner products between a single
 * NeighborhoodOperator and a NeighborhoodIterator, which is swept
 * across every pixel in an image region.  For operators that are
 * symmetric across their axes, the result is a fast convolution
 * with the image region.  Apply the mirror()'d operator for
 * non-symmetric NeighborhoodOperators.
 *
 * \ingroup ImageFilters
 *
 * \sa Image
 * \sa Neighborhood
 * \sa NeighborhoodOperator
 * \sa NeighborhoodIterator
 * \ingroup ITKImageFilterBase
 *
 * \wiki
 * \wikiexample{Images/NeighborhoodOperatorImageFilter,Apply a kernel to every pixel in an image}
 * \endwiki
 */
template< class TInputImage, class TOutputImage, class TOperatorValueType = typename TOutputImage::PixelType >
class ITK_EXPORT NeighborhoodOperatorImageFilter:
  public ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard "Self" & Superclass typedef. */
  typedef NeighborhoodOperatorImageFilter                 Self;
  typedef ImageToImageFilter< TInputImage, TOutputImage > Superclass;
  typedef       SmartPointer< Self >                      Pointer;
  typedef SmartPointer< const Self >                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(NeighborhoodOperatorImageFilter, ImageToImageFilter);

  /** Extract some information from the image types.  Dimensionality
   * of the two images is assumed to be the same. */
  typedef typename TOutputImage::PixelType         OutputPixelType;
  typedef typename TOutputImage::InternalPixelType OutputInternalPixelType;
  typedef typename  TInputImage::PixelType         InputPixelType;
  typedef typename  TInputImage::InternalPixelType InputInternalPixelType;
  typedef TOperatorValueType                       OperatorValueType;

  typedef typename NumericTraits<InputPixelType>::ValueType InputPixelValueType;
  typedef typename NumericTraits<OutputPixelType>::RealType ComputingPixelType;

  /** Extract some information from the image types.  Dimensionality
   * of the two images is assumed to be the same. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Image typedef support. */
  typedef TInputImage                      InputImageType;
  typedef TOutputImage                     OutputImageType;
  typedef typename InputImageType::Pointer InputImagePointer;

  /** Typedef for generic boundary condition pointer. */
  typedef ImageBoundaryCondition< InputImageType > *
  ImageBoundaryConditionPointerType;

  /** Typedef for the default boundary condition */
  typedef ZeroFluxNeumannBoundaryCondition< InputImageType > DefaultBoundaryCondition;

  /** Superclass typedefs. */
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;

  /** Neighborhood types */
  typedef Neighborhood< OperatorValueType,
                        itkGetStaticConstMacro(ImageDimension) > OutputNeighborhoodType;

  /** Sets the operator that is used to filter the image. Note
   * that the operator is stored as an internal COPY (it
   * is not part of the pipeline). */
  void SetOperator(const OutputNeighborhoodType & p)
  {
    m_Operator = p;
    this->Modified();
  }

  /** Get the operator that is used to filter the image. */
  const OutputNeighborhoodType & GetOperator() const
  { return m_Operator; }

  /** Allows a user to override the internal boundary condition. Care should be
   * be taken to ensure that the overriding boundary condition is a persistent
   * object during the time it is referenced.  The overriding condition
   * can be of a different type than the default type as long as it is
   * a subclass of ImageBoundaryCondition. */
  void OverrideBoundaryCondition(const ImageBoundaryConditionPointerType i)
  { m_BoundsCondition = i; }

  /** Get the boundary condition specified */
  ImageBoundaryConditionPointerType GetBoundaryCondition()
  { return m_BoundsCondition; }

  /** NeighborhoodOperatorImageFilter needs a larger input requested
   * region than the output requested region.  As such,
   * NeighborhoodOperatorImageFilter needs to provide an implementation for
   * GenerateInputRequestedRegion() in order to inform the pipeline
   * execution model.
   *
   * \sa ProcessObject::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion()
  throw ( InvalidRequestedRegionError );

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro( SameDimensionCheck,
                   ( Concept::SameDimension< InputImageDimension, ImageDimension > ) );
  itkConceptMacro( OperatorConvertibleToOutputCheck,
                   ( Concept::Convertible< OperatorValueType, OutputPixelType > ) );
  itkConceptMacro( InputConvertibleToOperatorCheck,
                   ( Concept::Convertible< InputPixelValueType, OperatorValueType > ) );
  itkConceptMacro( OperatorMultiplyOperatorCheck,
                   ( Concept::MultiplyOperator< OperatorValueType > ) );
  itkConceptMacro( OperatorAdditiveOperatorsCheck,
                   ( Concept::AdditiveOperators< OperatorValueType > ) );
  /** End concept checking */
#endif
protected:
  NeighborhoodOperatorImageFilter()
  { m_BoundsCondition = static_cast< ImageBoundaryConditionPointerType >( &m_DefaultBoundaryCondition ); }
  virtual ~NeighborhoodOperatorImageFilter() {}

  /** NeighborhoodOperatorImageFilter can be implemented as a
   * multithreaded filter.  Therefore, this implementation provides a
   * ThreadedGenerateData() routine which is called for each
   * processing thread. The output image data is allocated
   * automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to
   * the portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                            ThreadIdType threadId);

  void PrintSelf(std::ostream & os, Indent indent) const
  {  Superclass::PrintSelf(os, indent); }
private:
  NeighborhoodOperatorImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);                  //purposely not implemented

  /** Internal operator used to filter the image. */
  OutputNeighborhoodType m_Operator;

  /** Pointer to a persistent boundary condition object used
   * for the image iterator. */
  ImageBoundaryConditionPointerType m_BoundsCondition;

  /** Default boundary condition */
  DefaultBoundaryCondition m_DefaultBoundaryCondition;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkNeighborhoodOperatorImageFilter.hxx"
#endif

#endif
