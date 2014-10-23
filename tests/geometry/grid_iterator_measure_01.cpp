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

/*
 *  Test for the CartesianGrid Iterator get_measure()
 *
 *  author: pauletti
 *  date: 2014-08-15
 *
 */

#include "../tests.h"

#include <igatools/base/quadrature_lib.h>
#include <igatools/geometry/cartesian_grid.h>
#include <igatools/geometry/grid_element_handler.h>
#include <igatools/geometry/cartesian_grid_element.h>


template <int dim>
void elem_measure(const int n_knots = 5)
{
    OUTSTART

    auto grid = CartesianGrid<dim>::create(n_knots);
    auto flag = NewValueFlags::measure|NewValueFlags::w_measure;

    QGauss<dim> quad(2);
    GridElementHandler<dim> cache(grid, flag, quad);
    auto elem = grid->begin();
    cache.init_element_cache(elem);

    for (; elem != grid->end(); ++elem)
    {
        elem->print_info(out);

        cache.fill_element_cache(elem);
        out << "Measure: " << elem->get_measure() << endl;
        out.begin_item("Weighted Measure:");
        elem->get_w_measures().print_info(out);
        out.end_item();
    }

    OUTEND
}



int main()
{
    elem_measure<1>();
    elem_measure<2>();
    elem_measure<3>();

    return  0;
}
