#include "TrackingTools/GsfTools/interface/KullbackLeiblerDistance.h"
#include "TrackingTools/GsfTools/interface/DistanceBetweenComponents.h"

#include "TrackingTools/AnalyticalJacobians/interface/JacobianLocalToCartesian.h"
#include "TrackingTools/AnalyticalJacobians/interface/JacobianCartesianToLocal.h"
#include "TrackingTools/AnalyticalJacobians/interface/JacobianLocalToCurvilinear.h"

#include "DataFormats/GeometrySurface/interface/Surface.h"
#include "TrackingTools/TrajectoryParametrization/interface/LocalTrajectoryParameters.h"

#include "DataFormats/GeometrySurface/interface/Plane.h"

#include "TrackingTools/GsfTools/interface/GsfMatrixTools.h"


#include "FWCore/Utilities/interface/HRRealTime.h"
#include<iostream>

void st(){}
void en(){}


typedef DistanceBetweenComponents<5> Distance; 
typedef KullbackLeiblerDistance<5> KDistance;
typedef SingleGaussianState<5>  __attribute__ ((aligned (16))) GS;
typedef GS::Vector  __attribute__ ((aligned (16))) Vector;
typedef GS::Matrix  __attribute__ ((aligned (16))) Matrix;
typedef ROOT::Math::SMatrix<double,6,6,ROOT::Math::MatRepSym<double,6> >  __attribute__ ((aligned (16))) Matrix6;

  Distance const & distance() {
      static Distance * d = new KDistance;
      return *d;
  }

Matrix buildCovariance(float y) {

  // build a resonable covariance matrix as JIJ

  Basic3DVector<float>  axis(0.5,1.,1);
  
  Surface::RotationType rot(axis,0.5*M_PI);

  Surface::PositionType pos( 0., 0., 0.);

  Plane plane(pos,rot);
  LocalTrajectoryParameters tp(1., 1.,y, 0.,0.,1.);

  JacobianLocalToCartesian jl2c(plane,tp);
  return ROOT::Math::SimilarityT(jl2c.jacobian(),Matrix6(ROOT::Math::SMatrixIdentity()));
  // return  ROOT::Math::Transpose(jl2c.jacobian())* jl2c.jacobian();

}


int main(int args, char ** argv) {

  // Distance const & d = distance();

  Matrix cov1 = buildCovariance(2.);
  Matrix cov2 = buildCovariance(1.);

  double one = GsfMatrixTools::trace(cov1,cov2);

  double two =  GsfMatrixTools::trace<5>(cov1*cov2); 
 
  if (fabs(one-two)>1.e-12) std::cout << "vincenzo was wrong!" << std::endl;
  std::cout << one << " " << two << " "<< one-two << std::endl;  

  if (args==1) {
    st();	
    one = GsfMatrixTools::trace(cov2,cov1);
    en();
  } else {
    st();
    two =  GsfMatrixTools::trace<5>(cov2*cov1); 
    en();
  }

  if (fabs(one-two)>1.e-15) std::cout << "vincenzo was wrong!" << std::endl;


  return 0;

}
