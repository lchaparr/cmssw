#ifndef POPCON_RPC_DATA_PERRUN_SRC_H
#define POPCON_RPC_DATA_PERRUN_SRC_H

/*
 * \class RPCImonSH
 *  Core of RPC PopCon Appication
 *
 *  \author D. Pagano - Dip. Fis. Nucl. e Teo. & INFN Pavia
 */

#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>

#include "CondCore/PopCon/interface/PopConSourceHandler.h"

#include "CondFormats/RPCObjects/interface/RPCObCondPerRun.h"
#include "CondFormats/DataRecord/interface/RPCObCondPerRunRcd.h"
#include "CoralBase/TimeStamp.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include "CondTools/RPC/interface/RPCFwPerRun.h"
#include <string>
#include <fstream>


namespace popcon{
  class RpcDataIR : public popcon::PopConSourceHandler<RPCObImonPerRun>{
  public:
    void getNewObjects();
    std::string id() const { return m_name;}
    ~RpcDataIR(); 
    RpcDataIR(const edm::ParameterSet& pset); 

    RPCObImonPerRun* Idata;
    unsigned long long firstrun;
    unsigned long long lastrun;
    unsigned long long niov;	    
    unsigned long long utime;
    unsigned long long snc;
    unsigned long long tll;
    std::ofstream * output;
  private:
    std::string m_name;
    std::string host;
    std::string user;
    std::string passw;
    std::string hostPvssInfo;
    std::string userPvssInfo;
    std::string passwPvssInfo;
    std::string hostRunInfo;
    std::string userRunInfo;
    std::string passwRunInfo;
    unsigned long long m_since;
    unsigned long long m_till;

  };
}
#endif

