#include "CondCore/PopCon/interface/PopConAnalyzer.h"
#include "CondTools/RPC/interface/RPCImonPerLumiSH.h"
#include "FWCore/Framework/interface/MakerMacros.h"

typedef popcon::PopConAnalyzer<popcon::RpcDataIL> RPCImonPerLumiAnalyzer;
//define this as a plug-in
DEFINE_FWK_MODULE(RPCImonPerLumiAnalyzer);
