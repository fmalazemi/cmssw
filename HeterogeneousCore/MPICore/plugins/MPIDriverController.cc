// -*- C++ -*-
//
// Package:    HeterogeneousCore/MPICore
// Class:      MPIDriverController
//
/**\class MPIDriverController MPIDriverController.cc HeterogeneousCore/MPICore/plugins/MPIDriverController.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Fawaz M Sh Kh W Alazemi
//         Created:  Fri, 31 May 2024 13:12:40 GMT
//
//

// system include files
#include <memory>
#include <mpi.h>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "HeterogeneousCore/MPICore/interface/MPICommunicator.h"

//
// class declaration
//

class MPIDriverController : public edm::stream::EDProducer<> {
public:
  explicit MPIDriverController(const edm::ParameterSet&);
  ~MPIDriverController() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginStream(edm::StreamID) override;
  void produce(edm::Event&, const edm::EventSetup&) override;
  void endStream() override;

  //void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //void endRun(edm::Run const&, edm::EventSetup const&) override;
  //void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  // ----------member data ---------------------------
  edm::EDGetTokenT<MPIToken> communicatorToken_;
  edm::EDPutTokenT<int> tagToken_;
  edm::StreamID sid_ = edm::StreamID::invalidStreamID();

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
MPIDriverController::MPIDriverController(const edm::ParameterSet& iConfig):communicatorToken_{consumes(iConfig.getParameter<edm::InputTag>("communicator"))}, tagToken_{produces()} {
  //register your products
  /* Examples
  produces<ExampleData2>();

  //if do put with a label
  produces<ExampleData2>("label");
 
  //if you want to put into the Run
  produces<ExampleData2,InRun>();
  */
  //now do what ever other initialization is needed
}

MPIDriverController::~MPIDriverController() {
  // do anything here that needs to be done at destruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//

// ------------ method called to produce the data  ------------
void MPIDriverController::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
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
  MPIToken tokenData = iEvent.get(communicatorToken_);
  const MPICommunicator* MPICommPTR = tokenData.token_;  //replace with a better name
  MPI_Comm dataComm_ = MPICommPTR->Communicator();
  
  int tagID; 
  std::cout<<"Stream id = "<<sid_.value()<<" is waiting for Tag"<<std::endl;
  MPI_Recv(&tagID, 1, MPI_UNSIGNED, 0, 0, dataComm_, MPI_STATUS_IGNORE);
  std::cout<<"Stream id = "<<sid_.value()<<" will use tag = "<<tagID<<std::endl;
  iEvent.emplace(tagToken_, tagID);  

}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void MPIDriverController::beginStream(edm::StreamID stream) {
  sid_ = stream; 
  // please remove this method if not needed
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void MPIDriverController::endStream() {
  // please remove this method if not needed
}

// ------------ method called when starting to processes a run  ------------
/*
void
MPIDriverController::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
MPIDriverController::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
MPIDriverController::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
MPIDriverController::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MPIDriverController::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MPIDriverController);
