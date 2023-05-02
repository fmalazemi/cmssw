#include "HeterogeneousCore/MPICore/interface/MPICommunicator.h"
#include<iostream>
#include<mpi.h> 
#include<cassert>

	



MPICommunicator::MPICommunicator(std::string serviceName):serviceName_{std::move(serviceName)}{
}




MPICommunicator::~MPICommunicator(){
       MPI_Comm_disconnect(&mainComm_);
       MPI_Info port_info;
       MPI_Info_create(&port_info);
       MPI_Info_set(port_info, "ompi_global_scope", "true");
       MPI_Info_set(port_info, "ompi_unique", "true");
       MPI_Unpublish_name(serviceName_.c_str(), port_info, port_);
       MPI_Close_port(port_);
       std::cout<<"MPI Distructor\n"; 
}

void MPICommunicator::connect(){

	std::cout<<"Lookup name is "<<serviceName_<<std::endl;
        MPI_Lookup_name( serviceName_.c_str(), MPI_INFO_NULL, port_);
        MPI_Comm_connect( port_, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &mainComm_);
        std::cout<<"Client sucessfully connected to a service\n";
}

void MPICommunicator::publish_and_listen(){
       //publish
       MPI_Open_port(MPI_INFO_NULL, port_);
       MPI_Info port_info;
       MPI_Info_create(&port_info);
       MPI_Info_set(port_info, "ompi_global_scope", "true");
       MPI_Info_set(port_info, "ompi_unique", "true");
       MPI_Publish_name(serviceName_.c_str(), port_info, port_);
       std::cout<<"Serivce successfully published its name "<<serviceName_<<"\n";
       //listen
       MPI_Comm_accept(port_, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &mainComm_);
       std::cout<<"Service accepted a connection\n";
}

void MPICommunicator::splitCommunicator(){
	//Key ties are broken according to original rank
	//color 0 for control Communicator	
	MPI_Comm_split(mainComm_, 0, 0, &controlComm_);
	//color 1 for data communicator
	MPI_Comm_split(mainComm_, 1, 0, &dataComm_); 

}


MPI_Comm MPICommunicator::mainCommunicator()  const {
        return mainComm_; 
}

MPI_Comm MPICommunicator::dataCommunicator() const{ 
	return dataComm_; 
}

MPI_Comm MPICommunicator::controlCommunicator() const {
	return controlComm_; 
}


