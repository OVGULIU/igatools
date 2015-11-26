// headers for spline spaces
#include <igatools/basis_functions/bspline.h>
#include <igatools/basis_functions/space_tools.h>
#include <igatools/basis_functions/bspline_element.h>
#include <igatools/basis_functions/bspline_element_handler.h>
#include <igatools/basis_functions/nurbs.h>
#include <igatools/basis_functions/physical_space_basis.h>
// headers for quadrature
#include <igatools/base/quadrature_lib.h>
// headers for linear algebra objects
#include <igatools/linear_algebra/dense_matrix.h>
#include <igatools/linear_algebra/dense_vector.h>
// headers for system solving
//#include <igatools/linear_algebra/epetra_solver.h>
#include <igatools/linear_algebra/dof_tools.h>
// headers for function representations
#include <igatools/functions/function_lib.h>
#include <igatools/functions/function_element.h>
#include <igatools/geometry/grid_function_lib.h>
// headers for output
#include <igatools/io/writer.h>
#include <igatools/base/logstream.h>
// finally! my stuff!
//#include "my_formula_grid_function.h"
#include "custom_grid_function.h"
#include "custom_function.h"

// headers for Trilinos stuff
#include <Teuchos_GlobalMPISession.hpp>
#include <Tpetra_DefaultPlatform.hpp>
#include <Tpetra_Version.hpp>

using namespace iga;
using namespace std;
using namespace EpetraTools;
LogStream out;

#include "poisson_problem.h"

// ----------------------------------------------------------------------------
//   MY CUSTOM FUNCTION
// ----------------------------------------------------------------------------
#define PI 3.141592653589793
template<int dim> // exact solution
Values<dim,1,1> exact_solution(Points<dim> pts) {
  Values<dim,1,1> x = {1.0};
  for (int idim=0; idim<dim; idim++) {
    x *= sin( 4.0 * pts[idim] * PI);
  }
  return x;
}

template<int dim> // source term
Values<dim,1,1> source_term(Points<dim> pts) {
  Values<dim,1,1> x = {16.0 * dim * PI * PI};
  for (int idim=0; idim<dim; idim++) {
    x *= sin(4.0 * pts[idim] * PI);
  }
  return x;
}

// ----------------------------------------------------------------------------
//   MAIN
// ----------------------------------------------------------------------------
int main() {

  // problem dimension
  const int dim = 3;

  // geometry definition
  Geometry<dim> geometry;
  if (dim==2) geometry.load("../../../ring.nurbs");
  if (dim==3) geometry.load("../../../hose.nurbs");

  /*geometry.nel   = {1,1};
  geometry.deg   = {1,2};
  // weights
  geometry.weights[0] = 1.0;
  geometry.weights[1] = 1.0;
  geometry.weights[2] = sqrt(2.0)/2.0;
  geometry.weights[3] = sqrt(2.0)/2.0;
  geometry.weights[4] = 1.0;
  geometry.weights[5] = 1.0;
  // control points
  geometry.coefs[ 0] = 1.0;  geometry.coefs[ 6] = 0.0;
  geometry.coefs[ 1] = 2.0;  geometry.coefs[ 7] = 0.0;
  geometry.coefs[ 2] = 1.0;  geometry.coefs[ 8] = 1.0;
  geometry.coefs[ 3] = 2.0;  geometry.coefs[ 9] = 2.0;
  geometry.coefs[ 4] = 0.0;  geometry.coefs[10] = 1.0;
  geometry.coefs[ 5] = 0.0;  geometry.coefs[11] = 2.0;//*/

  // linear elasticity problem creation
  TensorSize<dim>  nel;
  TensorIndex<dim> deg;
  for (int idim=0; idim<dim; idim++) {
    nel[idim]=4;
    deg[idim]=2;
  }
  auto problem =  ElasticityProblem<dim>(nel,deg);

  problem.assemble(1.0,0.49);
  problem.solve();
  problem.check();
  problem.output();//*/

  /*auto grid    = Grid<dim>::create(3);
  auto space   = SplineSpace<dim,dim>::create(2,grid);
  auto id      = grid_functions::IdentityGridFunction<dim>::create(grid);
  auto domain  = Domain<dim>::create(id);
  auto refbasis= BSpline<dim,dim>::create(space);
  auto basis   = PhysicalSpaceBasis<dim,dim>::create(refbasis,domain);
  auto quad    = QGauss<dim>::create(3);
  grid->refine();
  
  auto handler = basis->create_cache_handler();
  auto el      = basis->begin();
  auto el_end  = basis->end();
  // setting the flags
  using Flags = space_element::Flags;
  auto flag = Flags::value | Flags::gradient | Flags::w_measure | Flags::divergence;
  handler->set_element_flags(flag);
  // setting the quarature rule
  handler->init_element_cache(el,quad);
  for (; el!=el_end; ++el) {
    handler->fill_element_cache(el);
    auto values = el->get_element_values();
    auto grads  = el->get_element_gradients();
    auto divs   = el->get_element_divergences();
    auto w_meas = el->get_element_w_measures();
    auto Nqn = quad->get_num_points();
    auto Nbf = el->get_num_basis(DofProperties::active);
    for (int ibf=0; ibf<Nbf; ibf++) {
      auto grad = grads.get_function_view(ibf);
      for (int iqn=0; iqn<Nqn; iqn++) {
        out << grad[iqn] << endl;
      }
    }
  }//*/


  return 0;
}


