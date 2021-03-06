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

#include <igatools/functions/function_lib.h>
#include <igatools/functions/identity_function.h>
#include <igatools/basis_functions/bspline.h>
#include <igatools/basis_functions/physical_basis.h>

#include <igatools/io/writer.h>

using namespace iga;
using namespace std;
using numbers::PI;

template<int dim>
void physical_basis(const int deg)
{
  using RefBasis = BSpline<dim>;
  using Basis    = PhysicalBasis<dim>;

  BBox<dim> box;
  box[0] = {{0.5, 1}};
  for (int i=1; i<dim; ++i)
    box[i] = {{PI/4,PI/2}};

  const int n_knots = 3;
  auto grid = Grid<dim>::create(box, n_knots);
  auto ref_basis = RefBasis::create(deg, grid);

  using Function = functions::BallFunction<dim>;
  const auto basis = Basis::create(
                       ref_basis,
                       Function::create(grid, IdentityFunction<dim>::create(grid)));

  const int n_plot_points = 2;
  Writer<dim> writer(basis->get_ptr_const_map_func(), n_plot_points);
  string filename = "ball_geometry-" + to_string(dim) + "d" ;
  writer.save(filename, true);
}


int main()
{
  physical_basis<2>(1);
  return  0;
}
