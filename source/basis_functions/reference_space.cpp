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


#include <igatools/basis_functions/reference_space.h>
#include <igatools/basis_functions/reference_element.h>
#include <igatools/base/array_utils.h>

using std::shared_ptr;
using std::make_shared;
using std::const_pointer_cast;

IGA_NAMESPACE_OPEN

template<int dim, int range, int rank>
const Size ReferenceSpace<dim, range, rank>::n_components;


template<int dim, int range, int rank>
const std::array<Size, ReferenceSpace<dim, range, rank>::n_components>
ReferenceSpace<dim, range, rank>::components =
    sequence<ReferenceSpace<dim, range, rank>::n_components>();

IGA_NAMESPACE_CLOSE

#include <igatools/basis_functions/reference_space.inst>

