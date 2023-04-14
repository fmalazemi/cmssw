import FWCore.ParameterSet.Config as cms
#from HeterogeneousCore.CUDACore.SwitchProducerCUDA import SwitchProducerCUDA


#we will need to write a simple module for testing


#config name
process = cms.Process('Writer')

process.load('HeterogeneousCore.MPIServices.MPIService_cfi')

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.MPIService = dict()


process.source = cms.Source('EmptySource')


#package or modules to load (we dont need for MPI 
#process.load('Configuration.StandardSequences.Accelerators_cff')

# enable logging for the TestPortableAnalyzer, we need it later ok with cout/cin now
#process.MessageLogger.TestPortableAnalyzer = cms.      untracked.PSet()

process.controller = cms.EDProducer("MPIController", service=cms.untracked.string("mpi_server"))

# run the producer on the cpu
process.sender= cms.EDProducer('MPISend', controller=cms.InputTag("controller"))



process.producer_task = cms.Task(process.controller)

process.process_path = cms.Path(
    process.sender, process.producer_task)


#process.serial_path = cms.Path(   process.testProducerSerial +  process.testAnalyzerSerial)

#process.output_path = cms.EndPath(process.output)

process.maxEvents.input = 1

