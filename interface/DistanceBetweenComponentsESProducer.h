#ifndef DistanceBetweenComponentsESProducer_h_
#define DistanceBetweenComponentsESProducer_h_

#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/GsfTools/interface/DistanceBetweenComponents.h"
#include <boost/shared_ptr.hpp>

/** Provides algorithms to measure the distance between  components
 * (currently either using a Kullback-Leibler or a Mahalanobis distance)
 */

class  DistanceBetweenComponentsESProducer: public edm::ESProducer{
 public:
  DistanceBetweenComponentsESProducer(const edm::ParameterSet & p);
  virtual ~DistanceBetweenComponentsESProducer(); 
  boost::shared_ptr<DistanceBetweenComponents> produce(const TrackingComponentsRecord &);
 private:
  edm::ParameterSet pset_;
};


#endif



