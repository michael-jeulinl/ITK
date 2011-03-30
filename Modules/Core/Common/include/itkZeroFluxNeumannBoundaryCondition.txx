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
#ifndef __itkZeroFluxNeumannBoundaryCondition_txx
#define __itkZeroFluxNeumannBoundaryCondition_txx
#include "itkZeroFluxNeumannBoundaryCondition.h"
namespace itk
{
template< class TImage >
typename ZeroFluxNeumannBoundaryCondition< TImage >::PixelType
ZeroFluxNeumannBoundaryCondition< TImage >
::operator()(const OffsetType & point_index, const OffsetType & boundary_offset,
             const NeighborhoodType *data) const
{
  int linear_index = 0;

  // Return the value of the pixel at the closest boundary point.
  for ( unsigned int i = 0; i < ImageDimension; ++i )
    {
    linear_index += ( point_index[i] + boundary_offset[i] ) * data->GetStride(i);
    }

  // The reinterpret_cast is necessary, cause we will have a warning if we
  // do not do this. (In fact this function exists for legacy
  // reasons. The overloaded function below should be (and is) used instead).
  // See any of the neighborhood iterators.
  //
  // (data->operator[](linear_index)) is guaranteed to be a pointer to
  // TImage::PixelType except for VectorImage, in which case, it will be a
  // pointer to TImage::InternalPixelType.
  //
  // A typical neighborhood iterator working on an image will use the boundary
  // condition in the following manner:
  //
  // \code
  // // Initialize the functor typically in the constructor.
  // m_NeighborhoodAccessorFunctor = image->GetNeighborhoodAccessor();
  // m_NeighborhoodAccessorFunctor->SetBegin( image->GetBufferPointer() );
  //
  // m_NeighborhoodAccessorFunctor.BoundaryCondition(
  //    point_index, boundary_offset, data, m_ChosenBoundaryCondition );
  // \endcode
  //
  return *( reinterpret_cast< PixelType * >( ( data->operator[](linear_index) ) ) );
}

template< class TImage >
typename ZeroFluxNeumannBoundaryCondition< TImage >::PixelType
ZeroFluxNeumannBoundaryCondition< TImage >
::operator()(const OffsetType & point_index, const OffsetType & boundary_offset,
             const NeighborhoodType *data,
             const NeighborhoodAccessorFunctorType & neighborhoodAccessorFunctor) const
{
  int linear_index = 0;

  // Return the value of the pixel at the closest boundary point.
  for ( unsigned int i = 0; i < ImageDimension; ++i )
    {
    linear_index += ( point_index[i] + boundary_offset[i] ) * data->GetStride(i);
    }

  return neighborhoodAccessorFunctor.Get( data->operator[](linear_index) );
}
} // end namespace itk

#endif