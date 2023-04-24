#ifndef HeterogeousCore_MPICore_interface_MPICommunicator_h
#define HeterogeousCore_MPICore_interface_MPICommunicator_h 

#include<mpi.h>
#include<string>
#include<map> 
#include<cassert>


class MPICommunicator { 
public:
  MPICommunicator(std::string serviceName);
   
  MPICommunicator() ;  
  ~MPICommunicator();
    
  void publish_and_listen();
  void connect();
  void splitCommunicator(int); 
  
MPI_Comm getCommunicator() const ; 
MPI_Comm getControlCommunicator() const; 
MPI_Comm getDataCommunicator() const; 

private:

 MPI_Comm communicator_; 
 MPI_Comm controlCommunicator_; 
 MPI_Comm dataCommunicator_; 
 char port_[MPI_MAX_PORT_NAME];
 const std::string serviceName_ = "";
 bool isService_ = false ; //Service is your are a listener 
 bool isDone = false; 

};


class MPIToken {
	public: 
		MPIToken() = default; 
		MPIToken(MPICommunicator const * t):token(t){}; 
		MPIToken(MPICommunicator const * t, int stream_, int source_):token(t), stream(stream_),source(source_){}; 
		MPICommunicator const * token; 
		int stream; 
		int source; 


};


#endif
