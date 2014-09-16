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

#ifndef NURBS_ELEMENT_ACCESSOR_H_
#define NURBS_ELEMENT_ACCESSOR_H_

#include <igatools/base/config.h>
#include <igatools/basis_functions/bspline_element_accessor.h>
#include <igatools/basis_functions/nurbs_uniform_quad_cache.h>

#ifdef NURBS
IGA_NAMESPACE_OPEN

template < int, int , int > class NURBSSpace ;



/**
 * See module on @ref accessors_iterators for a general overview.
 * @ingroup accessors
 */
template <int dim, int range, int rank >
class NURBSElementAccessor :
    public SpaceElementAccessor<NURBSSpace<dim,range,rank>>
{
public:
    using parent_t = SpaceElementAccessor<NURBSSpace<dim,range,rank>>;

    using ContainerType = const NURBSSpace< dim, range, rank>;

    using Space = NURBSSpace<dim,range,rank>;

    /**
     * Typedef for specifying the derivatives of the basis function in the
     * reference domain.
     */
    template <int order>
    using Derivative = typename parent_t::template Derivative<order>;

    using typename parent_t::Point;
    using typename parent_t::Value;


    /** Number of faces of the element. */
    using parent_t::n_faces;



    /** @name Constructors */
    ///@{
    /**
     * Default constructor. Not allowed to be used.
     */
    NURBSElementAccessor() = delete ;

    /**
     * \brief Constructor.
     * \todo Missing documentation.
     */
    NURBSElementAccessor(const std::shared_ptr<const Space> space,
                         const Index elem_index);

    NURBSElementAccessor(const std::shared_ptr<const Space> space,
                         const TensorIndex<dim> &elem_index);

    /**
     * Copy constructor.
     * It can be used with different copy policies (i.e. deep copy or shallow copy).
     * The default behaviour (i.e. using the proper interface of a classic copy constructor)
     * uses the deep copy.
     */
    NURBSElementAccessor(const NURBSElementAccessor<dim,range,rank> &elem,
                         const CopyPolicy &copy_policy = CopyPolicy::deep);

    /**
     * Move constructor.
     */
    NURBSElementAccessor(NURBSElementAccessor<dim,range,rank> &&element) = default;

    /** Destructor.*/
    ~NURBSElementAccessor() = default;
    ///@}

    /** @name Assignment operators */
    ///@{
    /**
     * Copy assignment operator.
     */
    NURBSElementAccessor<dim,range,rank> &
    operator=(const NURBSElementAccessor<dim,range,rank> &element) = default;



    /**
     * Move assignment operator.
     */
    NURBSElementAccessor<dim,range,rank> &
    operator=(NURBSElementAccessor<dim,range,rank> &&element) = default;
    ///@}


    /**
     * Get the NURBS weights associated to the element.
     */
    vector<Real> get_local_weights() const ;


    /**
     * @name Functions reimplemented from CartesianGridElement.
     */
    ///@{
    /**
     * Compare for equality.
     *
     * @note This functions is reimplemented from CartesianGridElement because
     * the NURBSElementAccessor contains the BSplineElementAccessor that must be
     * also used in the comparison.
     */
    bool operator==(const NURBSElementAccessor<dim,range,rank> &a) const;

    /**
     * Compare for inequality.
     *
     * @note This functions is reimplemented from CartesianGridElement because
     * the NURBSElementAccessor contains the BSplineElementAccessor that must be
     * also used in the comparison.
     */
    bool operator!=(const NURBSElementAccessor<dim,range,rank> &a) const;

    /**
     * @note This functions is reimplemented from CartesianGridElement because
     * the NURBSElementAccessor contains the BSplineElementAccessor that must be synchronized
     * (i.e. must have the same element-index) after an index update/reset.
     */
    void operator++();

    /**
     * Sets the index of the element using the flatten representation.
     * @note This function also updates the index for the tensor representation.
     * @warning This may be a dangerous function, be careful when using it
     * as it is easy to use incorrectly. Only use it if you know what you
     * are doing.
     *
     * @note This functions is reimplemented from CartesianGridElement because
     * the NURBSElementAccessor contains the BSplineElementAccessor that must be synchronized
     * (i.e. must have the same element-index) after an index update/reset.
     */
    void move_to(const Index flat_index);

    /**
     * Sets the index of the element using the tensor representation.
     * @note This function also updates the index for the flatten representation.
     * @warning this may be a dangerous function, be careful when using it
     * as it is easy to use incorrectly. Only use it if you know what you
     * are doing.
     *
     * @note This functions is reimplemented from CartesianGridElement because
     * the NURBSElementAccessor contains the BSplineElementAccessor that must be synchronized
     * (i.e. must have the same element-index) after an index update/reset.
     */
    void move_to(const TensorIndex<dim> &tensor_index);
    ///@}

    /** @name Functions for the basis and field evaluations without the use of the cache */
    ///@{

    /**
     * Returns a ValueTable with the <tt>deriv_order</tt>-th derivatives of all local basis function
     * at each point (in the unit domain) specified by the input argument <tt>points</tt>.
     * @note This function does not use the cache and therefore can be called any time without
     * needing to pre-call init_values()/fill_values().
     * @warning The evaluation <tt>points</tt> must belong to the unit hypercube
     * \f$ [0,1]^{\text{dim}} \f$ otherwise, in Debug mode, an assertion will be raised.
     */
    template <int deriv_order>
    ValueTable< Conditional< deriv_order==0,Value,Derivative<deriv_order> > >
    evaluate_basis_derivatives_at_points(const ValueVector<Point> &points) const;

    ///@}

private:

    /**
     * For each component gives a product array of the dimension
     */
    template<class T>
    using ComponentTable = StaticMultiArray<T,range,rank>;



    /**
     * Element accessor used to compute the BSpline basis functions (and derivatives)
     * needed to evaluate ne NURBS basis functions (and derivatives).
     */
    BSplineElementAccessor<dim,range,rank> bspline_element_accessor_;


    template <typename Accessor> friend class GridForwardIterator ;


    template <typename Accessor> friend class GridForwardIterator;
    friend class NURBSUniformQuadCache<dim, range, rank>;

} ;





IGA_NAMESPACE_CLOSE
#endif

#endif /* NURBS_ELEMENT_ACCESSOR_H_ */


