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
#ifndef __itkGaussianSmoothingOnUpdateTimeVaryingVelocityFieldTransform_h
#define __itkGaussianSmoothingOnUpdateTimeVaryingVelocityFieldTransform_h

#include "itkTimeVaryingVelocityFieldTransform.h"

namespace itk
{

/** \class GaussianSmoothingOnUpdateTimeVaryingVelocityFieldTransform
 * \brief Modifies the UpdateTransformParameters method
 * to peform a Gaussian smoothing of the
 * velocity field after adding the update array.
 *
 * This class is the same as \c TimeVaryingVelocityFieldTransform, except
 * for the changes to UpdateTransformParameters. The method smooths
 * the result of the addition of the update array and the displacement
 * field, using a \c GaussianOperator filter.
 *
 * \ingroup ITKDisplacementField
 */
template
  <class TScalar, unsigned int NDimensions>
class ITK_EXPORT GaussianSmoothingOnUpdateTimeVaryingVelocityFieldTransform
: public TimeVaryingVelocityFieldTransform<TScalar, NDimensions>
{
public:
  /** Standard class typedefs. */
  typedef GaussianSmoothingOnUpdateTimeVaryingVelocityFieldTransform   Self;
  typedef TimeVaryingVelocityFieldTransform<TScalar, NDimensions>      Superclass;
  typedef SmartPointer<Self>                                           Pointer;
  typedef SmartPointer<const Self>                                     ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro( GaussianSmoothingOnUpdateTimeVaryingVelocityFieldTransform,
                                                TimeVaryingVelocityFieldTransform );

  /** New macro for creation of through a Smart Pointer */
  itkSimpleNewMacro( Self );

  /** Dimension of the time varying velocity field. */
  itkStaticConstMacro( TimeVaryingVelocityFieldDimension, unsigned int, NDimensions+1 );

  /** Types from superclass */
  typedef typename Superclass::ScalarType                         ScalarType;
  typedef typename Superclass::DerivativeType                     DerivativeType;
  typedef typename DerivativeType::ValueType                      DerivativeValueType;
  typedef typename Superclass::TimeVaryingVelocityFieldType       TimeVaryingVelocityFieldType;
  typedef typename TimeVaryingVelocityFieldType::Pointer          TimeVaryingVelocityFieldPointer;
  typedef typename TimeVaryingVelocityFieldType::PixelType        DisplacementVectorType;
  typedef typename DisplacementVectorType::ValueType              DisplacementVectorValueType;


  /**
   * Get/Set the Gaussian spatial smoothing variance for the update field.
   * Default = 3.
   */
  itkSetMacro( GaussianSpatialSmoothingVarianceForTheUpdateField, ScalarType );
  itkGetConstReferenceMacro( GaussianSpatialSmoothingVarianceForTheUpdateField, ScalarType );

  /**
   * Get/Set the Gaussian temporal smoothing variance for the update field.
   * Default = 1.0.
   */
  itkSetMacro( GaussianTemporalSmoothingVarianceForTheUpdateField, ScalarType );
  itkGetConstReferenceMacro( GaussianTemporalSmoothingVarianceForTheUpdateField, ScalarType );

  /**
   * Get/Set the Gaussian spatial smoothing variance for the total field.
   * Default = 0.5.
   */
  itkSetMacro( GaussianSpatialSmoothingVarianceForTheTotalField, ScalarType );
  itkGetConstReferenceMacro( GaussianSpatialSmoothingVarianceForTheTotalField, ScalarType );

  /**
   * Get/Set the Gaussian temporal smoothing variance for the total field.
   * Default = 0.
   */
  itkSetMacro( GaussianTemporalSmoothingVarianceForTheTotalField, ScalarType );
  itkGetConstReferenceMacro( GaussianTemporalSmoothingVarianceForTheTotalField, ScalarType );

  /** Update the transform's parameters by the values in \c update.
   * We assume \c update is of the same length as Parameters. Throw
   * exception otherwise.
   * \c factor is a scalar multiplier for each value in update.
   * \c GaussianSmoothTimeVaryingVelocityField is called after the update is
   * added to the field.
   * See base class for more details.
   */
  virtual void UpdateTransformParameters( DerivativeType & update, ScalarType factor = 1.0 );

  /** Smooth the displacement field in-place.
   * Uses m_GaussSmoothSigma to change the variance for the GaussianOperator.
   * \warning Not thread safe. Does its own threading.
   */
  virtual TimeVaryingVelocityFieldPointer GaussianSmoothTimeVaryingVelocityField( TimeVaryingVelocityFieldType *, ScalarType, ScalarType );

protected:
  GaussianSmoothingOnUpdateTimeVaryingVelocityFieldTransform();
  virtual ~GaussianSmoothingOnUpdateTimeVaryingVelocityFieldTransform();
  void PrintSelf( std::ostream& os, Indent indent ) const;

  /** Track when the temporary displacement field used during smoothing
   * was last modified/initialized. We only want to change it if the
   * main displacement field is also changed, i.e. assigned to a new object */
  unsigned long                       m_GaussianSmoothingTempFieldModifiedTime;

  /** Used in GaussianSmoothTimeVaryingVelocityField as variance for the
   * GaussianOperator
   */
  ScalarType                        m_GaussianSpatialSmoothingVarianceForTheUpdateField;
  ScalarType                        m_GaussianSpatialSmoothingVarianceForTheTotalField;
  ScalarType                        m_GaussianTemporalSmoothingVarianceForTheUpdateField;
  ScalarType                        m_GaussianTemporalSmoothingVarianceForTheTotalField;

private:
  GaussianSmoothingOnUpdateTimeVaryingVelocityFieldTransform( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

};

} // end namespace itk

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkGaussianSmoothingOnUpdateTimeVaryingVelocityFieldTransform+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkGaussianSmoothingOnUpdateTimeVaryingVelocityFieldTransform.hxx"
#endif

#endif // __itkGaussianSmoothingOnUpdateTimeVaryingVelocityFieldTransform_h
