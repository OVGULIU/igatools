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

#include <igatools/geometry/domain.h>
#include <igatools/geometry/domain_element.h>
#include <igatools/utils/unique_id_generator.h>

using std::shared_ptr;
using std::to_string;

IGA_NAMESPACE_OPEN

template<int dim_, int codim_>
Domain<dim_, codim_>::
Domain(const SharedPtrConstnessHandler<GridFuncType> &func)
  :
  grid_func_(func),
  name_("Domain<" + to_string(dim_) + "," + to_string(codim_) + ">"),
  object_id_(UniqueIdGenerator::get_unique_id())
{}






template<int dim_, int codim_>
auto
Domain<dim_, codim_>::
get_grid_function() const -> std::shared_ptr<const GridFuncType>
{
  return grid_func_.get_ptr_const_data();
}

template<int dim_, int codim_>
const std::string &
Domain<dim_, codim_>::
get_name() const
{
  return name_;
}


template<int dim_, int codim_>
void
Domain<dim_, codim_>::
set_name(const std::string &name)
{
  name_ = name;
}

template<int dim_, int codim_>
int
Domain<dim_, codim_>::
get_object_id() const
{
  return object_id_;
}


template<int dim_, int codim_>
auto
Domain<dim_, codim_>::
create_cache_handler() const
-> std::unique_ptr<ElementHandler>
{
  return std::make_unique<ElementHandler>(this->shared_from_this());
}



template<int dim_, int codim_>
auto
Domain<dim_, codim_>::
create_element(const ListIt &index, const PropId &prop) const
-> std::unique_ptr<ConstElementAccessor>
{
  using Elem = ConstElementAccessor;
  auto elem = std::make_unique<Elem>(this->shared_from_this(), index, prop);
  Assert(elem != nullptr, ExcNullPtr());

  return elem;
}



template<int dim_, int codim_>
auto
Domain<dim_, codim_>::
create_element(const ListIt &index, const PropId &prop)
-> std::unique_ptr<ElementAccessor>
{
  using Elem = ElementAccessor;
  auto elem = std::make_unique<Elem>(this->shared_from_this(), index, prop);
  Assert(elem != nullptr, ExcNullPtr());

  return elem;
}



template<int dim_, int codim_>
auto
Domain<dim_, codim_>::
begin(const PropId &prop) -> ElementIterator
{
  return ElementIterator(this->shared_from_this(),
  grid_func_->get_grid()->get_elements_with_property(prop).begin(), prop);
}



template<int dim_, int codim_>
auto
Domain<dim_, codim_>::
end(const PropId &prop) -> ElementIterator
{
  return ElementIterator(this->shared_from_this(),
  grid_func_->get_grid()->get_elements_with_property(prop).end(),
  prop);
}



template<int dim_, int codim_>
auto
Domain<dim_, codim_>::
begin(const PropId &prop) const -> ElementConstIterator
{
  return this->cbegin(prop);
}



template<int dim_, int codim_>
auto
Domain<dim_, codim_>::
end(const PropId &prop) const -> ElementConstIterator
{
  return this->cend(prop);
}



template<int dim_, int codim_>
auto
Domain<dim_, codim_>::
cbegin(const PropId &prop) const -> ElementConstIterator
{
  return ElementConstIterator(this->shared_from_this(),
                              grid_func_->get_grid()->get_elements_with_property(prop).begin(),
                              prop);
}



template<int dim_, int codim_>
auto
Domain<dim_, codim_>::
cend(const PropId &prop) const -> ElementConstIterator
{
  return ElementConstIterator(this->shared_from_this(),
                              grid_func_->get_grid()->get_elements_with_property(prop).end(),
                              prop);
}


template<int dim_, int codim_>
void
Domain<dim_, codim_>::
print_info(LogStream &out) const
{
  using std::to_string;
  out.begin_item("GridFunction<"
                 + to_string(dim)
                 + ","
                 + to_string(space_dim)
                 +">");
  grid_func_->print_info(out);
  out.end_item();
//    AssertThrow(false,ExcNotImplemented());
}


IGA_NAMESPACE_CLOSE

#include <igatools/geometry/domain.inst>

