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
 *  Test for bernstein extraction class for periodic bases
 *  author: pauletti
 *  date: 2014-12-22
 *
 */

#include "bernstein_extraction_tests_common.h"
#include <igatools/basis_functions/bernstein_extraction.h>

// TODO (pauletti, Dec 26, 2014): this test needs to be update to current standards

template <int dim>
void
test(const int deg = 1)
{
  using SplineSpace = SplineSpace<dim>;

  typename SplineSpace::DegreeTable degt {{deg}};


  SafeSTLArray<SafeSTLVector<Real>,dim> knots({{0,1,2,3,4}});
  auto grid = Grid<dim>::const_create(knots);

  typename SplineSpace::PeriodicityTable per_t(SafeSTLArray<bool,dim>(true));
  auto int_mult = SplineSpace::get_multiplicity_from_regularity(InteriorReg::maximum,
                  degt, grid->get_num_intervals());
  auto sp_spec = SplineSpace::const_create(degt, grid, int_mult, per_t);
  typename SplineSpace::EndBehaviour endb(BasisEndBehaviour::periodic);
  typename SplineSpace::EndBehaviourTable endb_t { {endb} };

  typename SplineSpace::BoundaryKnotsTable bdry_knots;

  auto rep_knots = compute_knots_with_repetitions(*sp_spec,bdry_knots,endb_t);
  auto acum_mult = sp_spec->accumulated_interior_multiplicities();

  rep_knots.print_info(out);
  out << endl;
  acum_mult.print_info(out);
  out << endl;
  BernsteinExtraction<dim> operators(*grid, rep_knots, acum_mult, degt);
  operators.print_info(out);
}


int main()
{
  out.depth_console(10);
  test<1>();

  return 0;
}
