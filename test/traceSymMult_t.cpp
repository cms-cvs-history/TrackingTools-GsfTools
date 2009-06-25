#include "DataFormats/GeometrySurface/interface/Surface.h"
#include "TrackingTools/TrajectoryParametrization/interface/LocalTrajectoryParameters.h"

#include "DataFormats/GeometrySurface/interface/Plane.h"


#include "FWCore/Utilities/interface/HRRealTime.h"
#include<iostream>

void st(){}
void en(){}


typedef DistanceBetweenComponents<5> Distance; 
typedef KullbackLeiblerDistance<5> KDistance;
typedef SingleGaussianState<5> GS;
typedef GS::Vector Vector;
typedef GS::Matrix Matrix;
typedef ROOT::Math::SMatrix<double,6,6,ROOT::Math::MatRepSym<double,6> > Matrix6;

  Distance const & distance() {
      static Distance * d = new KDistance;
      return *d;
  }

Matrix buildCovariance() {

  // build a resonable covariance matrix as JIJ

  Basic3DVector<float>  axis(0.5,1.,1);
  
  Surface::RotationType rot(axis,0.5*M_PI);

  Surface::PositionType pos( 0., 0., 0.);

  Plane plane(pos,rot);
  LocalTrajectoryParameters tp(1., 1.,1., 0.,0.,1.);

  JacobianLocalToCartesian jl2c(plane,tp);
  return ROOT::Math::SimilarityT(jl2c.jacobian(),Matrix6(ROOT::Math::SMatrixIdentity()));
  // return  ROOT::Math::Transpose(jl2c.jacobian())* jl2c.jacobian();

}


/* compute the trace of a product of two sym matrices
 *   a(i,j)*b(j,i) = a(i,j)*b(i,j) sum over i and j
 */
template<typename T, int N>
double trace(ROOT::Math::SMatrix<T,N,N,ROOT::Math::MatRepSym<T,N> > const & a,
	     ROOT::Math::SMatrix<T,N,N,ROOT::Math::MatRepSym<T,N> > const & b) {
  typedef typename ROOT::Math::SMatrix<T,N,N,ROOT::Math::MatRepSym<T,N> >::const_interator CI;
  CI i1 = a.begin();
  CI e1 = a.end();
  CI i2 = b.begin();
  CI e2 = b.end();
  
  T res =0;
  // sum of the lower triangle;
  for (i1<e1; i1++, i2++)
    res += (*i1)*(*i2);
  res *=2.;
  // remove the duplicated diagonal...
  for (i=0;i<N;i++)
    res -= a(i,i)*a(i,i);

}



int main() {

  // Distance const & d = distance();

  Matrix cov1 = buildCovariance();
  Matrix cov2 = buildCovariance();

  double one = trace(cov1,cov2);

  double two =  GsfMatrixTools::trace<N>(cov1*cov2); 
 
  if (one!=two) std::cout << "vincenzo was wrong!" << std::endl;

  return 0;

}
