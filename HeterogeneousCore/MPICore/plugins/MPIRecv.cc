// -*- C++ -*-
//
// Package:    HeterogeneousCore/MPIRecv
// Class:      MPIRecv
//
/**\class MPIRecv MPIRecv.cc HeterogeneousCore/MPIRecv/plugins/MPIRecv.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Fawaz M Sh Kh W Alazemi
//         Created:  Thu, 17 Nov 2022 09:50:39 GMT
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

//
// class declaration
//
//
//

/******************
 * MPI Begin
 * Note: For testing purposes we assume Sender will try to find the received (which is lanuched manually at the moment)
 ******************/
#include<iostream>
#include<mpi.h>
#include<cstdlib> 
#include "HeterogeneousCore/MPIServices/interface/MPIService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include<string> 
#include<vector>
#include "HeterogeneousCore/MPICore/interface/MPICommunicator.h"
/******************
 * MPI end
 ******************/


class MPIRecv : public edm::stream::EDProducer<> {
public:
  explicit MPIRecv(const edm::ParameterSet&);
  ~MPIRecv() override;

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
 //MPICommunicator* x; 
 edm::EDGetTokenT<MPIToken> communicatorToken_; 

};

//
// constants, enums and typedefs
//
/******************
 * MPI Begin
 ******************/



/*********** MPI END ***********/

//
// static data member definitions
//

//
// constructors and destructor
//
MPIRecv::MPIRecv(const edm::ParameterSet& iConfig):communicatorToken_{consumes(iConfig.getParameter<edm::InputTag>("controller"))}{

  //register your products
  /* Examples
  produces<ExampleData2>();

  //if do put with a label
  produces<ExampleData2>("label");
 
  //if you want to put into the Run
  produces<ExampleData2,InRun>();
  */
  //now do what ever other initialization is needed


  //************* MPI Begin *****************
	std::cout<<"MPI Recv is up\n";
  //************* MPI END ******************
	
}

MPIRecv::~MPIRecv() {
  // do anything here that needs to be done at destruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty


  //**************** MPI Begin ****************
  //MPI_Finalize(); 
 
  //**************** MPI END *****************
}

//
// member functions
//

// ------------ method called to produce the data  ------------
void MPIRecv::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
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
 

  //****************** MPI Begin *****************
  const MPICommunicator* communicator = iEvent.get(communicatorToken_).token;   
  MPI_Comm dataComm_ = communicator->getDataCommunicator();
  std::cout<<"******************  RECV waiting for send to connect\n";
 
  MPI_Status status;
  int dummyInt_; 
  MPI_Recv(&dummyInt_, 1, MPI_INT, MPI_ANY_SOURCE, 432, dataComm_, MPI_STATUS_IGNORE);
  std::cout<<"___Received Data = "<<dummyInt_<<std::endl;

  std::cout<<"*********************  RECV: Receiver got the message:\n >>>>>>>>>>>>>>>>>>>>>>>>> \n";
  //****************** MPI END ******************


}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void MPIRecv::beginStream(edm::StreamID stream) {
  // please remove this method if not needed
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void MPIRecv::endStream() {
  // please remove this method if not needed
}

// ------------ method called when starting to processes a run  ------------
/*
void
MPIRecv::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
MPIRecv::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
MPIRecv::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
MPIRecv::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MPIRecv::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MPIRecv);
