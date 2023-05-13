// -*- C++ -*-
//
// Package:    HeterogeneousCore/MPISend
// Class:      MPISend
//
/**\class MPISend MPISend.cc HeterogeneousCore/MPICore/plugins/MPISend.cc

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
#include<vector>
#include <sstream>

  #include <typeinfo>

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
#include "HeterogeneousCore/MPICore/interface/MPICommunicator.h"
/******************
 * MPI end
 ******************/

class MPISend : public edm::stream::EDProducer<>{
public:
  explicit MPISend(const edm::ParameterSet&);
  ~MPISend() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

 //void acquire(edm::Event const&, edm::EventSetup const&, edm::WaitingTaskWithArenaHolder) override;
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
 edm::EDGetTokenT<MPIToken> outComm_; 
 edm::EDGetTokenT<std::vector<int>> inData_; 
 edm::EDPutTokenT<std::vector<int>> outData_; 
 //add tag var 
 edm::StreamID sid_ = edm::StreamID::invalidStreamID();
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
MPISend::MPISend(const edm::ParameterSet& iConfig):communicatorToken_{consumes(iConfig.getParameter<edm::InputTag>("communicator"))},inData_{consumes(iConfig.getParameter<edm::InputTag>("incomingData"))}{
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
	std::cout<<"MPI Sender 0-----------\n";
  //************* MPI END ******************
	
}

MPISend::~MPISend() {
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
/*
 void MPISend::acquire(edm::Event const& iEvent, edm::EventSetup const& iSetup, edm::WaitingTaskWithArenaHolder waitingTaskHolder) {
	 
printf("HI"); 


  
  std::async([this, holder = std::move(waitingTaskHolder)]{
    MPI_Send("123", 3, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    holder.doneWaiting(nullptr);
  });

}
*/



// ------------ method called to produce the data  ------------
void MPISend::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;
  using namespace std;
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
  MPIToken tokenInfo = iEvent.get(communicatorToken_); 
  const MPICommunicator* MPICommPTR =tokenInfo.token;   
  MPI_Comm dataComm_ = MPICommPTR->dataCommunicator();
  int tagID = tokenInfo.stream; 
  std::vector<int> data = iEvent.get(inData_); 
  printf("+++++ _____ (Sid %d, %d, %d) in Data = ",sid_.value(), tagID, (int)iEvent.id().event()); 
  for(int i : data){
	  printf("%d ", i); 
  }
  printf(" _____ ++++++\n");
  int x; 
  MPI_Status status;
  //MPI_Sendrecv(&data[0], data.size(), MPI_INT, 0, (int)(iEvent.id().event()), &x, 1, MPI_INT, 0, (int)(iEvent.id().event()), dataComm_, &status);
  MPI_Send(&data[0], (int)data.size(), MPI_INT, 0, tagID, dataComm_);  
  /*  
  int dummyInt_ = 13;  
  MPI_Send(&dummyInt_, 1, MPI_INT, 0, 432, dataComm_); 
  std::cout<<"MPISend::produce "<<sid_.value()<<std::endl; 
  */ 
  std::cout<<"--------------  SENDER sent data (Stream ID = "<<sid_.value()<<") \n"; 
  //****************** MPI END ******************



}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void MPISend::beginStream(edm::StreamID stream) {
  // please remove this method if not needed
  sid_ = stream;
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void MPISend::endStream() {
  // please remove this method if not needed
}

// ------------ method called when starting to processes a run  ------------
/*
void
MPISend::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
MPISend::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
MPISend::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
MPISend::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MPISend::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MPISend);
