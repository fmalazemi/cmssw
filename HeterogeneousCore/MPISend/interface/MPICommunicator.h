#ifndef HeterogeousCore_MPISend_interface_MPICommunicator_h
#define HeterogeousCore_MPISend_interface_MPICommunicator_h 

#include<mpi.h>
#include<string>
#include<map> 
#include<cassert>


class MPICommunicator { 
public:
  MPICommunicator(std::string serviceName):serviceName_{std::move(serviceName)}{} 
  MPICommunicator();  
  ~MPICommunicator();
    
  void publish_and_listen();
  void connect();
  
MPI_Comm getCommunicator() const ; 
MPI_Comm getCommunicator(std::string) const ;	

private:

 MPI_Comm communicator_; 
 char portName_[MPI_MAX_PORT_NAME];
 const std::string serviceName_ = "";
 const std::map<const std::string, std::pair<MPI_Comm, char * > > channels_; 
 bool isService_ = false ; //Service is your are a listener 
 bool isDone = false; 

};


class MPIToken {
	public: 
		MPIToken() = default; 
		MPIToken(MPICommunicator const * t):token(t){}; 
		MPICommunicator const * token; 


};


#endif
