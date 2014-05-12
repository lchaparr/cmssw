#include "CondCore/PopCon/interface/PopConAnalyzer.h"
#include "CondTools/RPC/interface/FindScanSH.h"
#include "FWCore/Framework/interface/MakerMacros.h"

typedef popcon::PopConAnalyzer<popcon::FindScanSH> FindScanAnalyzer;
//define this as a plug-in
DEFINE_FWK_MODULE(FindScanAnalyzer);
