#include "HeterogeneousCore/MPICore/interface/MPICommunicator.h"
#include<iostream>
#include<mpi.h> 
#include<cassert>

	



MPICommunicator::MPICommunicator(std::string serviceName):serviceName_{std::move(serviceName)}{
}




MPICommunicator::~MPICommunicator(){
       MPI_Comm_disconnect(&communicator_);
       MPI_Info port_info;
       MPI_Info_create(&port_info);
       MPI_Info_set(port_info, "ompi_global_scope", "true");
       MPI_Info_set(port_info, "ompi_unique", "true");
       MPI_Unpublish_name(serviceName_.c_str(), port_info, port_);
       MPI_Close_port(port_);
       std::cout<<"MPI Distructor\n"; 
}

void MPICommunicator::connect(){

        assert( ! isService_);
        if(isDone) return ; 
	std::cout<<"Lookup name is "<<serviceName_<<std::endl;
        MPI_Lookup_name( serviceName_.c_str(), MPI_INFO_NULL, port_);
        MPI_Comm_connect( port_, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &communicator_);
        std::cout<<"Client sucessfully connected to a service\n";
	isDone = true ; 
}

void MPICommunicator::publish_and_listen(){
       isService_ = true;
       if(isDone) return;
       //publish
       MPI_Open_port(MPI_INFO_NULL, port_);
       MPI_Info port_info;
       MPI_Info_create(&port_info);
       MPI_Info_set(port_info, "ompi_global_scope", "true");
       MPI_Info_set(port_info, "ompi_unique", "true");
       MPI_Publish_name(serviceName_.c_str(), port_info, port_);
       std::cout<<"Serivce successfully published its name "<<serviceName_<<"\n";
       //listen
       MPI_Comm_accept(port_, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &communicator_);
       std::cout<<"Service accepted a connection\n";
       isDone = true; 
}

MPI_Comm MPICommunicator::getCommunicator()  const {
        return communicator_; 
}



