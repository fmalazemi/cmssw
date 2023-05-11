// -*- C++ -*-
//
// Package:    HeterogeneousCore/MPIController
// Class:      MPIController
//
/**\class MPIController MPIController.cc HeterogeneousCore/MPIController/plugins/MPIController.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Fawaz M Sh Kh W Alazemi
//         Created:  Fri, 24 Mar 2023 21:03:08 GMT
//
//

#include <memory>
#include<iostream> 
#include <string> 

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "HeterogeneousCore/MPIServices/interface/MPIService.h"
#include "HeterogeneousCore/MPICore/interface/MPICommunicator.h"
#include <chrono>
#include <thread>
#include "api.h"
#include "messages.h"
#include<atomic>



class MPIController : public edm::stream::EDProducer<edm::GlobalCache<MPICommunicator>> {
public:
  explicit MPIController(const edm::ParameterSet&, MPICommunicator const *);
  ~MPIController() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  static std::unique_ptr<MPICommunicator> initializeGlobalCache(edm::ParameterSet const&);
  static void globalEndJob(MPICommunicator const* iMPICommunicator); 
private:
  void beginStream(edm::StreamID) override;
  void produce(edm::Event&, const edm::EventSetup&) override;
  void endStream() override;

  void beginRun(edm::Run const&, edm::EventSetup const&) override;
  void endRun(edm::Run const&, edm::EventSetup const&) override;
  void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  // ----------member data ---------------------------
  edm::StreamID sid_ = edm::StreamID::invalidStreamID(); 
  //std::optional<MPICommunicator> comm_; 
  MPISender link; 
  edm::EDPutTokenT<MPIToken> token_; 
static std::atomic<int> sharedData;

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
MPIController::MPIController(const edm::ParameterSet& iConfig, MPICommunicator const* MPICommPTR):token_{produces()} {
  //register your products
  /* Examples
  produces<ExampleData2>();

  //if do put with a label
  produces<ExampleData2>("label");
 
  //if you want to put into the Run
  produces<ExampleData2,InRun>();
  */
  //now do what ever other initialization is needed
  link = MPISender(MPICommPTR->controlCommunicator(), 0); 
}

MPIController::~MPIController() {
  // do anything here that needs to be done at destruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
	std::cout<<"Controller dies\n"; 
}

//
// member functions
//

std::unique_ptr<MPICommunicator>  MPIController::initializeGlobalCache(edm::ParameterSet const& iConfig) {

	EDM_MPI_build_types();
        edm::Service<MPIService> service;

        service->required();
        std::unique_ptr<MPICommunicator> MPICommPTR = std::make_unique<MPICommunicator>(iConfig.getUntrackedParameter<std::string>("service"));

        MPICommPTR->connect();
        MPICommPTR->splitCommunicator(); 	
//	EDM_MPI_Empty_t buffer; 
//	std::cout<<"We are here\n";
//	MPI_Send(&buffer, 1, EDM_MPI_Empty, 0, EDM_MPI_Connect, MPICommPTR->controlCommunicator()); 

        std::cout<<"Controller is UP and Connected\n";
 
        return MPICommPTR;
}

void MPIController::globalEndJob(MPICommunicator const* MPICommPTR) {
      //std::cout <<"Number of events seen "<<iMPICommunicator->value<<std::endl;
   }




// ------------ method called to produce the data  ------------
void MPIController::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
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
  printf("MPIController::produce, Sid = %d\n", sid_.value()); 
  printf("iEvent = %lld\n", iEvent.id().event()); 
  MPICommunicator const * MPICommPTR = globalCache();  
  link.sendEvent(sid_, iEvent.eventAuxiliary());
  iEvent.emplace(token_, MPICommPTR); 
  int x = 0; 
  std::cout<<sid_.value()<<" : Enter a number :"<<std::endl; 
  std::cin>>x; 
  std::cout<<sid_.value()<<" : You Entered : "<<x<<std::endl;  

}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void MPIController::beginStream(edm::StreamID sid) {
  // please remove this method if not needed
  sid_ = sid;
 // link.sendConnect(sid_);
 printf("MPIController::beginStream, Sid = %d\n", sid_.value());
  link.sendBeginStream(sid_);
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void MPIController::endStream() {
  printf("MPIController::endStream, Sid = %d\n", sid_.value());
	// signal the end stream

  link.sendEndStream(sid_);
  // signal the disconnection
  link.sendDisconnect(sid_);
}

// ------------ method called when starting to processes a run  ------------

void
MPIController::beginRun(edm::Run const& run, edm::EventSetup const& setup)
{
	printf("MPIController::beginRun, Sid = %d\n", sid_.value());
	auto aux = run.runAuxiliary(); 
	aux.setProcessHistoryID(run.processHistory().id()); 
	link.sendBeginRun(sid_, aux); 
	//transmit the ProcessHistory
	link.sendSerializedProduct(sid_, run.processHistory()) ; 
}


// ------------ method called when ending the processing of a run  ------------

void
MPIController::endRun(edm::Run const& run, edm::EventSetup const& setup)
{
	printf("MPIController::endRun, Sid = %d\n", sid_.value());
	auto aux = run.runAuxiliary(); 
	aux.setProcessHistoryID(run.processHistory().id()); 
	link.sendEndRun(sid_, aux); 
}


// ------------ method called when starting to processes a luminosity block  ------------

void
MPIController::beginLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& setup)
{
	printf("MPIController::beginLuminosityBlock, Sid = %d\n", sid_.value());
	auto aux = lumi.luminosityBlockAuxiliary(); 
	aux.setProcessHistoryID(lumi.processHistory().id()); 
	link.sendBeginLuminosityBlock(sid_, aux); 
	
}


// ------------ method called when ending the processing of a luminosity block  ------------

void
MPIController::endLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& setup)
{
	printf("MPIController::endLuminosityBlock, Sid = %d\n", sid_.value());
	auto aux = lumi.luminosityBlockAuxiliary(); 
	aux.setProcessHistoryID(lumi.processHistory().id()); 
	link.sendEndLuminosityBlock(sid_, aux); 
}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MPIController::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MPIController);
