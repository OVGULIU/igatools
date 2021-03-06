//-+--------------------------------------------------------------------
// Igatools a general purpose Isogeometric analysis library.
// Copyright (C) 2012-2016  by the igatools authors (see authors.txt).
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
/*
 *  Test for the BSpline element iterator to get the face values
 *  author: pauletti
 *  date: 2013-10-23
 */

#include "../tests.h"

#include <igatools/basis_functions/bspline.h>
#include <igatools/basis_functions/bspline_element.h>
#include <igatools/base/quadrature_lib.h>


template<int dim, int k=dim-1>
void sub_elem_values(const int n_knots, const int deg)
{
  OUTSTART

  auto grid = Grid<dim>::const_create(n_knots);
  auto space = SplineSpace<dim>::const_create(deg, grid);
  using Basis = BSpline<dim>;
  auto basis = Basis::const_create(space);

  const int n_qp = 2;
  auto k_quad = QGauss<k>::create(n_qp);
  auto quad = QGauss<dim>::create(n_qp);
  auto flag = basis_element::Flags::value |
              basis_element::Flags::gradient|
              basis_element::Flags::hessian;

  auto elem_handler = basis->create_cache_handler();
  elem_handler->template set_flags<dim>(flag);
  elem_handler->template set_flags<k>(flag);

  using Elem = typename Basis::ElementAccessor;
  using _Value = typename Elem::_Value;
  using _Gradient = typename Elem::_Gradient;
  using _Hessian = typename Elem::_Hessian;

  auto elem = basis->begin();
  auto end =  basis->end();

  elem_handler->template init_cache<dim>(*elem,quad);
  elem_handler->template init_cache<k>(*elem,k_quad);
  for (; elem != end; ++elem)
  {
    auto &grid_elem =
      dynamic_cast<ReferenceBasisElement<dim,1,1> &>(*elem).get_grid_element();
    if (grid_elem.is_boundary())
    {
      elem_handler->template fill_cache<dim>(*elem,0);
      out << "Element" << grid_elem.get_index() << endl;

      out.begin_item("Basis functions values:");
      elem->template get_basis_data<_Value,dim>(0,DofProperties::active).print_info(out);
      out.end_item();

      out.begin_item("Basis functions gradients:");
      elem->template get_basis_data<_Gradient,dim>(0,DofProperties::active).print_info(out);
      out.end_item();

      out.begin_item("Basis functions hessians:");
      elem->template get_basis_data<_Hessian,dim>(0,DofProperties::active).print_info(out);
      out.end_item();

      for (auto &s_id : UnitElement<dim>::template elems_ids<k>())
      {
        if (grid_elem.is_boundary(s_id))
        {
          elem_handler->template fill_cache<k>(*elem,s_id);
          out.begin_item("Sub Element: " + std::to_string(s_id));

          out.begin_item("Basis functions values:");
          elem->template get_basis_data<_Value,k>(s_id,DofProperties::active).print_info(out);
          out.end_item();

          out.begin_item("Basis functions gradients:");
          elem->template get_basis_data<_Gradient,k>(s_id,DofProperties::active).print_info(out);
          out.end_item();

          out.begin_item("Basis functions hessians:");
          elem->template get_basis_data<_Hessian,k>(s_id,DofProperties::active).print_info(out);
          out.end_item();


          out.end_item();
        } // end elem->is_boundary(s_id)
      } // end loop s_id
    } // end elem->is_boundary()
  } // end loop elem
  OUTEND
}
//  auto values    = elem->template get_values<0,k>(0);
//  auto gradients = elem->template get_values<1,k>(0);
//  auto hessians  = elem->template get_values<2,k>(0);

//  out.begin_item("Values basis functions:");
//  values.print_info(out);
//  out.end_item();

//  out.begin_item("Gradients basis functions:");
//  gradients.print_info(out);
//  out.end_item();
//
//  out.begin_item("Hessians basis functions:");
//  hessians.print_info(out);
//  out.end_item();


//    for (; elem != end; ++elem)
//    {
//        if (elem->is_boundary())
//        {
//            elem->fill_cache();
//            out << "Element" << elem->get_flat_index() << endl;
//            elem->get_basis_values().print_info(out);
//
//            for (int face = 0; face < num_faces; ++face)
//            {
//                if (elem->is_boundary(face))
//                {
//                    elem->fill_face_cache(face);
//                    out << "Face " << face << endl;
//                    out << "values: " << endl;
//                    elem->get_face_basis_values(face).print_info(out);
//                    out << endl;
//                }
//            }
//        }
//    }
//    out << endl;
//}




int main()
{
  out.depth_console(10);
  sub_elem_values<2,1>(2,1);
  sub_elem_values<1,0>(2,1);
  return 0;
}

