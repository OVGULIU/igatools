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

#ifndef NEW_PUSH_FORWARD_ELEMENT_ACCESSOR_H_
#define NEW_PUSH_FORWARD_ELEMENT_ACCESSOR_H_

#include <igatools/geometry/new_push_forward.h>
#include <igatools/geometry/new_mapping_element_accessor.h>

IGA_NAMESPACE_OPEN


template<Transformation type, int dim, int codim = 0>
class PushForwardElement
    : public MappingElement<dim, codim>
{
private:
    using self_t  = PushForwardElement<type, dim, codim>;
    using paren_t = MappingElement<dim, codim>;
    using PF      = NewPushForward<type, dim, codim>;
public:
   // using ContainerType = const PF;
    using paren_t::MappingElement;

    template <int range, int rank>
    using RefValue = typename PF::template RefValue<range, rank>;

    template <int range, int rank>
    using PhysValue = typename PF::template PhysValue<range, rank>;

    template <int range, int rank, int order>
    using RefDerivative = typename PF::template RefDerivative<range, rank, order>;

    template <int range, int rank, int order>
    using PhysDerivative = typename PF::template PhysDerivative<range, rank, order>;


public:

    template <int range, int rank, Transformation ttype=type >
    void
    transform_0(const ValueContainer<RefValue<range, rank>> &v_hat,
                ValueContainer< PhysValue<range, rank> > &v,
                EnableIf<ttype == Transformation::h_grad> * = 0) const
    {
        v = v_hat;
    }


    template <int range, int rank, Transformation ttype=type >
    void
    transform_1(const std::tuple<
                ValueContainer<RefValue<range, rank>>,
                ValueContainer<RefDerivative<range, rank, 1>>> &ref_values,
                const ValueContainer<PhysValue<range, rank>>   &phys_values,
                ValueContainer<PhysDerivative<range, rank, 1>> &Dv,
                EnableIf<ttype == Transformation::h_grad> * = 0) const
    {
        const auto Dv_hat = std::get<1>(ref_values);

        const int n_func   = Dv_hat.get_num_functions();
        const int n_points = Dv_hat.get_num_points();
        auto Dv_it     = Dv.begin();
        auto Dv_hat_it = Dv_hat.cbegin();

        const auto &DF_inv = this->get_inv_gradients();
        for (int i_fn = 0; i_fn < n_func; ++i_fn)
            for (Index j_pt = 0; j_pt < n_points; ++j_pt)
            {
                const auto &DF_inv = DF_inv[j_pt];
                (*Dv_it) = compose((*Dv_hat_it), DF_inv);
                ++Dv_hat_it;
                ++Dv_it;
            }
    }


#if 0
    template <int order, int range, int rank, Transformation ttype=type >
    void
    transform_0(const ValueContainer<RefValue<range, rank>> &D0v_hat,
                ValueContainer< PhysValue<range, rank> > &D0v,
                EnableIf<ttype == Transformation::h_grad> * = 0) const;



    template < int range, int rank, Transformation ttype=type >
    void
    transform_values(
            const ValueContainer< RefValue<range, rank> > &D0v_hat,
            ValueContainer< PhysValue<range, rank> > &D0v,
            EnableIf<ttype == Transformation::h_div> * = 0) const;
#endif

private:
    template <typename Accessor> friend class GridForwardIterator;
    friend class NewPushForward<type, dim, codim>;

};

IGA_NAMESPACE_CLOSE

#endif
