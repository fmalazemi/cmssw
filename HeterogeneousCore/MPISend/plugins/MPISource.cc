// -*- C++ -*-
//
// Package:    HeterogeneousCore/MPISource
// Class:      MPISource
//
/**\class MPISource MPISource.cc HeterogeneousCore/MPISource/plugins/MPISource.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Fawaz M Sh Kh W Alazemi
//         Created:  Fri, 24 Mar 2023 20:57:12 GMT
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"



#include "messages.h"
#include<iostream> 
#include "HeterogeneousCore/MPIServices/interface/MPIService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include<string>
#include "HeterogeneousCore/MPISend/interface/MPICommunicator.h"

//
// class declaration
//







class MPISource : public edm::stream::EDProducer<edm::GlobalCache<MPICommunicator>> {
public:
  explicit MPISource(const edm::ParameterSet&,  MPICommunicator const *);
  ~MPISource() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
 static std::unique_ptr<MPICommunicator> initializeGlobalCache(edm::ParameterSet const& iConfig) {
        edm::Service<MPIService> service;
        service->required();
	std::unique_ptr<MPICommunicator> com = std::make_unique<MPICommunicator>(iConfig.getUntrackedParameter<std::string>("service"));
	com->publish_and_listen();
	std::cout<<"Source is Up and ready to connect\n";
	return com; 
	

 }
 static void globalEndJob(MPICommunicator const* iGlobalCommunicator) {
      //std::cout <<"Number of events seen "<<iGlobalCommunicator->value<<std::endl;
   }
private:
  void beginStream(edm::StreamID) override;
  void produce(edm::Event&, const edm::EventSetup&) override;
  void endStream() override;

  //void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //void endRun(edm::Run const&, edm::EventSetup const&) override;
  //void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  // ----------member data ---------------------------
  //std::optional<MPICommunicator> communicator_;
  edm::EDPutTokenT<MPIToken> token_;







};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MPISource::MPISource(const edm::ParameterSet& iConfig, MPICommunicator const * c): token_{produces()} {
  //register your products
  /* Examples
  produces<ExampleData2>();

  //if do put with a label
  produces<ExampleData2>("label");
 
  //if you want to put into the Run
  produces<ExampleData2,InRun>();
  */
  //now do what ever other initialization is needed
/*	edm::Service<MPIService> service; 
	service->required(); 
	communicator_.emplace(iConfig.getUntrackedParameter<std::string>("service")); 
	communicator_->publish_and_listen() ;
	std::cout<<"Source is Up and ready to connect\n"; 
	*/
}

MPISource::~MPISource() {
  // do anything here that needs to be done at destruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
	std::cout<<"Source is Down\n"; 
}

//
// member functions
//

// ------------ method called to produce the data  ------------
void MPISource::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  /* This is an event example
  //Read 'ExampleData' from the Event
  ExampleData const& in = iEvent.get(inToken_);

  //Use the ExampleData to create an ExampleData2 which 
  // is put into the Event
  iEvent.put(std::make_unique<ExampleData2>(in));
  */

  /* this is an EventSetup example
  //Read SetupData from the SetupRecord in the EventSetup
  SetupData& setup = iSetup.getData(setupToken_);
  */
  //MPICommunicator const * com_ptr = (globalCache()get()); //&communicator_.value(); 
  iEvent.emplace(token_, globalCache());

  
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void MPISource::beginStream(edm::StreamID) {
  // please remove this method if not needed
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void MPISource::endStream() {
  // please remove this method if not needed
}

// ------------ method called when starting to processes a run  ------------
/*
void
MPISource::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
MPISource::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
MPISource::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
MPISource::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MPISource::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MPISource);
