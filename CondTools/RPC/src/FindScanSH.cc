// -*- C++ -*-
//
// Package:    FindScanSH
// Class:      FindScanSH
// 
/**\class FindScanSH FindScanSH.cc CondTools/DoAQuery/src/FindScanSH.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Simranjit Singh Chhibra,40 5-B06,+41227674539,
//         Created:  Fri Oct  7 10:51:47 CEST 2011
// $Id: FindScanSH.cc,v 1.1 2011/11/24 16:37:38 mmaggi Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondTools/RPC/interface/FindScanSH.h"

#include "RelationalAccess/ISession.h"
#include "RelationalAccess/ITransaction.h"
#include "RelationalAccess/ISchema.h"
#include "RelationalAccess/ITable.h"
#include "RelationalAccess/ITableDataEditor.h"
#include "RelationalAccess/TableDescription.h"
#include "RelationalAccess/IQuery.h"
#include "RelationalAccess/ICursor.h"

#include "CoralBase/AttributeList.h"
#include "CoralBase/Attribute.h"
#include "CoralBase/AttributeSpecification.h"
#include "CoralBase/TimeStamp.h"

struct geom{
  std::string alias;
  int region;
  int ring;
  int station;
  int sector;
  int layer;
  int subsector;
};

popcon::FindScanSH::FindScanSH(const edm::ParameterSet& iConfig)
{
  m_host = iConfig.getUntrackedParameter<std::string>("host");
  m_user = iConfig.getUntrackedParameter<std::string>("user");
  m_passw = iConfig.getUntrackedParameter<std::string>("passw");
  std::cout << "-------------START-------------" <<std::endl;
}


popcon::FindScanSH::~FindScanSH()
{
  std::cout << "-------------END-------------" <<std::endl;
}

void
popcon::FindScanSH::getNewObjects()
{
  std::cout<< "taginfo.name====="<<tagInfo().name<<
    "\t taginfo.size====="<<tagInfo().size<<
    "\t lastRun====="<<tagInfo().lastInterval.first<< std::endl;
  
  coral::ISession* masterSession = this->connect( m_host,m_user,m_passw );
  
  masterSession->transaction().start( true );
  coral::ISchema& masterSchema = masterSession->nominalSchema();
  coral::IQuery* masterQuery = masterSchema.newQuery();
  coral::IQueryDefinition& subQuery = masterQuery->defineSubQuery("ALIAS");

  masterQuery->addToTableList("ALIAS","G");
  masterQuery->addToTableList("RPCPVSSDETID","E");
  masterQuery->addToTableList("DP_NAME2ID","B");
  masterQuery->addToTableList("ALIASES","A");

  masterQuery->addToOutputList("G.L");
  masterQuery->addToOutputList("G.T");
  masterQuery->addToOutputList("E.PVSS_ID");
  masterQuery->addToOutputList("E.REGION");
  masterQuery->addToOutputList("E.RING");
  masterQuery->addToOutputList("E.STATION");
  masterQuery->addToOutputList("E.SECTOR");
  masterQuery->addToOutputList("E.LAYER");
  masterQuery->addToOutputList("E.SUBSECTOR");
  masterQuery->addToOutputList("E.SUPPLYTYPE");
  //  masterQuery->addToOrderList("A.L");

  subQuery.addToTableList("RPCPVSSDETID","E");
  subQuery.addToTableList("DP_NAME2ID","B");
  subQuery.addToTableList("ALIASES","A");
  subQuery.addToOutputList("A.ALIAS","L");
  subQuery.addToOutputList("max(E.SINCE)","T");
  subQuery.groupBy("A.ALIAS");

  coral::AttributeList subCondition;
  subCondition.extend<coral::TimeStamp>("tsince");
  subCondition.extend<int>("region");
  subCondition.extend<int>("ring");
  subCondition.extend<std::string>("suptype");
  
  coral::TimeStamp tsince(2011,2,4,7,0,0,0);
  std::cout <<"TSince ==== "<<tsince.total_nanoseconds()/1000/1000/1000<<std::endl;

  subCondition["tsince"].data<coral::TimeStamp>() = tsince;
  subCondition["suptype"].data<std::string>() = "HV";
  subCondition["region"].data<int>() = 1;
  subCondition["ring"].data<int>() = 1;
  std::string subcondition1 = "E.PVSS_ID=B.ID and A.DPE_NAME=B.DPNAME";
  std::string subcondition2 = " and E.SUPPLYTYPE=:suptype";
  std::string subcondition3 = " and E.SINCE < :tsince";
  std::string subcondition4 = " and E.REGION=:region and E.RING=:ring";
  std::string subcondition  = subcondition1+
    subcondition2+
    subcondition3+
    subcondition4;
    
  subQuery.setCondition(subcondition, subCondition);

  coral::AttributeList masCondition;
  std::string mascond1="E.PVSS_ID is not NULL and G.T=E.SINCE";
  std::string mascond2=" and G.L=A.ALIAS";
  std::string mascond3 = " and E.PVSS_ID=B.ID and A.DPE_NAME=B.DPNAME";
  std::string condM = mascond1+mascond2+mascond3;
  masterQuery->setCondition(condM,masCondition);

  coral::ICursor& cur = masterQuery->execute();

  std::map<int,geom> hvmap;
  int nrow=0;
  while(cur.next()){
    nrow++;
    const coral::AttributeList& row = cur.currentRow();
    std::string alias(row["G.L"].data<std::string>());
    coral::TimeStamp ts = row["G.T"].data<coral::TimeStamp>();
    int dpid=row["E.PVSS_ID"].data<int>();
    std::string regi=row["E.REGION"].data<std::string>();
    std::string ring=row["E.RING"].data<std::string>();
    std::string stat=row["E.STATION"].data<std::string>();
    std::string sect=row["E.SECTOR"].data<std::string>();
    std::string laye=row["E.LAYER"].data<std::string>();
    std::string subs=row["E.SUBSECTOR"].data<std::string>();
    std::string typ(row["E.SUPPLYTYPE"].data<std::string>());
    int reg=atoi(regi.c_str());
    int rin=atoi(ring.c_str());
    int sta=atoi(stat.c_str());
    int sec=atoi(sect.c_str());
    int lay=atoi(laye.c_str());
    int sub=atoi(subs.c_str());
    geom g;
    g.alias=alias;
    g.region=reg;
    g.ring=rin;
    g.station=sta;
    g.sector=sec;
    g.layer=lay;
    g.subsector=sub;
    hvmap[dpid]=g;
    std::cout<<"-> raw number="<<nrow
	     <<" alias="<<alias
	     <<" date="<<ts.day()<<"/"<<ts.month()<<"/"<<ts.year()<<" "
	     <<ts.hour()<<":"<<ts.minute()<<"."<<ts.second()
	     <<" dpid="<<dpid
	     <<" region="<<reg
	     <<" ring="<<rin
	     <<" station="<<sta
	     <<" sector="<<sec
	     <<" layer="<<lay
	     <<" subsector="<<sub
	     <<std::endl;
  }
  delete masterQuery;
  masterSession->transaction().commit();

  // In the loop I will query for each detector the HV values

  std::stringstream pdrange;
  pdrange <<"(";
  std::map<int,geom>::iterator hvlast=hvmap.end();
  hvlast--;
  for(std::map<int,geom>::iterator hv=hvmap.begin();hv!=hvmap.end();hv++){
    pdrange<<" "<<hv->first;
    if (hv!=hvlast)
     pdrange<<",";
  }
  pdrange<<")";
  std::cout <<"Querying in "<<pdrange.str()<<std::endl;
  //  for(std::map<int,geom>::iterator hv=hvmap.begin();hv!=hvmap.end();hv++){
  //  int dpid=hv->first;
    
    masterSession->transaction().start( true );
    coral::IQuery* queryV = masterSchema.newQuery();
    queryV->addToTableList( "FWCAENCHANNEL" );
    queryV->addToOutputList( "FWCAENCHANNEL.DPID", "DPID" );
    queryV->addToOutputList( "FWCAENCHANNEL.CHANGE_DATE", "TSTAMP" );
    queryV->addToOutputList( "FWCAENCHANNEL.ACTUAL_VMON", "VMON" );
    coral::AttributeList VcondData;
    VcondData.extend<coral::TimeStamp>( "tsince" );
    VcondData.extend<coral::TimeStamp>( "tuntil" );
    //    VcondData.extend<int>( "dpid" );
    VcondData["tsince"].data<coral::TimeStamp>() = tsince;
    coral::TimeStamp tuntil(2011,2,10,0,0,0,0);
    VcondData["tuntil"].data<coral::TimeStamp>() = tuntil;
    
    // VcondData["dpid"].data<int>() = dpid;
    std::string Vcond1= " NOT FWCAENCHANNEL.ACTUAL_VMON IS NULL AND FWCAENCHANNEL.CHANGE_DATE >:tsince and FWCAENCHANNEL.CHANGE_DATE <:tuntil";
    //std::string Vcond3= "and FWCAENCHANNEL.DPID =:dpid";
    std::string Vcond3= "and FWCAENCHANNEL.DPID IN "+pdrange.str();
    std::string Vcond2=" AND FWCAENCHANNEL.ACTUAL_VMON > 900 and FWCAENCHANNEL.ACTUAL_VMON < 6500";
    std::string Vcond = Vcond1+Vcond2+Vcond3;
    queryV->setCondition(Vcond,VcondData);
    coral::ICursor& vcur = queryV->execute();
    
    while(vcur.next()){
      const coral::AttributeList& Vrow=vcur.currentRow();
      coral::TimeStamp Vts = Vrow["TSTAMP"].data<coral::TimeStamp>();
      float val = Vrow["VMON"].data<float>();
      int dpid = Vrow["DPID"].data<float>();
      std::cout <<dpid<<" ===== "<<hvmap[dpid].alias
		<<" UnixTime "<<Vts.total_nanoseconds()/1000/1000/1000

		<<" date="<<Vts.day()<<"/"<<Vts.month()<<"/"<<Vts.year()<<" "
		<<Vts.hour()<<":"<<Vts.minute()<<"."<<Vts.second()
		<<" Vmon = "<<val
		<<std::endl;
    }
    delete queryV;
    masterSession->transaction().commit();
    //  }

  delete masterSession;

  
}



