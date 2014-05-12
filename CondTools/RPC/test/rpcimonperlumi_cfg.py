import FWCore.ParameterSet.Config as cms

process = cms.Process("rpcNoiseStrips")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.CondDBCommon.connect = 'sqlite_file:ImonLumiTest.db'
process.CondDBCommon.DBParameters.authenticationPath = '/afs/cern.ch/cms/DB/conddb'



process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDBCommon,	
    timetype = cms.untracked.string('runnumber'),
    logconnect = cms.untracked.string('sqlite_file:myOrcof_StripsLOG.db'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('RPCObImonPerLumiRcd'),
        tag = cms.string('RPCImonPerRun_v1')))
)    


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## process.source = cms.Source("PoolSource",
##     replace 'myfile.root' with the source file you want to use
##     fileNames = cms.untracked.vstring(
##         'file:myfile.root'
##     )
## )

process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(200990),
    lastValue = cms.uint64(200990),
    interval = cms.uint64(1)
)

process.rpcImonPerLumi = cms.EDAnalyzer("RPCImonPerLumiAnalyzer",
   record = cms.string('RPCObImonPerLumiRcd'),
   loggingOn = cms.untracked.bool(True),
   SinceAppendMode = cms.bool(True),
   Source = cms.PSet(
    #    version = cms.untracked.uint32(1),
    firstrun = cms.untracked.uint64(200990),
    lastrun = cms.untracked.uint64(200992),
    host  = cms.untracked.string(''),
    user  = cms.untracked.string(''),
    passw = cms.untracked.string(''),
    hostRunInfo  = cms.untracked.string(''),
    userRunInfo  = cms.untracked.string(''),
    passwRunInfo = cms.untracked.string('')
    )
)


process.p = cms.Path(process.rpcImonPerLumi)
