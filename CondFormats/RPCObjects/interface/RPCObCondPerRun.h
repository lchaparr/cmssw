/*
 * Payload definition(s): Current (RPCObImon), High Voltage (RPCObVmon), Chamber Status (RPCObStatus) 
 *
 *  $Date: 2009/11/10 12:20:23 $
 *  $Revision: 1.17 $
 *  \author D. Pagano - Dip. Fis. Nucl. e Teo. & INFN Pavia
 */

#ifndef RPCObCondPerRun_h
#define RPCObCondPerRun_h
#include <vector>

class RPCObImonPerRun {
    public:
  //int run;
  struct I_Item {
    float dpid;
    float run;
    float value;
    float value_rpc_on;
    float value_rpc_phys_on;
    float status;
  };
  RPCObImonPerRun(){}
  virtual ~RPCObImonPerRun(){}
  std::vector<I_Item> ObImonPerRun_rpc;
};

class RPCObImonPerLumi {
    public:
  //  int run;
      struct I_Item {
        float dpid;
        float value;
	float run;  
	float lumi;  
        float status;
      };
    RPCObImonPerLumi(){}
    virtual ~RPCObImonPerLumi(){}
    std::vector<I_Item> ObImonPerLumi_rpc;
   };

class RPCObVmonPerRun {
    public:
      struct V_Item {
        int dpid;
        float value;
      };
    RPCObVmonPerRun(){}
    virtual ~RPCObVmonPerRun(){}
    std::vector<V_Item> ObVmonPerRun_rpc;
   };

class RPCObTempPerRun {
    public:
      struct T_Item {
        int dpid;
        float value;
      };
    RPCObTempPerRun(){}
    virtual ~RPCObTempPerRun(){}
    std::vector<T_Item> ObTempPerRun_rpc;
   };

#endif

