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
#ifndef __itkLevelSetVelocityNeighborhoodExtractor_h
#define __itkLevelSetVelocityNeighborhoodExtractor_h

#include "itkLevelSetNeighborhoodExtractor.h"

namespace itk
{
/** \class LevelSetVelocityNeighborhoodExtractor
 * \brief Locate pixels of a particular level set.
 *
 * LevelSetVelocityNeighborhoodExtractor extends the functionality of
 * LevelSetNeighborhoodExtractor by also extracting the values
 * of velocity variables at the specified level set. Specifically,
 * it populates two containers: one containing the value of velocity
 * variables immediately inside the contour defined by the level set and the
 * other containing values for velocity variables immediately outside.
 *
 * The containers AuxInsideValues() and AuxOutsideValues() can
 * be used in conjunction with Superclass::InsidePoints() and
 * Superclass::OutsidePoints() in FastMarchingExtensionImageFilter
 * to produce images which extends the velocity variables smoothly
 * from the specified level set.
 *
 * This class is templated over the image type representing
 * the level set, the type of the auxiliary/velocity variables and the
 * number of auxiliary/velocity variables.
 *
 * Implemenation of this class is based on Chapter 11 of
 * "Level Set Methods and Fast Marching Methods", J.A. Sethian,
 * Cambridge Press, Second edition, 1999.
 *
 * \ingroup LevelSetSegmentation
 *
 * \ingroup ITKLevelSets
 */
template<
  class TLevelSet,
  class TAuxValue,
  unsigned int VAuxDimension = 1
  >
class ITK_EXPORT LevelSetVelocityNeighborhoodExtractor:
  public LevelSetNeighborhoodExtractor< TLevelSet >
{
public:
  /** Standard class typdedefs. */
  typedef LevelSetVelocityNeighborhoodExtractor      Self;
  typedef LevelSetNeighborhoodExtractor< TLevelSet > Superclass;
  typedef SmartPointer< Self >                       Pointer;
  typedef SmartPointer< const Self >                 ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LevelSetVelocityNeighborhoodExtractor,
               LevelSetNeighborhoodExtractor);

  /** The type of the level set. */
  typedef LevelSetTypeDefault< TLevelSet > LevelSetType;

  /** The dimension of the level set. */
  itkStaticConstMacro(SetDimension, unsigned int,
                      LevelSetType::SetDimension);

  /** Index typedef support */
  typedef::itk::Index< itkGetStaticConstMacro(SetDimension) > Index;

  /** AuxVarType typedef support. */
  typedef AuxVarTypeDefault< TAuxValue, VAuxDimension, itkGetStaticConstMacro(SetDimension) >
  AuxVarType;
  typedef typename AuxVarType::AuxValueType         AuxValueType;
  typedef typename AuxVarType::AuxValueVectorType   AuxValueVectorType;
  typedef typename AuxVarType::AuxValueContainer    AuxValueContainer;
  typedef typename AuxVarType::AuxImageType         AuxImageType;
  typedef typename AuxVarType::AuxImagePointer      AuxImagePointer;
  typedef typename AuxVarType::AuxImageConstPointer AuxImageConstPointer;

  /** Set the auxiliary images. */
  void SetAuxImage(const AuxImageType *ptr, unsigned int idx = 0)
  {
    if ( idx < VAuxDimension && m_AuxImage[idx] != ptr )
      {
      m_AuxImage[idx] = ptr;
      }
    this->Modified();
  }

  /** Get the auxiliary images. */
  AuxImageConstPointer GetAuxImage(unsigned int idx = 0)
  {
    if ( idx >= VAuxDimension )
      {
      return NULL;
      }
    else
      {
      return m_AuxImage[idx];
      }
  }

  /** Get the container of auxiliary values associated with the inside
   *  points. */
  itkGetObjectMacro(AuxInsideValues, AuxValueContainer);

  /** Get the container of auxiliary values associate with the outside
   *  points. */
  itkGetObjectMacro(AuxOutsideValues, AuxValueContainer);
protected:
  LevelSetVelocityNeighborhoodExtractor();
  ~LevelSetVelocityNeighborhoodExtractor(){}
  void PrintSelf(std::ostream & os, Indent indent) const;

  virtual void Initialize();

  virtual double CalculateDistance(Index & index);

private:
  LevelSetVelocityNeighborhoodExtractor(const Self &); //purposely not
                                                       // implemented
  void operator=(const Self &);                        //purposely not
                                                       // implemented

  typename AuxValueContainer::Pointer m_AuxInsideValues;
  typename AuxValueContainer::Pointer m_AuxOutsideValues;
  AuxImageConstPointer m_AuxImage[VAuxDimension];
};
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLevelSetVelocityNeighborhoodExtractor.hxx"
#endif

#endif
