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

/**
 *  @file
 *  @brief  SafeSTLArray serialization
 *  @author  martinelli
 *  @date 2015-05-05
 */

#include "../tests.h"
#include <igatools/utils/safe_stl_array.h>

template <int N>
void array_serialization(const SafeSTLArray<Real,N> &arr)
{
  OUTSTART

  const std::string filename = "array" + std::to_string(N) + ".xml";

  {
    ofstream xml_ostream(filename);
    OArchive xml_out(xml_ostream);
#ifdef USE_CEREAL
    xml_out << CEREAL_NVP(arr);
#else
    xml_out << BOOST_SERIALIZATION_NVP(arr);
#endif
  }

  SafeSTLArray<Real,N> arr_in;
  arr_in.fill(0);
  {
    ifstream xml_istream(filename);
    IArchive xml_in(xml_istream);
    xml_in >> arr_in;
  }

  arr_in.print_info(out);
  out << endl;

  OUTEND
}


int main()
{
  SafeSTLArray<Real,3> arr_3 = {-1.,-2.,-3.};
  array_serialization(arr_3);


  SafeSTLArray<Real,0> arr_0;
  array_serialization(arr_0);

  return 0;
}
