//-+--------------------------------------------------------------------
// Igatools a general purpose Isogeometric analysis library.
// Copyright (C) 2012-2015  by the igatools authors (see authors.txt).
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

#include <igatools/basis_functions/nurbs_space.h>
//#include <igatools/basis_functions/space_tools.h>

//#include <igatools/base/sub_function.h>
//#include <igatools/base/exceptions.h>

#ifdef NURBS


using std::to_string;
using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::bind;
using std::placeholders::_1;
using std::placeholders::_2;

IGA_NAMESPACE_OPEN


#if 0

template <int dim_, int range_, int rank_>
NURBSSpace<dim_, range_, rank_>::
NURBSSpace(const int degree,
           shared_ptr< GridType > knots,
           const WeightsTable &weights)
    :
    NURBSSpace(DegreeTable(TensorIndex<dim>(degree)), knots, weights)
{}



template <int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
create(const int degree,
       shared_ptr< GridType > knots,
       const WeightsTable &weights) -> shared_ptr<self_t>
{
    return shared_ptr<self_t>(new self_t(degree, knots,  weights));
}



template <int dim_, int range_, int rank_>
NURBSSpace<dim_, range_, rank_>::
NURBSSpace(const DegreeTable &degree,
           shared_ptr<GridType> knots,
           const WeightsTable &weights)
    :
    BaseSpace(knots),
    sp_space_(SpSpace::create(degree,knots)),
    weights_(weights)
{

    create_refinement_connection();
    perform_post_construction_checks();
}



template <int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
create(const DegreeTable &degree, shared_ptr<GridType> knots,
       const WeightsTable &weights)
-> shared_ptr<self_t>
{
    return shared_ptr<self_t>(new self_t(degree, knots, weights));
}



template <int dim_, int range_, int rank_>
NURBSSpace<dim_, range_, rank_>::
NURBSSpace(const DegreeTable &deg,
           std::shared_ptr<GridType> knots,
           std::shared_ptr<const MultiplicityTable> interior_mult,
           const EndBehaviourTable &ends,
           const WeightsTable &weights)
    :
    BaseSpace(knots),
    sp_space_(SpSpace::create(deg, knots, interior_mult, ends)),
    weights_(weights)
{
    create_refinement_connection();
    perform_post_construction_checks();
}



template <int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
create(const DegreeTable &deg,
       std::shared_ptr<GridType> knots,
       std::shared_ptr<const MultiplicityTable> interior_mult,
       const EndBehaviourTable &ends,
       const WeightsTable &weights) -> shared_ptr<self_t>
{
    return shared_ptr<self_t>(new self_t(deg, knots, interior_mult, ends, weights));
}

#endif

template <int dim_, int range_, int rank_>
NURBSSpace<dim_, range_, rank_>::
NURBSSpace(std::shared_ptr<SpSpace> bs_space,
           const WeightFunctionPtrTable &weight_func_table)
    :
    BaseSpace(
        bs_space->get_grid(),
        bs_space->get_dof_distribution()),
    sp_space_(bs_space),
    weight_func_table_(weight_func_table)
{
#ifndef NDEBUG
    int comp_id = 0;
    for (const auto &w_func : weight_func_table_)
    {
        Assert(w_func != nullptr, ExcNullPtr());

        Assert(*this->get_grid() == *w_func->get_grid(),ExcMessage("Mismatching grids."));

        using WeightRefSpace = ReferenceSpace<dim_,1,1>;
        auto w_func_as_ref_space = std::dynamic_pointer_cast<const WeightRefSpace>(w_func->get_ig_space());
        Assert(w_func_as_ref_space != nullptr,
               ExcMessage("The space for the weight function is not of type ReferenceSpace<" +
                          std::to_string(WeightRefSpace::dim) + "," +
                          std::to_string(WeightRefSpace::range) + "," +
                          std::to_string(WeightRefSpace::rank) + ">."));


        Assert(w_func_as_ref_space->is_bspline(),
               ExcMessage("The space for the weight function is not BSplineSpace."));

        Assert(sp_space_->get_num_basis_table()[comp_id] ==
               w_func_as_ref_space->get_dof_distribution()->get_num_dofs_table()[0],
               ExcMessage("Mismatching number of basis functions and weight "
                          "coefficients for scalar component " + to_string(comp_id)));

        ++comp_id;
    }
#endif
}


template <int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
create(std::shared_ptr<SpSpace> bs_space,
       const WeightFunctionPtrTable &weight_func_table) -> shared_ptr<self_t>
{
    return shared_ptr<self_t>(new self_t(bs_space,weight_func_table));
}


#if 0
template <int dim_, int range_, int rank_>
void
NURBSSpace<dim_, range_, rank_>::
create_refinement_connection()
{
    // create a signal and a connection for the grid refinement
    this->connect_refinement_h_function(
        bind(&self_t::refine_h_weights, this, std::placeholders::_1, std::placeholders::_2));
}
template <int dim_, int range_, int rank_>
void
NURBSSpace<dim_, range_, rank_>::
perform_post_construction_checks() const
{
#ifndef NDEBUG
    // check that the number of weights is equal to the number of basis functions in the space
    for (auto comp : components)
    {
        Assert(sp_space_->get_num_basis(comp) == weights_[comp].flat_size(),
               ExcDimensionMismatch(sp_space_->get_num_basis(comp),weights_[comp].flat_size()));
    }
#endif
}
#endif

template<int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
get_this_space() const -> std::shared_ptr<const self_t >
{
    auto ref_sp = const_cast<self_t *>(this)->shared_from_this();
    auto nrb_space = std::dynamic_pointer_cast<self_t>(ref_sp);
    Assert(nrb_space != nullptr,ExcNullPtr());

    return nrb_space;
}

template<int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
create_element(const Index flat_index) const
-> std::shared_ptr<SpaceElement<dim_,0,range_,rank_> >
{
    using Elem = NURBSElement<dim_,range_,rank_>;

    const auto nrb_space = this->get_this_space();

    auto elem = make_shared<Elem>(nrb_space,flat_index);
    Assert(elem != nullptr, ExcNullPtr());

    return elem;
}


template <int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
get_weights() const -> const WeightFunctionPtrTable &
{
    return weight_func_table_;
}



#if 0
template <int dim_, int range_, int rank_>
void
NURBSSpace<dim_, range_, rank_>::
reset_weights(const WeightsTable &weights)
{
    weights_ = weights;
    perform_post_construction_checks();
}


template<int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
get_ref_face_space(const Index face_id,
                   SafeSTLVector<Index> &face_to_element_dofs,
                   typename GridType::FaceGridMap &elem_map) const
-> std::shared_ptr<RefFaceSpace>
{
    auto f_space = sp_space_->get_ref_face_space(face_id, face_to_element_dofs, elem_map);

    // TODO (pauletti, Jun 11, 2014): this should be put and completed in
    // get_face_weigjts()
    const auto &v_weights = weights_;
    //const auto &active_dirs = UnitElement<dim>::face_active_directions[face_id];
    typename RefFaceSpace::WeightsTable f_weights(v_weights.get_comp_map());

    const auto n_basis = f_space->get_num_basis_table();
    for (int comp : f_weights.get_active_components_id())
    {
        f_weights[comp].resize(n_basis[comp],1.0);
        //        for (auto j : RefFaceSpace::dims)
        //            f_weights(comp).copy_data_direction(j, v_weights(comp).get_data_direction(active_dirs[j]));
    }


    return RefFaceSpace::create(f_space, f_weights);
}



template<int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
get_face_space(const Index face_id,
               SafeSTLVector<Index> &face_to_element_dofs) const
-> std::shared_ptr<FaceSpace>
{
    auto elem_map = std::make_shared<typename GridType::FaceGridMap>();
    auto face_ref_sp = get_ref_face_space(face_id, face_to_element_dofs, *elem_map);
    auto map  = get_push_forward()->get_mapping();

    auto fmap = MappingSlice<FaceSpace::PushForwardType::dim, FaceSpace::PushForwardType::codim>::
    create(map, face_id, face_ref_sp->get_grid(), elem_map);
    auto fpf = FaceSpace::PushForwardType::create(fmap);
    auto face_space = FaceSpace::create(face_ref_sp,fpf);

    return face_space;
}

template <int dim_, int range_, int rank_>
void
NURBSSpace<dim_, range_, rank_>::
refine_h_weights(
    const SafeSTLArray<bool,dim> &refinement_directions,
    const GridType &grid_old1)
{
    auto grid = this->get_grid();
    auto grid_old = this->get_grid()->get_grid_pre_refinement();

    auto knots_with_repetitions_pre_refinement = sp_space_->get_spline_space_previous_refinement()
                                                 ->compute_knots_with_repetition(
                                                     sp_space_->get_end_behaviour());
    auto knots_with_repetitions = sp_space_->compute_knots_with_repetition(
                                      sp_space_->get_end_behaviour());

    for (int direction_id = 0; direction_id < dim; ++direction_id)
    {
        if (refinement_directions[direction_id])
        {
            for (const int comp_id : weights_.get_active_components_id())
            {
                const int p = sp_space_->get_degree()[comp_id][direction_id];
                const auto &U = knots_with_repetitions_pre_refinement[comp_id].get_data_direction(direction_id);
                const auto &Ubar = knots_with_repetitions[comp_id].get_data_direction(direction_id);


                SafeSTLVector<Real> knots_added(Ubar.size());

                // find the knots in the refined space that are not present in the old space
                auto it = std::set_difference(
                              Ubar.begin(),Ubar.end(),
                              U.begin(),U.end(),
                              knots_added.begin());

                knots_added.resize(it-knots_added.begin());
                const auto &X = knots_added;


                const int m = U.size()-1;
                const int r = X.size()-1;
                const int a = space_tools::find_span(p,X[0],U);
                const int b = space_tools::find_span(p,X[r],U)+1;

                const int n = m-p-1;

                const auto Pw = weights_[comp_id];
                const auto old_sizes = Pw.tensor_size();
                Assert(old_sizes[direction_id] == n+1,
                       ExcDimensionMismatch(old_sizes[direction_id], n+1));

                auto new_sizes = old_sizes;
                new_sizes[direction_id] += r+1; // r+1 new weights in the refinement direction
                Assert(new_sizes[direction_id] ==
                       sp_space_->get_num_basis(comp_id,direction_id),
                       ExcDimensionMismatch(new_sizes[direction_id],
                                            sp_space_->get_num_basis(comp_id,direction_id)));

                DynamicMultiArray<Real,dim> Qw(new_sizes);

                for (Index j = 0; j <= a-p; ++j)
                    Qw.copy_slice(direction_id,j,Pw.get_slice(direction_id,j));

                for (Index j = b-1; j <= n; ++j)
                    Qw.copy_slice(direction_id,j+r+1,Pw.get_slice(direction_id,j));

                Index i = b + p - 1;
                Index k = b + p + r;
                for (Index j = r; j >= 0; --j)
                {
                    while (X[j] <= U[i] && i > a)
                    {
                        Qw.copy_slice(direction_id,k-p-1,Pw.get_slice(direction_id,i-p-1));
                        k = k-1;
                        i = i-1;
                    }
                    Qw.copy_slice(direction_id,k-p-1,
                                  Qw.get_slice(direction_id,k-p));

                    for (Index l = 1; l <= p; ++l)
                    {
                        Index ind = k-p+l;

                        Real alfa = Ubar[k+l] - X[j];
                        if (fabs(alfa) == 0.0)
                        {
                            Qw.copy_slice(direction_id,ind-1,Qw.get_slice(direction_id,ind));
                        }
                        else
                        {
                            alfa = alfa / (Ubar[k+l] - U[i-p+l]);

                            Qw.copy_slice(direction_id,ind-1,
                                          alfa  * Qw.get_slice(direction_id,ind-1) +
                                          (1.0-alfa) * Qw.get_slice(direction_id,ind));
                        }
                    } // end loop l
                    k = k-1;
                } // end loop j

                weights_[comp_id] = Qw;

            } // end loop comp_id

        } // end if (refinement_directions[direction_id])

    } // end loop direction_id

    this->perform_post_construction_checks();
}

#endif


#if 0
template <int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
get_interior_mult() const -> std::shared_ptr<const MultiplicityTable>
{
    return sp_space_->get_interior_mult();
}
#endif


template <int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
get_spline_space() const -> const std::shared_ptr<SpSpace>
{
    return sp_space_;
}



#if 0
template <int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
get_push_forward() -> std::shared_ptr<PushForwardType>
{
    return sp_space_->get_push_forward();
}

template <int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
get_push_forward() const -> std::shared_ptr<const PushForwardType>
{
    return sp_space_->get_push_forward();
}

#endif




template<int dim_, int range_, int rank_>
template<int k>
auto
NURBSSpace<dim_, range_, rank_>::
get_ref_sub_space(const int s_id,
                  InterSpaceMap<k> &dof_map,
                  std::shared_ptr<CartesianGrid<k>> sub_grid) const
-> std::shared_ptr<SubRefSpace<k> >
{
    //TODO (martinelli Nov 27,2014): implement this function
#if 0
    if (!(sub_grid))
    {
        typename GridType::template InterGridMap<k>  elem_map;
        sub_grid   = this->get_grid()->template get_sub_grid<k>(s_id, elem_map);
    }
    auto sub_mult   = this->template get_sub_space_mult<k>(s_id);
    auto sub_degree = this->template get_sub_space_degree<k>(s_id);

    auto sub_space = SubRefSpace<k>::create(sub_degree, sub_grid, sub_mult);

    auto &k_elem = UnitElement<dim>::template get_elem<k>(s_id);

    // Crating the mapping between the space degrees of freedom
    const auto &active_dirs = k_elem.active_directions;
    const int n_dir = k_elem.constant_directions.size();

    TensorIndex<dim> tensor_index;
    int comp_i = 0;
    dof_map.resize(sub_space->get_num_basis());
    for (auto comp : components)
    {
        const int n_basis = sub_space->get_num_basis(comp);
        const auto &sub_local_indices = sub_space->get_dof_distribution_patch().get_index_table()[comp];
        const auto &elem_global_indices = dof_distribution_global_.get_index_table()[comp];

        for (Index sub_i = 0; sub_i < n_basis; ++sub_i, ++comp_i)
        {
            const auto sub_base_id = sub_local_indices.flat_to_tensor(sub_i);

            for (int j=0; j<k; ++j)
                tensor_index[active_dirs[j]] =  sub_base_id[j];
            for (int j=0; j<n_dir; ++j)
            {
                auto dir = k_elem.constant_directions[j];
                auto val = k_elem.constant_values[j];
                const int fixed_id = val * (this->get_num_basis(comp, dir) - 1);
                tensor_index[dir] = fixed_id;

            }
            dof_map[comp_i] = elem_global_indices(tensor_index);
        }

    }

    return sub_space;
#endif
    Assert(false,ExcNotImplemented());
    AssertThrow(false,ExcNotImplemented());
    return nullptr;
}



template<int dim_, int range_, int rank_>
template<int k>
auto
NURBSSpace<dim_, range_, rank_>::
get_sub_space(const int s_id, InterSpaceMap<k> &dof_map,
              std::shared_ptr<CartesianGrid<k>> sub_grid,
              std::shared_ptr<typename GridType::template InterGridMap<k>> elem_map) const
-> std::shared_ptr<SubSpace<k> >
{
    //TODO (martinelli Nov 27,2014): implement this function
#if 0
    using SubMap = SubMapFunction<k, dim, space_dim>;
    auto grid =  this->get_grid();
//    typename GridType::template InterGridMap<k> elem_map;
//    auto sub_grid = this->get_grid()->template get_sub_grid<k>(s_id, elem_map);

    auto sub_ref_space = get_ref_sub_space(s_id, dof_map, sub_grid);
    auto F = IdentityFunction<dim>::create(grid);
    auto sub_map_func = SubMap::create(sub_grid, F, s_id, *elem_map);
    auto sub_space = SubSpace<k>::create(sub_ref_space, sub_map_func);
    return sub_space;
#endif
    Assert(false,ExcNotImplemented());
    AssertThrow(false,ExcNotImplemented());
    return nullptr;
}

template <int dim_, int range_, int rank_>
void
NURBSSpace<dim_, range_, rank_>::
print_info(LogStream &out) const
{
    out.begin_item("BSpline Space:");
    sp_space_->print_info(out);
    out.end_item();

    int comp_id = 0;
    for (const auto &w_func :weight_func_table_)
    {

        out.begin_item("Weight function[" + to_string(comp_id) +"] :");
        w_func->print_info(out);
        out.end_item();

        ++comp_id;
    }
}



template <int dim_, int range_, int rank_>
Real
NURBSSpace<dim_, range_, rank_>::
get_weight_coef_from_basis_id(const Index basis_id) const
{
    //TODO (pauletti, Mar 22, 2015): this looks like a forced function study its use

    const auto &basis_offset = sp_space_->get_basis_offset();

    int comp_id = 0;
    for (; comp_id < n_components-1 ; ++comp_id)
        if (basis_id < basis_offset[comp_id+1])
            break;

    const Index w_id = basis_id - basis_offset[comp_id];

    return (weight_func_table_[comp_id]->get_coefficients())[w_id];
}



template <int dim_, int range_, int rank_>
bool
NURBSSpace<dim_, range_, rank_>::
is_bspline() const
{
    return false;
}

template <int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
get_degree() const -> const DegreeTable &
{
    return this->sp_space_->get_degree();
}

template <int dim_, int range_, int rank_>
void
NURBSSpace<dim_, range_, rank_>::
get_element_dofs(
    const Index element_id,
    SafeSTLVector<Index> &dofs_global,
    SafeSTLVector<Index> &dofs_local_to_patch,
    SafeSTLVector<Index> &dofs_local_to_elem,
    const std::string &dofs_property) const
{
    this->sp_space_->get_element_dofs(
        element_id,
        dofs_global,
        dofs_local_to_patch,
        dofs_local_to_elem,
        dofs_property);
}

template <int dim_, int range_, int rank_>
std::set<Index>
NURBSSpace<dim_, range_, rank_>::
get_interior_dofs() const
{
    return sp_space_->get_interior_dofs();
}


template <int dim_, int range_, int rank_>
std::set<Index>
NURBSSpace<dim_, range_, rank_>::
get_boundary_dofs(const int s_id, const topology_variant &k) const
{
    return sp_space_->get_boundary_dofs(s_id, k);
}


template <int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
get_periodicity() const -> const PeriodicityTable &
{
    return sp_space_->get_periodicity();
}


template <int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
get_end_behaviour_table() const -> const EndBehaviourTable &
{
    return sp_space_->get_end_behaviour_table();
};


template <int dim_, int range_, int rank_>
auto
NURBSSpace<dim_, range_, rank_>::
get_elem_handler() const -> std::shared_ptr<SpaceElementHandler<dim_,0,range_,rank_>>
{
    return ElementHandler::create(this->get_this_space());
}


#ifdef SERIALIZATION
template <int dim_, int range_, int rank_>
template<class Archive>
void
NURBSSpace<dim_, range_, rank_>::
serialize(Archive &ar, const unsigned int version)
{
    ar &boost::serialization::make_nvp("ReferenceSpace",
                                       boost::serialization::base_object<BaseSpace>(*this));

    ar.template register_type<SpSpace>();
    ar &boost::serialization::make_nvp("sp_space_",sp_space_);

    ar &boost::serialization::make_nvp("weight_func_table_",weight_func_table_);
}
///@}
#endif // SERIALIZATION


IGA_NAMESPACE_CLOSE

#include <igatools/basis_functions/nurbs_space.inst>

#endif // #ifdef NURBS

