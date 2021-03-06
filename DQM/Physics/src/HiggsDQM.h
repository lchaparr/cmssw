#ifndef HiggsDQM_H
#define HiggsDQM_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

// Trigger stuff
#include "DataFormats/Common/interface/TriggerResults.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/Common/interface/Handle.h" 
#include "FWCore/Framework/interface/DataKeyTags.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include <DataFormats/EgammaCandidates/interface/GsfElectron.h>

#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

class DQMStore;
 
class HiggsDQM: public edm::EDAnalyzer{

public:

  HiggsDQM(const edm::ParameterSet& ps);
  virtual ~HiggsDQM();
  
protected:

  void beginJob();
  void beginRun(edm::Run const& run, 
                edm::EventSetup const& eSetup);
  void analyze(edm::Event const& e, 
               edm::EventSetup const& eSetup);
  void beginLuminosityBlock(edm::LuminosityBlock const& lumiSeg, 
                            edm::EventSetup const& context) ;
  void endLuminosityBlock(edm::LuminosityBlock const& lumiSeg, 
                          edm::EventSetup const& c);
  void endRun(edm::Run const& run, 
              edm::EventSetup const& eSetup);
  void endJob();

private:

  double Distance( const reco::Candidate & c1, const reco::Candidate & c2 );
  double DistancePhi( const reco::Candidate & c1, const reco::Candidate & c2 );
  double calcDeltaPhi(double phi1, double phi2);
  void bookHistos(DQMStore * bei );
  
  unsigned long long m_cacheID_;
  int nLumiSecs_;
  int nEvents_, irun, ievt;
  reco::CandidateCollection *leptonscands_;
  int leptonflavor;
  float pi;
  
  DQMStore* bei_;  
  HLTConfigProvider hltConfigProvider_;
  bool isValidHltConfig_;

  // Variables from config file
  std::string   theElecTriggerPathToPass;
  std::string   theMuonTriggerPathToPass;
  edm::InputTag theTriggerResultsCollection;
  edm::InputTag theMuonCollectionLabel;
  edm::InputTag theElectronCollectionLabel;
  edm::InputTag theCaloJetCollectionLabel;
  edm::InputTag theCaloMETCollectionLabel;
  edm::InputTag thePfMETCollectionLabel;
  double ptThrMu1_; // pt cut on the first muon for the Z^0
  double ptThrMu2_; // pt cut on the second muon for the Z^0 
  
  // Histograms
  MonitorElement* h_vertex_number;
  MonitorElement* h_vertex_chi2;
  MonitorElement* h_vertex_d0;
  MonitorElement* h_vertex_numTrks;
  MonitorElement* h_vertex_sumTrks;
  MonitorElement* h_jet_et;
  MonitorElement* h_jet2_et;
  MonitorElement* h_jet_count;
  MonitorElement* h_caloMet;
  MonitorElement* h_caloMet_phi;
  MonitorElement* h_pfMet;
  MonitorElement* h_pfMet_phi;
  int nfourlept,nElectron,nMuon,nLepton,nZEE,nZMuMu,nHiggs,nLooseIsolEle,nLooseIsolMu;
  MonitorElement* h_eMultiplicity;
  MonitorElement* h_mMultiplicity;
  MonitorElement* h_ePt;
  MonitorElement* h_eEta;
  MonitorElement* h_ePhi;
  MonitorElement* h_mPt_GMTM;
  MonitorElement* h_mEta_GMTM;
  MonitorElement* h_mPhi_GMTM;
  MonitorElement* h_mPt_GMPT;
  MonitorElement* h_mEta_GMPT;
  MonitorElement* h_mPhi_GMPT;
  MonitorElement* h_mPt_GM;
  MonitorElement* h_mEta_GM;
  MonitorElement* h_mPhi_GM;
  MonitorElement* h_mPt_TM;
  MonitorElement* h_mEta_TM;
  MonitorElement* h_mPhi_TM;
  MonitorElement* h_mPt_STAM;
  MonitorElement* h_mEta_STAM;
  MonitorElement* h_mPhi_STAM;
  MonitorElement* h_eCombIso;
  MonitorElement* h_mCombIso;
  MonitorElement* h_dimumass_GMGM;
  MonitorElement* h_dimumass_GMTM;
  MonitorElement* h_dimumass_TMTM;
  MonitorElement* h_dielemass;
  MonitorElement* h_lepcounts;
  
  
};


#endif
