/*
 *  See headers for a description
 *
 *  \author D. Pagano - Dip. Fis. Nucl. e Teo. & INFN Pavia
 */

#include "CondTools/RPC/interface/RPCImonPerLumiSH.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include<iostream>

popcon::RpcDataIL::RpcDataIL(const edm::ParameterSet& pset) :
  m_name(pset.getUntrackedParameter<std::string>("name","RpcData")),
  host(pset.getUntrackedParameter<std::string>("host", "source db host")),
  user(pset.getUntrackedParameter<std::string>("user", "source username")),
  passw(pset.getUntrackedParameter<std::string>("passw", "source password")),
  hostPvssInfo(pset.getUntrackedParameter<std::string>("hostPvssInfo", "source db host for pvss run info ")),
  userPvssInfo(pset.getUntrackedParameter<std::string>("userPvssInfo", "source username for pvss run info ")),
  passwPvssInfo(pset.getUntrackedParameter<std::string>("passwPvssInfo", "source password for pvss run info ")),
  hostRunInfo(pset.getUntrackedParameter<std::string>("hostRunInfo", "source db host for run info")),
  userRunInfo(pset.getUntrackedParameter<std::string>("userRunInfo", "source username for run info")),
  passwRunInfo(pset.getUntrackedParameter<std::string>("passwRunInfo", "source password for run info")),
  firstrun(pset.getUntrackedParameter<unsigned long long >("firstrun",200000)),
  lastrun(pset.getUntrackedParameter<unsigned long long>("lastrun",200010)){
}

popcon::RpcDataIL::~RpcDataIL()
{
}

void popcon::RpcDataIL::getNewObjects() {

  std::cout << "------- " << m_name << " - > getNewObjects\n" 
	    << "got offlineInfo "<< tagInfo().name 
	    << ", size " << tagInfo().size << ", last object valid since " 
	    << tagInfo().lastInterval.first << " token "   
            << tagInfo().lastPayloadToken << std::endl;

  std::cout << " ------ last entry info regarding the payload (if existing): " 
	    << logDBEntry().usertext << "last record with the correct tag has been written in the db: "
	    << logDBEntry().destinationDB << std::endl; 
  
  snc = tagInfo().lastInterval.first;


   std::cout << std::endl << "=============================================" << std::endl;
   std::cout << std::endl << "===================  IMON  ==================" << std::endl;
   std::cout << std::endl << "=============================================" << std::endl << std::endl;
   snc = m_since;
   std::cout << ">> Range mode [" << snc << ", " << m_till << "]" << std::endl;
   std::cout << std::endl << "=============================================" << std::endl << std::endl;
   
   
   RPCFwPerRun caen ( host, user, passw, hostPvssInfo, userPvssInfo, passwPvssInfo ,hostRunInfo, userRunInfo, passwRunInfo );
   
   std::cout << " test 1 "<<std::endl;

   std::vector<RPCObImonPerLumi::I_Item> Icheck;
   
   Icheck = caen.createIMONPerLumi(firstrun, lastrun);

   std::cout << " test 2 "<<std::endl;
   
   for( unsigned long int runn = firstrun; runn <= lastrun;++runn){
     std::stringstream ss;
     ss<< runn<< "_ImonPerLumi.txt";
     output = new std::ofstream(ss.str());
     *output << runn << std::endl;
     
     Idata = new RPCObImonPerLumi();
     RPCObImonPerLumi::I_Item Ifill;
     std::vector<RPCObImonPerLumi::I_Item>::iterator Iit;
     for(Iit = Icheck.begin(); Iit != Icheck.end(); Iit++)
       {
	 Ifill = *(Iit);
	 if(Ifill.run == (float)runn){
	 Idata->ObImonPerLumi_rpc.push_back(Ifill);
	 std::cout << " iamdata data run "<< Ifill.run << " lumi "<< Ifill.lumi << " did "<< Ifill.dpid <<" current "<< Ifill.value <<std::endl;
	 *output << Ifill.dpid<< " "<< Ifill.lumi <<" " <<Ifill.value << " " << 1 <<" " <<std::endl;
	 }
       }
     std::cout << ">> Final object size: " << Idata->ObImonPerLumi_rpc.size() << std::endl;
     if (Idata->ObImonPerLumi_rpc.size() > 0) {
       niov = snc;
     } else {
       niov = snc;
       std::cout << "NO DATA TO BE STORED" << std::endl;
     }
     m_to_transfer.push_back(std::make_pair((RPCObImonPerLumi*)Idata,runn));
   }  
}

