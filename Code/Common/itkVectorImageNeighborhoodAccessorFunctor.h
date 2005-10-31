/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorImageNeighborhoodAccessorFunctor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorImageNeighborhoodAccessorFunctor_h
#define __itkVectorImageNeighborhoodAccessorFunctor_h

#include "itkVariableLengthVector.h"
#include "itkImageBoundaryCondition.h"
#include "itkNeighborhood.h"
#include "itkImageBase.h"

namespace itk
{
 
/** \class VectorImageNeighborhoodAccessorFunctor
 * \brief Provides accessor interfaces to Access pixels and is meant to be
 * used on pointers to pixels held by the Neighborhood class.
 *
 * A typical user should not need to use this class. The class is internally
 * used by the neighborhood iterators.
 * 
 * \note
 * This work is part of the National Alliance for Medical Image Computing 
 * (NAMIC), funded by the National Institutes of Health through the NIH Roadmap
 * for Medical Research, Grant U54 EB005149.
  */
template< class TImage >
class VectorImageNeighborhoodAccessorFunctor
{
public:
  typedef TImage                                ImageType;
  typedef typename ImageType::PixelType         PixelType;
  typedef typename ImageType::InternalPixelType InternalPixelType;
  typedef unsigned int                          VectorLengthType;
  typedef typename ImageType::OffsetType        OffsetType;

  typedef Neighborhood< InternalPixelType *,
          ::itk::GetImageDimension< TImage >::ImageDimension> NeighborhoodType;

  typedef ImageBoundaryCondition< ImageType > const 
                          *ImageBoundaryConditionConstPointerType;
  
  VectorImageNeighborhoodAccessorFunctor( VectorLengthType length ) 
      : m_VectorLength( length ), m_OffsetMultiplier( length -1 ) { };
  VectorImageNeighborhoodAccessorFunctor() 
      : m_VectorLength( 0 ), m_OffsetMultiplier( 0 ) {};
  
  /** Set the pointer index to the start of the buffer.
   * This must be set by the iterators to the starting location of the buffer. 
   * Typically a neighborhood iterator iterating on a neighborhood of an Image, 
   * say \c image will set this in its constructor. For instance:
   * 
   * \code
   * ConstNeighborhoodIterator( radius, image, )
   *   {
   *   ...
   *   m_NeighborhoodAccessorFunctor.SetBegin( image->GetBufferPointer() );
   *   }
   * \endcode
   */
  inline void SetBegin( const InternalPixelType * begin ) 
    { this->m_Begin = const_cast< InternalPixelType * >( begin ); }

  /** Method to dereference a pixel pointer. This is used from the 
   * ConstNeighborhoodIterator as the equivalent operation to (*it).
   * This method should be preferred over the former (*it) notation. 
   * The reason is that dereferencing a pointer to a location of 
   * VectorImage pixel involves a different operation that simply
   * dereferencing the pointer. Here a PixelType (array of InternalPixelType s)
   * is created on the stack and returned.  */
  inline PixelType Get( const InternalPixelType *pixelPointer ) const 
    { 
    return PixelType(pixelPointer+(pixelPointer-m_Begin)*m_OffsetMultiplier, m_VectorLength);
    }
  
  /** Method to set the pixel value at a certain pixel pointer */
  inline void Set( InternalPixelType* &pixelPointer, const PixelType &p ) const
    {
    InternalPixelType *truePixelPointer = 
      pixelPointer + (pixelPointer-m_Begin)*m_OffsetMultiplier;
    for( VectorLengthType i=0; i< m_VectorLength; i++ ) 
      { 
      truePixelPointer[i] = p[i];
      }
    }
 
  inline PixelType BoundaryCondition( 
      const OffsetType& point_index,
      const OffsetType &boundary_offset, 
      const NeighborhoodType *data,
      const ImageBoundaryConditionConstPointerType boundaryCondition) const
    {
    return boundaryCondition->operator()(point_index, boundary_offset, data, *this);
    }


  /** Methods to Set/Get vector length. This should be the length of a block of
   * pixels in the VectorImage. */
  void SetVectorLength( VectorLengthType length )
    { 
    m_VectorLength = length;
    m_OffsetMultiplier = length - 1;
    }

  /** Methods to Set/Get vector length. This should be the length of a block of
   * pixels in the VectorImage. */
  VectorLengthType GetVectorLength()
    {
    return m_VectorLength;
    }
   
private:
   VectorLengthType m_VectorLength; 
   VectorLengthType m_OffsetMultiplier; // m_OffsetMultiplier = m_VectorLength-1
                                        // (precomputed for speedup).
   InternalPixelType *m_Begin; // Begin of the buffer. 
};


} // end namespace itk
#endif
