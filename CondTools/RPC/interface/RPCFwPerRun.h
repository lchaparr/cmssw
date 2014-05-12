#ifndef RPC_DB_FW_PR_H
#define RPC_DB_FW_PR_H

/*
 * \class RPCFw
 *  Reads data from OMDS and creates conditioning objects per run
 *
 *  \
 * modified after RPCFw from D. Pagano - Dip. Fis. Nucl. e Teo. & INFN Pavia
 */


#include "CondTools/RPC/interface/RPCDBCom.h"
#include "CoralBase/TimeStamp.h"
#include "CondTools/RPC/interface/RPCImonPerRunSH.h"
#include "CondTools/RPC/interface/RPCVmonSH.h"
#include "CondTools/RPC/interface/RPCStatusSH.h"
#include "CondTools/RPC/interface/RPCTempSH.h"
#include "CondFormats/RPCObjects/interface/RPCObCond.h"
#include "CondTools/RPC/interface/RPCGasSH.h"
#include "CondTools/RPC/interface/RPCIDMapSH.h"
#include "CondFormats/RPCObjects/interface/RPCObFebmap.h"
#include "CondFormats/RPCObjects/interface/RPCObUXC.h"
#include "CondFormats/RPCObjects/interface/RPCObGasMix.h"

//struct dbread{
//    float alias;
//   float value;
//};


class RPCFwPerRun : virtual public RPCDBCom
{
public:
  RPCFwPerRun( const std::string& connectionString,
	       const std::string& userName,
	       const std::string& password,
	       const std::string& connectionStringPvssInfo,
	       const std::string& userNamePvssInfo,
	       const std::string& passwordPvssInfo,
	       const std::string& connectionStringRunInfo,
	       const std::string& userNameRunInfo,
	       const std::string& passwordRunInfo
	       )
	       ;
  virtual ~RPCFwPerRun();
  void run();
  
  coral::TimeStamp UTtoT(long long utime);
  unsigned long long TtoUT(coral::TimeStamp time);

  coral::TimeStamp tMIN;
  coral::TimeStamp tMAX;
  unsigned long long N_IOV;

  std::vector<RPCObImonPerRun::I_Item> createIMON(long long firstrun, long long lstrun);
  std::vector<RPCObImonPerLumi::I_Item> createIMONPerLumi(long long firstrun, long long lstrun);

  std::vector<RPCObVmon::V_Item> createVMON(long long from, long long till); 
  std::vector<RPCObStatus::S_Item> createSTATUS(long long since, long long till); 
  std::vector<RPCObGas::Item> createGAS(long long since, long long till);
  std::vector<RPCObTemp::T_Item> createT(long long since, long long till);
  std::vector<RPCObPVSSmap::Item> createIDMAP();
  std::vector<RPCObFebmap::Feb_Item> createFEB(long long since, long long till);	
  std::vector<RPCObUXC::Item> createUXC(long long since, long long till);
  std::vector<RPCObGasMix::Item> createMix(long long since, long long till);
  bool isMajor(coral::TimeStamp fir, coral::TimeStamp sec);
  
private:
  std::string m_connectionString;
  std::string m_userName;
  std::string m_password;
  std::string m_connectionStringPvssInfo;
  std::string m_userNamePvssInfo;
  std::string m_passwordPvssInfo;
  std::string m_connectionStringRunInfo;
  std::string m_userNameRunInfo;
  std::string m_passwordRunInfo;
};

#endif
