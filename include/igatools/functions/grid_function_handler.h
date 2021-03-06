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

#ifndef __GRID_FUNCTION_HANDLER_H_
#define __GRID_FUNCTION_HANDLER_H_

#include <igatools/base/config.h>
#include <igatools/geometry/grid.h>
#include <igatools/functions/grid_function.h>
#include <igatools/geometry/grid_handler.h>

IGA_NAMESPACE_OPEN

template <int, int> class GridFunctionElement;

/**
 *
 * @ingroup handlers
 */
template<int dim_, int range_>
class GridFunctionHandler
//    :
//  public std::enable_shared_from_this<GridFunctionHandler<dim_,range_> >
{
private:
  using self_t = GridFunctionHandler<dim_, range_>;

public:
  static const int range = range_;
  static const int dim = dim_;

  using GridFunctionType = const GridFunction<dim_, range_>;
  using GridType = const Grid<dim_>;
  using GridHandler = typename GridType::Handler;

  using ElementAccessor = GridFunctionElement<dim_, range_>;
  using ElementIterator = GridIterator<ElementAccessor>;


  using List = typename GridType::List;
  using ListIt = typename GridType::ListIt;
  using Flags = grid_function_element::Flags;

protected:
  using FlagsArray = SafeSTLArray<Flags, dim+1>;

  using topology_variant = TopologyVariants<dim_>;

  template<int k>
  using ConstQuad = const Quadrature<k>;
  using eval_pts_variant = SubElemPtrVariants<ConstQuad,dim_>;

private:
  GridFunctionHandler() = delete;

public:
  GridFunctionHandler(const std::shared_ptr<GridFunctionType> &grid_function);


  virtual ~GridFunctionHandler() = default;


  std::shared_ptr<GridFunctionType> get_grid_function() const;


public:
  virtual void set_flags(const topology_variant &sdim,
                         const Flags &flag);

  template <int sdim>
  void set_flags(const Flags &flag);

  void set_element_flags(const Flags &flag);

  virtual void init_cache(ElementAccessor &elem,
                          const eval_pts_variant &quad) const;

  void init_cache(ElementIterator &elem,
                  const eval_pts_variant &quad) const;

  void init_element_cache(ElementAccessor &elem,
                          const std::shared_ptr<const Quadrature<dim_>> &quad) const;

  void init_element_cache(ElementIterator &elem,
                          const std::shared_ptr<const Quadrature<dim_>> &quad) const;

  virtual void fill_cache(const topology_variant &sdim,
                          ElementAccessor &elem,
                          const int s_id) const = 0;

  void fill_cache(const topology_variant &sdim,
                  ElementIterator &elem,
                  const int s_id) const;

  template <int sdim>
  void fill_cache(ElementIterator &elem,
                  const int s_id) const;

  template <int sdim>
  void fill_cache(ElementAccessor &elem,
                  const int s_id) const;

  void fill_element_cache(ElementAccessor &elem) const;

  void fill_element_cache(ElementIterator &elem) const;

  //protected:
public:
  const GridHandler &
  get_grid_handler() const;

  GridHandler &
  get_grid_handler();

protected:
  typename ElementAccessor::CacheType &
  get_element_cache(ElementAccessor &elem) const;
//*/

private:
  /**
   * Alternative to
   * template <int sdim> set_flags()
   */
  struct SetFlagsDispatcher : boost::static_visitor<void>
  {
    SetFlagsDispatcher(const Flags flag, FlagsArray &flags);

    template<int sdim>
    void operator()(const Topology<sdim> &);

    const Flags flag_;
    FlagsArray &flags_;
  };



  struct InitCacheDispatcher : boost::static_visitor<void>
  {
    InitCacheDispatcher(const self_t &grid_function_handler,
                        ElementAccessor &elem,
                        const FlagsArray &flags);


    template<int sdim>
    void operator()(const std::shared_ptr<const Quadrature<sdim>> &quad);

    const self_t &grid_function_handler_;
    ElementAccessor &elem_;
    const FlagsArray &flags_;
  };




private:
  std::shared_ptr<GridFunctionType> grid_function_;

protected:
  std::unique_ptr<GridHandler> grid_handler_;

private:
  FlagsArray flags_;

//  friend ElementAccessor;
};



IGA_NAMESPACE_CLOSE

#endif

