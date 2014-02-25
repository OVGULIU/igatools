//-+--------------------------------------------------------------------
// Igatools a general purpose Isogeometric analysis library.
// Copyright (C) 2012-2014  by the igatools authors (see authors.txt).
//
// This file is part of the igatools library.
//
// The igatools library is free software: you can use it, redistribute
// it and/or modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//-+--------------------------------------------------------------------

// QualityAssurance: martinelli, 31 Jan 2014

#ifndef DYNAMIC_MULTI_ARRAY_H_
#define DYNAMIC_MULTI_ARRAY_H_

#include <igatools/base/config.h>
#include <igatools/utils/multi_array.h>

IGA_NAMESPACE_OPEN

/**
 * @brief Dynamic multi-dimensional array container, i.e. a tensor-like array
 * container of variable dimension and fixed @p rank.
 *
 * @see MultiArray
 *
 * @ingroup multi_array_containers
 *
 * @author M. Martinelli
 * @date 31 Jan 2014
 *
 *
 */
template<class T, int rank>
class DynamicMultiArray : public MultiArray<std::vector<T>,rank>
{
public:
    /** @name Constructors and destructor */
    ///@{
    /**
     * Construct an empty multiarray.
     */
    DynamicMultiArray() = default;

    /**
     * Construct a square multiarray of zeros with @p dim entries in each
     * array dimension.
     */
    DynamicMultiArray(const Size dim);

    /**
     * Construct a rectangular multiarray of zeros with @p dim[i] entries
     * in the i-th array dimension.
     */
    DynamicMultiArray(const TensorSize<rank> &dim);

    /** Copy constructor. */
    DynamicMultiArray(const DynamicMultiArray<T,rank> &data) = default;

    /** Move constructor. */
    DynamicMultiArray(DynamicMultiArray<T,rank> &&data) = default;

    /** Destructor. */
    ~DynamicMultiArray() = default;
    ///@}


    /** @name Assignment operators */
    ///@{
    /** Copy assignment operator. */
    DynamicMultiArray<T,rank> &operator=(const DynamicMultiArray<T,rank> &data) = default;

    /** Move assignment operator. */
    DynamicMultiArray<T,rank> &operator=(DynamicMultiArray<T,rank> &&data) = default;
    ///@}


    /** @name Functions for changing the size of the dynamic multiarray */
    ///@{

    /**
     * Resize the multiarray as square container with @p dim entries in each
     * array dimension.
     */
    void resize(const Size dim);


    /**
     * Resize the multiarray as rectangular container with <p>dim[i]</p> entries
     * in the i-th array dimension.
     */
    void resize(const TensorSize<rank> &dim);
    ///@}



    //TODO: where do we need this function? (MM 22 Feb 2014)
    /**
     * Extract a sub multi-array from the array and return it
     * as a flat vector.
     * @todo: where do we need this function? (MM 22 Feb 2014)
     */
    std::vector<T> get_flat_view(const TensorIndex<rank> &start,
                                 const TensorIndex<rank> &inc) const;




    /** @name Functions to get/set slices of the DynamicMultiArray */
    ///@{
    /**
     * Return a slice of the DynamicMultiArray corresponding to the @p index
     * in the @p direction specified by the input arguments.
     */
    Conditional<(rank > 0),DynamicMultiArray<T,rank-1>,DynamicMultiArray<T,0> >
    get_slice(const int direction, const Index index) const;

    /**
     * Copy the rank-1 DynamicMultiArray @p slice,
     *  in the slice
     *  corresponding to the @p index in the @p direction
     *  specified by the input arguments.
     */
    void copy_slice(const int direction, const Index index,
                    const Conditional<(rank > 0),DynamicMultiArray<T,rank-1>,
                    DynamicMultiArray<T,0>> &slice);
    ///@}


};


/**
 * Output operator.
 * @relates DynamicMultiArray
 */
template<class T, int rank>
LogStream &operator<<(LogStream &out, const DynamicMultiArray<T,rank> &data);


/**
 * Given a scalar @p a and a DynamicMultiArray @p B,
 * returns a DynamicMultiArray that is the multiplication-by-scalar a * B
 * @note This function is enabled only if @p T is a floating point type
 * and @p rank >= 0.
 *
 * @relates DynamicMultiArray
 */
template<class T, int rank>
Enable_if<std::is_floating_point<T>::value  &&(rank>=0),DynamicMultiArray<T,rank> >
operator*(const Real a, const DynamicMultiArray<T,rank> &B)
{
    DynamicMultiArray<T,rank> res(B);

    for (auto & r : res)
        r *= a;

    return res;
}


/**
 * Given two DynamicMultiArray @p A and @p B,
 * returns a DyqnamicMultiArray that is the sum (entry-by-entry) <tt>A + B</tt>.
 * \warning @p A and @p B must have the same sizes, otherwise an assertion
 * will be raised (in debug mode).
 * @note This function is enabled only if @p T is a floating point type
 * and @p rank >= 0.
 *
 * @relates DynamicMultiArray
 */
template<class T, int rank>
Enable_if< std::is_floating_point<T>::value  &&(rank>=0),DynamicMultiArray<T,rank>>
        operator+(const DynamicMultiArray<T,rank> &A, const DynamicMultiArray<T,rank> &B)
{
    for (Index i = 0 ; i < rank ; ++i)
        Assert(A.tensor_size()[i] == B.tensor_size()[i],
               ExcDimensionMismatch(A.tensor_size()[i],B.tensor_size()[i]));

    DynamicMultiArray<T,rank> res(A);

    const Size n_entries = res.flat_size();
    for (Index i = 0 ; i < n_entries ; ++i)
        res(i) += B(i);

    return res;
}


IGA_NAMESPACE_CLOSE

#endif