#include "HeterogeneousCore/MPISend/interface/MPICommunicator.h"
#include<iostream>
#include<mpi.h> 
#include<cassert>

MPICommunicator::~MPICommunicator(){
     /*  
       if(isService_){
              MPI_Comm_free(&communicator_);
              MPI_Unpublish_name(serviceName_.c_str(), MPI_INFO_NULL, portName_);
              MPI_Close_port(portName_);
              std::cout<<"Service unpublish and closed port\n";
       }else{
              MPI_Comm_disconnect( &communicator_ );
              std::cout<<"Master disconnected\n";
       }*/
       std::cout<<"MPI Distructor\n"; 
}

void MPICommunicator::connect(){

        assert( ! isService_);
        if(isDone) return ; 
        MPI_Lookup_name( serviceName_.c_str(), MPI_INFO_NULL, portName_);
        MPI_Comm_connect( portName_, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &communicator_);
        std::cout<<"Client sucessfully connected to a service\n";
	//channels_.insert(std::make_pair(serviceName_,std::make_pair(communicator_, portName_))); 
	isDone = true ; 
}

void MPICommunicator::publish_and_listen(){
       isService_ = true;
       if(isDone) return;
       //publish
       MPI_Open_port(MPI_INFO_NULL, portName_);
       MPI_Publish_name(serviceName_.c_str(), MPI_INFO_NULL, portName_);
       std::cout<<"Serivce successfully published its name "<<serviceName_<<"\n";
       //listen
       MPI_Comm_accept(portName_, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &communicator_);
       std::cout<<"Service accepted a connection\n";
       isDone = true; 
}

MPI_Comm MPICommunicator::getCommunicator()  const {
//      assert(channels_.size() > 0); //If channels_ size zero then we did not call connect or publish  
        //return first comm
        return communicator_; 
        //return (channels_.begin()->second).first;
}

MPI_Comm MPICommunicator::getCommunicator(std::string serviceName) const {
        assert(channels_.find(serviceName) != channels_.end());
        return channels_.at(serviceName).first;
}


