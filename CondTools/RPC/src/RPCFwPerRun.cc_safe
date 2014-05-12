 /* 
 *  See header file for a description of this class.
 * \author O.Iorio, 
 * based on the class RPCFw by 
 * D. Pagano - Dip. Fis. Nucl. e Teo. & INFN Pavia
 */

#include "CondTools/RPC/interface/RPCFwPerRun.h"
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
#include <iostream>
#include <stdexcept>
#include <vector>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include "CondFormats/RPCObjects/interface/RPCObFebmap.h"

RPCFwPerRun::RPCFwPerRun( const std::string& connectionString,
			  const std::string& userName,
			  const std::string& password,
			  const std::string& connectionStringPvssInfo,
			  const std::string& userNamePvssInfo,
			  const std::string& passwordPvssInfo,
			  const std::string& connectionStringRunInfo,
			  const std::string& userNameRunInfo,
			  const std::string& passwordRunInfo
			  ):
  RPCDBCom(),
  m_connectionString( connectionString ),
  m_userName( userName ),
  m_password( password ),
  m_connectionStringPvssInfo( connectionStringPvssInfo ),
  m_userNamePvssInfo( userNamePvssInfo ),
  m_passwordPvssInfo( passwordPvssInfo ),
  m_connectionStringRunInfo( connectionStringRunInfo ),
  m_userNameRunInfo( userNameRunInfo ),
  m_passwordRunInfo( passwordRunInfo )
{}


RPCFwPerRun::~RPCFwPerRun()
{}

void RPCFwPerRun::run()
{
}


//----------------------------- I M O N ------------------------------------------------------------------------
std::vector<RPCObImonPerLumi::I_Item> RPCFwPerRun::createIMONPerLumi(long long firstrun, long long lastrun)
{

  std::cout << ">>processing runs from  "<< firstrun << " to "<< lastrun << std::endl;

  
  std::vector<float> dpids;
  std::map<float, std::vector< coral::TimeStamp > > dpids_initial_times;
  std::map<float, std::vector< float > > dpids_initial_values;
  std::map<float, std::vector< float > > dpids_values;
  std::map<float, std::vector< coral::TimeStamp> > dpids_times;

  coral::ISession* sessionRunInfo = this->connect( m_connectionStringRunInfo,
                                            m_userNameRunInfo, m_passwordRunInfo );
  
  sessionRunInfo->transaction().start( true );
  coral::ISchema& schemaRunInfo = sessionRunInfo->nominalSchema();
    
  std::cout << "connection "<< m_connectionStringRunInfo<< " " << " userName "<< m_userNameRunInfo << " passwd "<< m_passwordRunInfo << std::endl;


  //  coral::ISession* sessionPvssInfo = this->connect( m_connectionStringPvssInfo,
  //                                          m_userNamePvssInfo, m_passwordPvssInfo );
  
  //  sessionPvssInfo->transaction().start( true );
  //  coral::ISchema& schemaPvssInfo = sessionPvssInfo->nominalSchema();
    
  //  std::cout << "connection "<< m_connectionStringPvssInfo<< " " << " userName "<< m_userNamePvssInfo << " passwd "<< m_passwordPvssInfo << std::endl;
  
  
  coral::ISession* session = this->connect( m_connectionString,
					    m_userName, m_password );
  

  session->transaction().start( true );
  coral::ISchema& schema = session->nominalSchema();
  

  
  std::vector<RPCObImonPerRun::I_Item> imonrunarray;
  std::vector<RPCObImonPerLumi::I_Item> imonlumiarray;
  //  
  RPCObImonPerLumi::I_Item Itemp;


  {
    coral::IQuery* queryZero = schema.newQuery();
    queryZero->addToTableList( "FWCAENCHANNEL" );
    queryZero->addToOutputList( "FWCAENCHANNEL.DPID", "DPID" );
    queryZero->setDistinct();
    
    coral::ICursor& cursorZero = queryZero->execute();
    while ( cursorZero.next() ) {
      const coral::AttributeList& rowZero = cursorZero.currentRow();
      float did  = rowZero["DPID"].data<float>();
      std::cout<< " did "<< did << std::endl; 
      dpids.push_back(did);
    }
  }

  //  dpids.push_back(143494);
  //dpids.push_back(37073);
  //dpids.push_back(9397);

    
  for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
    float did = dpids.at(id_ind);
    dpids_values[did].clear();
    dpids_times[did].clear();
    dpids_initial_values[did].clear();
    dpids_initial_values[did].push_back(-1.0);
    dpids_initial_times[did].clear();
  }
  
  
  for (long long int run = firstrun; run <=lastrun; ++run){
    std::vector<coral::TimeStamp> starttimes;
    std::vector<coral::TimeStamp> stoptimes;
    coral::AttributeList conditionDataRun;
    coral::AttributeList conditionDataRunRPC;
    coral::AttributeList conditionDataRunCMSRPC;
    coral::IQuery* queryRunInfo = schemaRunInfo.newQuery();

    
    

    queryRunInfo->addToTableList( "LUMI_SECTIONS" );
    queryRunInfo->addToOutputList( "STARTTIME", "STARTTIME" );
    queryRunInfo->addToOutputList( "STOPTIME", "STOPTIME" );
    
    queryRunInfo->addToOutputList( "RUNNUMBER", "RUNNUMBER" );
    queryRunInfo->addToOutputList( "LUMISECTION", "LUMISECTION" );
    
    conditionDataRun.extend<long long int>( "run" );
    //    conditionDataRun.extend<coral::TimeStamp>( "START" );
    conditionDataRun["run"].data<long long int>() = run;
    std::string conditionRun = "RUNNUMBER = :run"; 
    std::cout << "run is "<<run << std::endl;
    queryRunInfo->setCondition( conditionRun , conditionDataRun );
    coral::ICursor& cursorRunInfo = queryRunInfo->execute();
    bool empty = true;
    while ( cursorRunInfo.next() ) {
      empty = false;
      const coral::AttributeList& row = cursorRunInfo.currentRow();
      coral::TimeStamp start =  row["STARTTIME"].data<coral::TimeStamp>();
      coral::TimeStamp stop =  row["STOPTIME"].data<coral::TimeStamp>();
      //      int isready =  row["ISREADY"].data<bool>();
      int lumisection =  row["LUMISECTION"].data<int>();
      long long runnumbertest=  row["RUNNUMBER"].data<int>();
      int ndate = (start.day() * 10000) + (start.month() * 100) + (start.year()-2000);
      int ntime = (start.hour() * 10000) + (start.minute() * 100) + start.second();
      //      std::cout << " testing run number " << runnumbertest << " lumi number "<< lumisection << " timestamp start " << ntime  << std::endl;
      starttimes.push_back(start);
      stoptimes.push_back(stop);
    }
    //Condition on the run:

    coral::AttributeList conditionData;
    coral::AttributeList conditionDataZero;
    coral::AttributeList conditionDataPrevious;
    coral::AttributeList conditionDataPreviousRunInDB;
    coral::TimeStamp start,stop,previousStart,previousStop;
    conditionData.extend<coral::TimeStamp>( "start" );
    conditionData.extend<coral::TimeStamp>( "stop" );
    conditionDataPrevious.extend<coral::TimeStamp>( "previousstart" );
    conditionDataPrevious.extend<coral::TimeStamp>( "previousstop" );
    conditionDataPreviousRunInDB.extend<coral::TimeStamp>( "start" );
    conditionDataPreviousRunInDB.extend<coral::TimeStamp>( "previousstart" );
    conditionDataPreviousRunInDB.extend<coral::TimeStamp>( "previousstop" );
    
    size_t nlumis = starttimes.size();
    
    if(nlumis >0){
      start = starttimes.at(0);
      stop = stoptimes.at(nlumis-1); 
      previousStart= starttimes.at(0);
      previousStop= starttimes.at(0);
    }
    
    for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
      float did = dpids.at(id_ind);
      dpids_initial_times[did].push_back(start);
    }
    

    conditionData["start"].data<coral::TimeStamp>() = start;
    conditionData["stop"].data<coral::TimeStamp>() = stop;
    //    conditionData["previous"].data<coral::TimeStamp>() = previous= starttimes.at(0);
    
    //Fetch the first condition previous to the time interval:
    std::string condition = "FWCAENCHANNEL.ACTUAL_IMON IS NOT NULL AND FWCAENCHANNEL.CHANGE_DATE <:start AND FWCAENCHANNEL.CHANGE_DATE> :previous";

    //Fetch the conditions during the time interval of the run:

    std::cout << " starttimes size " <<starttimes.size()<< " nlum   " << nlumis<<std::endl;
    //  dpids.push_back();
    //    dpids.push_back(143494);
    //dpids.push_back(37073);
    //dpids.push_back(9397);
    //dpids.push_back(256);
    //dpids.push_back(126);
    //dpids.push_back(19);

  
    
    size_t count_dpids_with_previous=0;
    for(size_t nlum = 0;nlum < nlumis;++nlum){

      std::cout << " now for did "<< 9397 << " dpid initial value is " << dpids_initial_values[9397].at(0)<< std::endl;
    
      bool isfirst = (nlum==0);
      
      double valmean=0;
      long int timemean=0;
      
      start = starttimes.at(nlum);
      stop = stoptimes.at(nlum);
      
      std::cout << "check lumi "<<nlum << std::endl;
      
      conditionData["start"].data<coral::TimeStamp>() = start;
      conditionData["stop"].data<coral::TimeStamp>() = stop;
      
      
      conditionDataPrevious["previousstart"].data<coral::TimeStamp>() = previousStart;
      conditionDataPrevious["previousstop"].data<coral::TimeStamp>() = previousStop;
      
      //First part: make sure to have
      bool allDpidsHavePrevious = false;
      std::cout <<"  run is " << run<<std::endl;
      previousStart = start; 
      previousStop = start; 
      
      int nDaysToLookBack = 0;

      for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
	float did = dpids.at(id_ind);
	dpids_values[did].clear();
	dpids_times[did].clear();
      }
      
      for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
	float did = dpids.at(id_ind);
	if(dpids_initial_values[did].at(0)>=0){
	  dpids_values[did].push_back(dpids_initial_values[did].at(0));
	  ++count_dpids_with_previous;
	}
	std::cout << " dpid "<< did << " initial value  "<< dpids_initial_values[did].at(0) <<" countwithprevious "<< count_dpids_with_previous<< std::endl;
      }
      allDpidsHavePrevious = (count_dpids_with_previous == dpids.size());
      
      long int DeltaDays=1;
      bool hasPreviousRunInDB = false;
      
      std::cout << " is it? "<< (allDpidsHavePrevious==false && isfirst)<<std::endl;
      if (!hasPreviousRunInDB && !empty){
	  
	while(allDpidsHavePrevious==false && isfirst ){
	  
	  std::cout<<" test should I be in this loop"<< !allDpidsHavePrevious << std::endl; 
	  long int Day = 86400;
	  long int nWeekConvStart = Day*(nDaysToLookBack+DeltaDays);
	  long int nWeekConvStop = Day*(nDaysToLookBack);
	  
	  //	std::cout <<"nwconv start " <<  nWeekConvStart<<std::endl;
	  //	std::cout <<"nwconv start " <<  nWeekConvStop<<std::endl;
	  //	std::cout <<"start " <<  start.total_nanoseconds()/1000000000<<std::endl;
	  //	std::cout <<"nwconv UT " << TtoUT(start) <<std::endl;
	  //	std::cout<<" double invert "<< (UTtoT(TtoUT(start)+86400)).total_nanoseconds()<<std::endl;
	  //	std::cout<<" double invert v2 "<< (UTtoT(start.total_nanoseconds()/1000000000)).total_nanoseconds()<<std::endl;
	  
	  long int utotstart = start.total_nanoseconds()/1000000000;
	  
	  previousStart =  UTtoT(utotstart-nWeekConvStart);
	  previousStop = UTtoT(utotstart-nWeekConvStop);
	  
	  conditionDataPrevious["previousstart"].data<coral::TimeStamp>() = previousStart;
	  conditionDataPrevious["previousstop"].data<coral::TimeStamp>() = previousStop;
	  
	  coral::IQuery* queryPrevious = schema.newQuery();
	  queryPrevious->addToTableList( "FWCAENCHANNEL" );
	  queryPrevious->addToOutputList( "FWCAENCHANNEL.DPID", "DPID" );
	  queryPrevious->addToOutputList( "FWCAENCHANNEL.CHANGE_DATE", "TSTAMP" );
	  queryPrevious->addToOutputList( "FWCAENCHANNEL.ACTUAL_IMON", "IMON" );
	  condition = "FWCAENCHANNEL.ACTUAL_IMON IS NOT NULL AND FWCAENCHANNEL.CHANGE_DATE >:previousstart AND FWCAENCHANNEL.CHANGE_DATE <:previousstop ORDER BY FWCAENCHANNEL.CHANGE_DATE";
	  queryPrevious->setCondition( condition , conditionDataPrevious );
	  coral::ICursor& cursorPrev = queryPrevious->execute();
	  
	  std::cout<< " weeks looking back "<< nDaysToLookBack << " looking between UTs Start "<< previousStart.total_nanoseconds() << " Stop " << previousStop.total_nanoseconds()<< " delta days "<< DeltaDays <<std::endl;
	  
	  std::cout << "dpids size "<< dpids.size()<<std::endl;
	  
	  while ( cursorPrev.next() ) {
	    const coral::AttributeList& rowPrev = cursorPrev.currentRow();
	    float did  = rowPrev["DPID"].data<float>();
	    float val = rowPrev["IMON"].data<float>();
	    coral::TimeStamp ts =  rowPrev["TSTAMP"].data<coral::TimeStamp>();
	    if(!dpids_values[did].size()>0) {
	      if(dpids_initial_times[did].size()>0) dpids_initial_times[did].at(0) = ts;
	      else dpids_initial_times[did].push_back(ts);
	      if(dpids_initial_values[did].size()>0) dpids_initial_values[did].at(0) = val;
	      else dpids_initial_values[did].push_back(val);
	    }
	  }
	
	  //count_dpids_with_previous=0;
	  for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
	    float did = dpids.at(id_ind);
	    if(dpids_values[did].size()>0)continue;
	    
	    if(dpids_initial_values[did].size()!=0){std::cout << "dpid "<< did << " has init val "<< dpids_initial_values[did].at(0)  <<std::endl ;
	      if(dpids_initial_values[did].at(0)>=0){
		dpids_values[did].push_back(dpids_initial_values[did].at(0));
		++count_dpids_with_previous;
		std::cout << "noww filling dpid "<< did << " with " << dpids_initial_values[did].at(0)<< " count now is "<< count_dpids_with_previous <<std::endl;
		std::cout << "ts is " << dpids_initial_times[did].at(0).total_nanoseconds() <<std::endl;
	      }
	    }
	  }
	  std::cout << " alldipidshasprevious  "<< allDpidsHavePrevious <<" nweeks" << nDaysToLookBack<<std::endl;
	  allDpidsHavePrevious = (count_dpids_with_previous == dpids.size());
	
	  if ( nDaysToLookBack>=32 ){
	    break; //FIX IT: THERE ARE PATOLOGICAL CASES WITH INACTIVE CHAMBERS!!!
	  } 
	  //if ( nDaysToLookBack<=10 ){
	  //  ;//	  ++nDaysToLookBack;
	  //}
	  //if ( nDaysToLookBack>=10 && nDaysToLookBack< 100 ){
	  // DeltaDays=10;
	  //}
	  //if ( nDaysToLookBack>=100 && nDaysToLookBack< 1000 ){
	  //  DeltaDays=100;
	  //}
	  //if ( nDaysToLookBack>=1000 && nDaysToLookBack< 5000 ){
	  //  DeltaDays=500;
	  //}
	  nDaysToLookBack+=DeltaDays;
	  //
	if ( allDpidsHavePrevious == true )break; 
	
	}
	std::cout <<  " I am free "<<std::endl;
      }
    
    
      coral::IQuery* query = schema.newQuery();
      query->addToTableList( "FWCAENCHANNEL" );
      query->addToOutputList( "FWCAENCHANNEL.DPID", "DPID" );
      query->addToOutputList( "FWCAENCHANNEL.CHANGE_DATE", "TSTAMP" );
      query->addToOutputList( "FWCAENCHANNEL.ACTUAL_IMON", "IMON" );
      condition = "FWCAENCHANNEL.ACTUAL_IMON IS NOT NULL AND FWCAENCHANNEL.CHANGE_DATE >:start AND FWCAENCHANNEL.CHANGE_DATE <:stop ORDER BY FWCAENCHANNEL.CHANGE_DATE";
      query->setCondition( condition , conditionData );
      
    
      coral::ICursor& cursorI = query->execute();
      
      for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
	dpids_times[dpids.at(id_ind)].push_back(start);
      }
      //values.push_back(previousVal);
    
      std::cout <<  " I a about to do the query "<<std::endl;
      while ( cursorI.next() ) {
	const coral::AttributeList& row = cursorI.currentRow();
	float did = row["DPID"].data<float>();
	float val = row["IMON"].data<float>();
	coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
	dpids_values[did].push_back(val);
	dpids_times[did].push_back(ts);
      }
      for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
	dpids_times[dpids.at(id_ind)].push_back(stop);
      }
    
      for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
	std::cout <<  " loop on the averages "<< id_ind <<std::endl;
	valmean =0;
	long int total_time_check=stop.total_nanoseconds()-start.total_nanoseconds();
	long int total_time=0;
	float did = dpids.at(id_ind);
	std::cout << " did "<< did << " initial time "<<  dpids_initial_times[did].at(0).total_nanoseconds()<<std::endl; 
	for (size_t it_values =0;it_values<dpids_values[did].size();++it_values){
	  long int timeinit = dpids_times[did].at(it_values).total_nanoseconds(); 
	  long int timeofchange = dpids_times[did].at(it_values+1).total_nanoseconds(); 
	  long int time = dpids_times[did].at(it_values+1).total_nanoseconds() - dpids_times[did].at(it_values).total_nanoseconds(); 
	  std::cout << " valmean " << valmean << std::endl;
	  valmean += dpids_values[did].at(it_values)*time;
	  total_time +=time;
	  std::cout << " value change "<< dpids_values[did].at(it_values)<< " time init "<< timeinit<< " time of change "<< timeofchange << " deltatime in secs. "<< time/1000000000. <<std::endl;
	  std::cout << " valformean "<< dpids_values[did].at(it_values)*time;
	  
	}
	std::cout << " dpid "<< did << "check lumi "<<  nlum << " start time " << start.total_nanoseconds() << " end time "<< stop.total_nanoseconds() <<" valmean " <<valmean <<std::endl;
	std::cout << "check lumi "<< nlum << " time 1 "<< total_time_check<< " time 2 "<< total_time << std::endl;
	valmean = valmean/total_time;
	std::cout << "valmean "<< valmean<<std::endl;
      
	Itemp.value = valmean;
	Itemp.run = run;
	Itemp.dpid = did;
	Itemp.lumi = nlum+1;
	//      start = stop;
	if(!( dpids_initial_values[did].at(0)<0.)){
	  Itemp.status = 0;
	  float tmpp = dpids_values[did].at(dpids_values[did].size()-1);
	  (dpids_initial_values[did].at(0))=tmpp;
	}
	else{ Itemp.value = -1;  }
	imonlumiarray.push_back(Itemp);

	std::cout << " now for did "<< did << " dpid initial value is " << dpids_initial_values[did].at(0)<< std::endl;
      }
      
      //    dpids.push_back(9397);
      //     dpids.push_back(256);
 
      //      std::cout << " now for did "<< 9397 << " dpid initial value is " << (dpids_initial_values[9397].at(0))<< std::endl;
    }

  }
  long since = 0, till = 1;
  sessionRunInfo->transaction().commit();
  delete sessionRunInfo;


 
  /*  tMIN = UTtoT(since);
      
  std::vector <  long long > lumisections;
  std::vector <  long long > runs;
  


  std::cout <<">> Processing since: "<<tMIN.day()<<"/"<<tMIN.month()<<"/"<<tMIN.year()<<" "<<tMIN.hour()<<":"<<tMIN.minute()<<"."<<tMIN.second()<< std::endl;
  int nRows = 0;
  coral::IQuery* queryI = schema.newQuery();
  queryI->addToTableList( "FWCAENCHANNEL" );
  queryI->addToOutputList( "FWCAENCHANNEL.DPID", "DPID" );
  queryI->addToOutputList( "FWCAENCHANNEL.CHANGE_DATE", "TSTAMP" );
  queryI->addToOutputList( "FWCAENCHANNEL.ACTUAL_IMON", "IMON" );
  
  RPCObImonPerRun::I_Item Itemp;
  coral::TimeStamp tlast = tMIN;
  if (till > since) {
    tMAX = UTtoT(till);
    std::cout <<">> Processing till: "<<tMAX.day()<<"/"<<tMAX.month()<<"/"<<tMAX.year()<<" "<<tMAX.hour()<<":"<<tMAX.minute()<<"."<<tMAX.second()<< std::endl;
    std::cout << ">> creating IMON object..." << std::endl;
    
  } else {
    std::cout << ">> creating IMON object..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string condition = "FWCAENCHANNEL.ACTUAL_IMON IS NOT NULL AND FWCAENCHANNEL.CHANGE_DATE >:tmin";
    queryI->setCondition( condition , conditionData );
    coral::ICursor& cursorI = queryI->execute();
    while ( cursorI.next() ) {
      const coral::AttributeList& row = cursorI.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float val = row["IMON"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      if (isMajor(ts, tlast)) tlast = ts;
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.value = val;
      //Itemp.day = ndate;
      //Itemp.time = ntime;
      imonarray.push_back(Itemp);
      ++nRows;
    }
  }
  N_IOV = TtoUT(tlast);
  if (till > since) N_IOV = till;
  std::cout << ">> Imon array --> size: " << imonarray.size() << " >> done." << std::endl;
  delete queryI;
  session->transaction().commit();
  delete session;*/
  return imonlumiarray;
  //  return imonarlumiray;
}




std::vector<RPCObImonPerRun::I_Item> RPCFwPerRun::createIMON(long long firstrun, long long lastrun)
{

  std::cout << ">>processing runs from  "<< firstrun << " to "<< lastrun << std::endl;

  
  std::vector<float> dpids;
  std::map<float, std::vector< coral::TimeStamp > > dpids_initial_times;
  std::map<float, std::vector< float > > dpids_initial_values;
  std::map<float, std::vector< float > > dpids_values;
  std::map<float, std::vector< coral::TimeStamp> > dpids_times;

  coral::ISession* sessionRunInfo = this->connect( m_connectionStringRunInfo,
                                            m_userNameRunInfo, m_passwordRunInfo );
  
  sessionRunInfo->transaction().start( true );
  coral::ISchema& schemaRunInfo = sessionRunInfo->nominalSchema();
    
  std::cout << "connection "<< m_connectionStringRunInfo<< " " << " userName "<< m_userNameRunInfo << " passwd "<< m_passwordRunInfo << std::endl;
  
  
  coral::ISession* session = this->connect( m_connectionString,
					    m_userName, m_password );
  

  session->transaction().start( true );
  coral::ISchema& schema = session->nominalSchema();
  

  
  std::vector<RPCObImonPerRun::I_Item> imonrunarray;
  std::vector<RPCObImonPerLumi::I_Item> imonlumiarray;
  //  
  RPCObImonPerRun::I_Item Itemp;


  {
    coral::IQuery* queryZero = schema.newQuery();
    queryZero->addToTableList( "FWCAENCHANNEL" );
    queryZero->addToOutputList( "FWCAENCHANNEL.DPID", "DPID" );
    queryZero->setDistinct();
    
        coral::ICursor& cursorZero = queryZero->execute();
    while ( cursorZero.next() ) {
      const coral::AttributeList& rowZero = cursorZero.currentRow();
      float did  = rowZero["DPID"].data<float>();
      std::cout<< " did "<< did << std::endl; 
      dpids.push_back(did);
      }
  }

  //dpids.push_back(143494);
  //dpids.push_back(37073);
  //dpids.push_back(9397);

    

  for (long long int run = firstrun; run <=lastrun; ++run){

    for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
      float did = dpids.at(id_ind);
      dpids_values[did].clear();
      dpids_times[did].clear();
      dpids_initial_values[did].clear();
      dpids_initial_values[did].push_back(-1.0);
      dpids_initial_times[did].clear();
    }
    
    std::vector<coral::TimeStamp> starttimes;
    std::vector<coral::TimeStamp> stoptimes;
    std::map< long long int, float > is_rpcready;
    std::map< long long int, float > is_cmsrpcready;
    coral::AttributeList conditionDataRun;
    coral::AttributeList conditionDataRunRPC;
    coral::AttributeList conditionDataRunCMSRPC;
    coral::IQuery* queryRunInfo = schemaRunInfo.newQuery();
    
    coral::IQuery* queryRunInfoRPC = schemaRunInfo.newQuery();
    coral::IQuery* queryRunInfoCMSRPC = schemaRunInfo.newQuery();

    queryRunInfo->addToTableList( "LUMI_SECTIONS" );
    queryRunInfo->addToOutputList( "STARTTIME", "STARTTIME" );
    queryRunInfo->addToOutputList( "STOPTIME", "STOPTIME" );
    queryRunInfo->addToOutputList( "RUNNUMBER", "RUNNUMBER" );
    queryRunInfo->addToOutputList( "LUMISECTION", "LUMISECTION" );

    queryRunInfo->addToOutputList( "RPC_READY", "ISRPCREADY" );
    queryRunInfo->addToOutputList( "CMS_ACTIVE", "ISCMSACTIVE" );

    /* 

       queryRunInfoRPC->addToOutputList( "RPC_READY", "ISREADY" );
       queryRunInfoCMSRPC->addToOutputList( "RPC_READY", "ISREADY" );
       queryRunInfoCMSRPC->addToOutputList( "CMS_ACTIVE", "ISCMSACTIVE" );
    */
    conditionDataRun.extend<long long int>( "run" );
    
    //    conditionDataRunRPC.extend<long long int>( "run" );
    //conditionDataRunRPC.extend< int>( "isrpcready" );
    
    //    conditionDataRunCMSRPC.extend<long long int>( "run" );
    //conditionDataRunCMSRPC.extend< int>( "isrpcready" );
    //conditionDataRunCMSRPC.extend< int>( "iscmsready" );
    
    //    conditionDataRun.extend<coral::TimeStamp>( "START" );
    conditionDataRun["run"].data<long long int>() = run;
    std::string conditionRun = "RUNNUMBER = :run"; 
    std::string conditionRunRPC = "RUNNUMBER = :run and RPC_READY > 0 "; 
    std::string conditionRunCMSRPC = "RUNNUMBER = :run and RPC_READY > 0 and CMS_ACTIVE > 0"; 
    std::cout << "run is "<<run << std::endl;
    queryRunInfo->setCondition( conditionRun , conditionDataRun );
    //    queryRunInfoRPC->setCondition( conditionRunRPC , conditionDataRunRPC );
    //queryRunInfoCMSRPC->setCondition( conditionRunCMSRPC , conditionDataRunCMSRPC );

    coral::ICursor& cursorRunInfo = queryRunInfo->execute();
    bool empty = true;
    while ( cursorRunInfo.next() ) {
      empty = false;
      const coral::AttributeList& row = cursorRunInfo.currentRow();
      coral::TimeStamp start =  row["STARTTIME"].data<coral::TimeStamp>();
      coral::TimeStamp stop =  row["STOPTIME"].data<coral::TimeStamp>();
      float isrpcready =  row["ISRPCREADY"].data<bool>();
      float iscmsactive =  row["ISCMSACTIVE"].data<bool>();
      int lumisection =  row["LUMISECTION"].data<int>();
      long long runnumbertest=  row["RUNNUMBER"].data<int>();
      int ndate = (start.day() * 10000) + (start.month() * 100) + (start.year()-2000);
      int ntime = (start.hour() * 10000) + (start.minute() * 100) + start.second();
      std::cout << " testing run number " << runnumbertest << " lumi number "<< lumisection << " timestamp start " << ntime  << " is rpc ready " << " is cms active "<< iscmsactive << std::endl;
      
      
      is_rpcready[start.total_nanoseconds()]=isrpcready;
      is_cmsrpcready[start.total_nanoseconds()]=iscmsactive * isrpcready;    
      
      starttimes.push_back(start);
      stoptimes.push_back(stop);
    }
    std::cout << " empty? "<< empty<< std::endl;
    //Condition on the run:
    
    coral::AttributeList conditionData;
    coral::AttributeList conditionDataZero;
    coral::AttributeList conditionDataPrevious;
    coral::TimeStamp start,stop,previousStart,previousStop;
    conditionData.extend<coral::TimeStamp>( "start" );
    conditionData.extend<coral::TimeStamp>( "stop" );
    conditionDataPrevious.extend<coral::TimeStamp>( "previousstart" );
    conditionDataPrevious.extend<coral::TimeStamp>( "previousstop" );
    
    size_t nlumis = starttimes.size();
    
    if(nlumis >0){
      start = starttimes.at(0);
      stop = stoptimes.at(nlumis-1); 
      previousStart= starttimes.at(0);
      previousStop= starttimes.at(0);
    }
    
    for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
      float did = dpids.at(id_ind);
      dpids_initial_times[did].push_back(start);
    }
    

    conditionData["start"].data<coral::TimeStamp>() = start;
    conditionData["stop"].data<coral::TimeStamp>() = stop;
    //    conditionData["previous"].data<coral::TimeStamp>() = previous= starttimes.at(0);

    //Fetch the first condition previous to the time interval:
    std::string condition = "FWCAENCHANNEL.ACTUAL_IMON IS NOT NULL AND FWCAENCHANNEL.CHANGE_DATE <:start AND FWCAENCHANNEL.CHANGE_DATE> :previous";

    //Fetch the conditions during the time interval of the run:

    std::cout << " starttimes size " <<starttimes.size()<< " nlum   " << nlumis<<std::endl;
    //  dpids.push_back();
    //    dpids.push_back(143494);
    //    dpids.push_back(37073);
    //    dpids.push_back(9397);
    //dpids.push_back(256);
    //dpids.push_back(126);
    //dpids.push_back(19);
    
  
    
    size_t count_dpids_with_previous=0;

      std::cout << " now for did "<< 9397 << " dpid initial value is " << dpids_initial_values[9397].at(0)<< std::endl;

      bool isfirst =true;// (run==firstrun );
      
      double valmean=0;
      double valmeanrpcready =0;
      double valmeancmsrpcready =0;

      long int timemean=0;
      long int timemeanrpcready=0;
      long int timemeancmsrpcready=0;
            
      
      conditionData["start"].data<coral::TimeStamp>() = start;
      conditionData["stop"].data<coral::TimeStamp>() = stop;
      
      
      conditionDataPrevious["previousstart"].data<coral::TimeStamp>() = previousStart;
      conditionDataPrevious["previousstop"].data<coral::TimeStamp>() = previousStop;
      
      //First part: make sure to have
      bool allDpidsHavePrevious = false;
      
      previousStart = start; 
      previousStop = start; 
      
      int nDaysToLookBack = 0;

      for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
	float did = dpids.at(id_ind);
	dpids_values[did].clear();
	dpids_times[did].clear();
      }
      
      for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
	float did = dpids.at(id_ind);
	if(dpids_initial_values[did].at(0)>=0){
	  dpids_values[did].push_back(dpids_initial_values[did].at(0));
	  ++count_dpids_with_previous;
	}
	std::cout << " dpid "<< did << " initial value  "<< dpids_initial_values[did].at(0) <<" countwithprevious "<< count_dpids_with_previous<< std::endl;
      }
      allDpidsHavePrevious = (count_dpids_with_previous == dpids.size());
      
      long int DeltaDays=1;
    
      while(allDpidsHavePrevious==false && isfirst  && !empty){

	std::cout<<" test should I be in this loop"<< !allDpidsHavePrevious << std::endl; 
	long int Day = 86400;
	long int nWeekConvStart = Day*(nDaysToLookBack+DeltaDays);
	long int nWeekConvStop = Day*(nDaysToLookBack);
       
	//	std::cout <<"nwconv start " <<  nWeekConvStart<<std::endl;
	//	std::cout <<"nwconv start " <<  nWeekConvStop<<std::endl;
	//	std::cout <<"start " <<  start.total_nanoseconds()/1000000000<<std::endl;
	//	std::cout <<"nwconv UT " << TtoUT(start) <<std::endl;
	//	std::cout<<" double invert "<< (UTtoT(TtoUT(start)+86400)).total_nanoseconds()<<std::endl;
	//	std::cout<<" double invert v2 "<< (UTtoT(start.total_nanoseconds()/1000000000)).total_nanoseconds()<<std::endl;
	
	long int utotstart = start.total_nanoseconds()/1000000000;
	
	previousStart =  UTtoT(utotstart-nWeekConvStart);
	previousStop = UTtoT(utotstart-nWeekConvStop);
	
	conditionDataPrevious["previousstart"].data<coral::TimeStamp>() = previousStart;
	conditionDataPrevious["previousstop"].data<coral::TimeStamp>() = previousStop;
	
	coral::IQuery* queryPrevious = schema.newQuery();
	queryPrevious->addToTableList( "FWCAENCHANNEL" );
	queryPrevious->addToOutputList( "FWCAENCHANNEL.DPID", "DPID" );
	queryPrevious->addToOutputList( "FWCAENCHANNEL.CHANGE_DATE", "TSTAMP" );
	queryPrevious->addToOutputList( "FWCAENCHANNEL.ACTUAL_IMON", "IMON" );
	condition = "FWCAENCHANNEL.ACTUAL_IMON IS NOT NULL AND FWCAENCHANNEL.CHANGE_DATE >:previousstart AND FWCAENCHANNEL.CHANGE_DATE <:previousstop ORDER BY FWCAENCHANNEL.CHANGE_DATE";
	queryPrevious->setCondition( condition , conditionDataPrevious );
	coral::ICursor& cursorPrev = queryPrevious->execute();
	
	std::cout<< " weeks looking back "<< nDaysToLookBack << " looking between UTs Start "<< previousStart.total_nanoseconds() << " Stop " << previousStop.total_nanoseconds()<< " delta days "<< DeltaDays <<std::endl;
	
	std::cout << "dpids size "<< dpids.size()<<std::endl;
    
	while ( cursorPrev.next() ) {
	  const coral::AttributeList& rowPrev = cursorPrev.currentRow();
	  float did  = rowPrev["DPID"].data<float>();
	  float val = rowPrev["IMON"].data<float>();
	  coral::TimeStamp ts =  rowPrev["TSTAMP"].data<coral::TimeStamp>();
	  if(!dpids_values[did].size()>0) {
	    if(dpids_initial_times[did].size()>0) dpids_initial_times[did].at(0) = ts;
	    else dpids_initial_times[did].push_back(ts);
	    if(dpids_initial_values[did].size()>0) dpids_initial_values[did].at(0) = val;
	    else dpids_initial_values[did].push_back(val);
	  }
	}
	
	//count_dpids_with_previous=0;
	for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
	  float did = dpids.at(id_ind);
	  if(dpids_values[did].size()>0)continue;
	  
	  if(dpids_initial_values[did].size()!=0){std::cout << "dpid "<< did << " has init val "<< dpids_initial_values[did].at(0)  <<std::endl ;
	    if(dpids_initial_values[did].at(0)>=0){
	      dpids_values[did].push_back(dpids_initial_values[did].at(0));
	      ++count_dpids_with_previous;
	      std::cout << "noww filling dpid "<< did << " with " << dpids_initial_values[did].at(0)<< " count now is "<< count_dpids_with_previous <<std::endl;
	      std::cout << " did size is " << dpids_values[did].size()<<std::endl;
	      //	      std::cout << "ts is " << dpids_initial_times[did].at(0).total_nanoseconds() <<std::endl;
	    }
	  }
	}
	std::cout << " alldipidshasprevious  "<< allDpidsHavePrevious <<" nweeks" << nDaysToLookBack<<std::endl;
	allDpidsHavePrevious = (count_dpids_with_previous == dpids.size());
	std::cout << " dipids with previous  "<< count_dpids_with_previous <<std::endl;

	if ( nDaysToLookBack>=32 ){
	  break; //FIX IT: THERE ARE PATOLOGICAL CASES WITH INACTIVE CHAMBERS!!!
	}
	//if ( nDaysToLookBack<=10 ){
	//  ;//	  ++nDaysToLookBack;
	//}
	//if ( nDaysToLookBack>=10 && nDaysToLookBack< 100 ){
	// DeltaDays=10;
	//}
	//if ( nDaysToLookBack>=100 && nDaysToLookBack< 1000 ){
	//  DeltaDays=100;
	//}
	//if ( nDaysToLookBack>=1000 && nDaysToLookBack< 5000 ){
	//  DeltaDays=500;
	//}
	nDaysToLookBack+=DeltaDays;
	//
	if ( allDpidsHavePrevious == true )break; 
	

	

      }
      std::cout <<  " I am free "<<std::endl;

      for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
	dpids_times[dpids.at(id_ind)].push_back(start);
      }
      
      
      for(size_t nlum = 0;nlum < nlumis;++nlum){
	
	if (empty) continue;

      	start = starttimes.at(nlum);
	stop = stoptimes.at(nlum);
	

	std::cout << "check lumi "<<nlum << std::endl;
	
	conditionData["start"].data<coral::TimeStamp>() = start;
	conditionData["stop"].data<coral::TimeStamp>() = stop;
      

	//	coral::IQuery* queryLumiInfo = schemaRunInfo.newQuery();
	//	coral::AttributeList conditionDataLumi;
	//	conditionDataLumi.extend<long long int>( "run" );
	//	conditionDataLumi["run"].data<long long int>() = run;
	//conditionDataLumi.extend<long long int>( "lumi" );
	//conditionDataLumi["lumi"].data<long long int>() = nlum+1;
	//std::string conditionLumi = "RUNNUMBER = :run and LUMISECTION = :lumi"; 
	//queryLumiInfo->addToTableList( "LUMI_SECTIONS" );
	//queryLumiInfo->addToOutputList( "STARTTIME", "STARTTIME" );
	//queryLumiInfo->addToOutputList( "RPC_READY", "ISRPCREADY" );
	//queryLumiInfo->addToOutputList( "CMS_ACTIVE", "ISCMSACTIVE" );
	//queryLumiInfo->setCondition( conditionLumi , conditionDataLumi );
	//coral::ICursor& cursorIA = queryLumiInfo->execute();

	float isrpcready =0.;
	float iscmsrpcready=0.;

	/*while ( cursorIA.next() ) {
	  const coral::AttributeList& row = cursorIA.currentRow();
	  isrpcready =  row["ISRPCREADY"].data<bool>();
	  iscmsactive =  row["ISCMSACTIVE"].data<bool>();
	  if(nlum =0){
	    is_rpcready[start.total_nanoseconds()]=isrpcready;
	    is_cmsrpcready[start.total_nanoseconds()]=iscmsactive;
    	  }

	  
	  }*/
	  
      
	isrpcready =  is_rpcready[start.total_nanoseconds()];
	iscmsrpcready =  is_cmsrpcready[start.total_nanoseconds()];
	std::cout<< " start at time " << start.total_nanoseconds() <<" isrpcready " << is_rpcready[start.total_nanoseconds()]<< std::endl;

	
	coral::IQuery* query = schema.newQuery();
	query->addToTableList( "FWCAENCHANNEL" );
	query->addToOutputList( "FWCAENCHANNEL.DPID", "DPID" );
	query->addToOutputList( "FWCAENCHANNEL.CHANGE_DATE", "TSTAMP" );
	query->addToOutputList( "FWCAENCHANNEL.ACTUAL_IMON", "IMON" );
	condition = "FWCAENCHANNEL.ACTUAL_IMON IS NOT NULL AND FWCAENCHANNEL.CHANGE_DATE >:start AND FWCAENCHANNEL.CHANGE_DATE <:stop ORDER BY FWCAENCHANNEL.CHANGE_DATE";
	query->setCondition( condition , conditionData );
	
	coral::ICursor& cursorI = query->execute();
	
	//values.push_back(previousVal);
	
	std::cout <<  " I am about to do the query "<<std::endl;
	
	std::cout<< " init time lumi "<< start.total_nanoseconds()/1000000000.<< " end time lumi "<< stop.total_nanoseconds()/1000000000.<<std::endl;

	//Do calculation lumi by lumi
	for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
	  float did = dpids.at(id_ind);
	  //	  std::cout << " did is address "<< (dpids_values[did]) << std::endl;
	  //	  std::cout << " did "<<did << " did size " << dpids_values[did].size() << " initial dpid "<< dpids_initial_values[did].at(0) << std::endl;
	  if(dpids_values[did].size()>0){ 
	       dpids_values[did].push_back(dpids_values[did].at(dpids_values[did].size()-1));
	       dpids_times[did].push_back(start);
	     }
	}
	
	while ( cursorI.next() ) {
	  const coral::AttributeList& row = cursorI.currentRow();
	  float did = row["DPID"].data<float>();
	  float val = row["IMON"].data<float>();
	  coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
	  
	  dpids_values[did].push_back(val);
	  dpids_times[did].push_back(ts);
	  //Itemp.run = (float)run;
	  
	  std::cout<< " did attime " << ts.total_nanoseconds() <<" isrpcready " << isrpcready<< std::endl;
	  is_rpcready[ts.total_nanoseconds()]=isrpcready;
	  is_cmsrpcready[ts.total_nanoseconds()]=iscmsrpcready;
	  
	}

      } 
       
      for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
	dpids_times[dpids.at(id_ind)].push_back(stop);
      
	std::cout << " now for did "<< dpids.at(id_ind) << " dpid initial value is " << dpids_initial_values[dpids.at(id_ind)].at(0)<< std::endl;
	//std::cout << " now for did "<< dpids.at(id_ind) << " dpid initial time is " << dpids_initial_times[dpids.at(id_ind)].at(0)<< std::endl;
      }
  
      for (size_t id_ind = 0;id_ind< dpids.size() ;++id_ind){
	if (!empty){
	std::cout <<  " loop on the averages "<< id_ind <<std::endl;
	valmean =0;
	//long int total_time_check=stop.total_nanoseconds()-start.total_nanoseconds();
	long int total_time=0;
	long int total_timerpcready=0;
	long int total_timecmsrpcready=0;
	float did = dpids.at(id_ind);
	std::cout << " did "<< did << " initial time "<<  dpids_initial_times[did].at(0).total_nanoseconds()<<std::endl; 
	
	for (size_t it_values =0;it_values<dpids_values[did].size();++it_values){
	  long int timeinit = dpids_times[did].at(it_values).total_nanoseconds(); 
	  long int timeofchange = dpids_times[did].at(it_values+1).total_nanoseconds(); 
	  
	  
	  long int time = dpids_times[did].at(it_values+1).total_nanoseconds() - dpids_times[did].at(it_values).total_nanoseconds(); 
	  std::cout << " valmean " << valmean << std::endl;
	  valmean += dpids_values[did].at(it_values)*time;
	  total_time +=time;
	  
	  
	  std::cout<< " did @time " << (dpids_times[did].at(it_values)).total_nanoseconds() <<" isrpcready "<< is_rpcready[(dpids_times[did].at(it_values)).total_nanoseconds()]<< " is cms rpc ready "<< is_cmsrpcready[(dpids_times[did].at(it_values)).total_nanoseconds()]<< " " << std::endl; 
	  if(is_rpcready[(dpids_times[did].at(it_values)).total_nanoseconds()]>0.){
	    std::cout<< " did @time " << (dpids_times[did].at(it_values)).total_nanoseconds() <<" isrpcready 1 " << std::endl;
	    total_timerpcready+=time;
	    valmeanrpcready+=dpids_values[did].at(it_values)*time;
	  }
	  
	  if(is_cmsrpcready[(dpids_times[did].at(it_values)).total_nanoseconds()]>0.){
	    total_timecmsrpcready+=time;
	    valmeancmsrpcready+=dpids_values[did].at(it_values)*time;
	  }
	  
	  
std::cout << " value change "<< dpids_values[did].at(it_values)<< " time init "<< timeinit<< " time of change "<< timeofchange << " deltatime in secs. "<< time/1000000000. <<std::endl;
	  std::cout << " valformean "<< dpids_values[did].at(it_values)*time;
	  
	}
	std::cout << " dpid "<< did <<" valmean " <<valmean <<std::endl;
	//std::cout << "check lumi "<< nlum << " time 1 "<< total_time_check<< " time 2 "<< total_time << std::endl;
	valmean = valmean/total_time;
	
	if(total_timerpcready >0)valmeanrpcready = valmeanrpcready/total_timerpcready;
	else valmeanrpcready =-1;

	if(total_timecmsrpcready >0)valmeancmsrpcready = valmeancmsrpcready/total_timecmsrpcready;
	else valmeancmsrpcready =-1;
	std::cout << "valmean "<< valmean<<std::endl;
	Itemp.value = valmean;
	Itemp.value_rpc_on = valmeanrpcready;
	Itemp.value_rpc_phys_on = valmeancmsrpcready;
	
	Itemp.dpid = did;
	Itemp.run = run;
	//      start = stop;
	if(!( dpids_initial_values[did].at(0)<0.)){
	  float tmpp = dpids_values[did].at(dpids_values[did].size()-1);
	  (dpids_initial_values[did].at(0))=tmpp;
	}
	else{ Itemp.value = -1;  }
	imonrunarray.push_back(Itemp);
	}
	else{
	  Itemp.value = -1;
	  Itemp.value_rpc_on = -1;
	  Itemp.value_rpc_phys_on = -1;
	  float did = dpids.at(id_ind);	
	  Itemp.dpid = did;
	  Itemp.run = run;
	}
	
      }
   
      //    dpids.push_back(9397);
      //     dpids.push_back(256);
    
      //      std::cout << " now for did "<< 9397 << " dpid initial value is " << (dpids_initial_values[9397].at(0))<< std::endl;
    

  }
  long since = 0, till = 1;
  sessionRunInfo->transaction().commit();
  delete sessionRunInfo;


 
  /*  tMIN = UTtoT(since);
      
  std::vector <  long long > lumisections;
  std::vector <  long long > runs;
  


  std::cout <<">> Processing since: "<<tMIN.day()<<"/"<<tMIN.month()<<"/"<<tMIN.year()<<" "<<tMIN.hour()<<":"<<tMIN.minute()<<"."<<tMIN.second()<< std::endl;
  int nRows = 0;
  coral::IQuery* queryI = schema.newQuery();
  queryI->addToTableList( "FWCAENCHANNEL" );
  queryI->addToOutputList( "FWCAENCHANNEL.DPID", "DPID" );
  queryI->addToOutputList( "FWCAENCHANNEL.CHANGE_DATE", "TSTAMP" );
  queryI->addToOutputList( "FWCAENCHANNEL.ACTUAL_IMON", "IMON" );
  
  RPCObImonPerRun::I_Item Itemp;
  coral::TimeStamp tlast = tMIN;
  if (till > since) {
    tMAX = UTtoT(till);
    std::cout <<">> Processing till: "<<tMAX.day()<<"/"<<tMAX.month()<<"/"<<tMAX.year()<<" "<<tMAX.hour()<<":"<<tMAX.minute()<<"."<<tMAX.second()<< std::endl;
    std::cout << ">> creating IMON object..." << std::endl;
    
  } else {
    std::cout << ">> creating IMON object..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string condition = "FWCAENCHANNEL.ACTUAL_IMON IS NOT NULL AND FWCAENCHANNEL.CHANGE_DATE >:tmin";
    queryI->setCondition( condition , conditionData );
    coral::ICursor& cursorI = queryI->execute();
    while ( cursorI.next() ) {
      const coral::AttributeList& row = cursorI.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float val = row["IMON"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      if (isMajor(ts, tlast)) tlast = ts;
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.value = val;
      //Itemp.day = ndate;
      //Itemp.time = ntime;
      imonarray.push_back(Itemp);
      ++nRows;
    }
  }
  N_IOV = TtoUT(tlast);
  if (till > since) N_IOV = till;
  std::cout << ">> Imon array --> size: " << imonarray.size() << " >> done." << std::endl;
  delete queryI;
  session->transaction().commit();
  delete session;*/
  return imonrunarray;
  //  return imonarlumiray;
}



//------------------------------------------------------- V M O N ---------------------------------------------------
std::vector<RPCObVmon::V_Item> RPCFwPerRun::createVMON(long long since, long long till)
{
  tMIN = UTtoT(since);
  std::cout <<">> Processing since: "<<tMIN.day()<<"/"<<tMIN.month()<<"/"<<tMIN.year()<<" "<<tMIN.hour()<<":"<<tMIN.minute()<<"."<<tMIN.second()<< std::endl;

  coral::ISession* session = this->connect( m_connectionString,
                                            m_userName, m_password );
  session->transaction().start( true );
  coral::ISchema& schema = session->nominalSchema();
  int nRows = 0;
  coral::IQuery* queryV = schema.newQuery();
  queryV->addToTableList( "FWCAENCHANNEL" );
  queryV->addToOutputList( "FWCAENCHANNEL.DPID", "DPID" );
  queryV->addToOutputList( "FWCAENCHANNEL.CHANGE_DATE", "TSTAMP" );
  queryV->addToOutputList( "FWCAENCHANNEL.ACTUAL_VMON", "VMON" );
  RPCObVmon::V_Item Vtemp;
  std::vector<RPCObVmon::V_Item> vmonarray;
  coral::TimeStamp tlast = tMIN;
  if (till > since) {
    tMAX = UTtoT(till);
    std::cout <<">> Processing till: "<<tMAX.day()<<"/"<<tMAX.month()<<"/"<<tMAX.year()<<" "<<tMAX.hour()<<":"<<tMAX.minute()<<"."<<tMAX.second()<< std::endl;
    std::cout << ">> creating VMON object..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string condition = " NOT FWCAENCHANNEL.ACTUAL_VMON IS NULL AND FWCAENCHANNEL.CHANGE_DATE >:tmin AND FWCAENCHANNEL.CHANGE_DATE <:tmax";
    queryV->setCondition( condition , conditionData );
    coral::ICursor& cursorV = queryV->execute();
    while ( cursorV.next() ) {
      const coral::AttributeList& row = cursorV.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float val = row["VMON"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      if (isMajor(ts, tlast)) tlast = ts;
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Vtemp.dpid = id;
      Vtemp.value = val;
      Vtemp.day = ndate;
      Vtemp.time = ntime;
      vmonarray.push_back(Vtemp);
      ++nRows;
    }
  } else {
    std::cout << ">> creating VMON object..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string condition = " NOT FWCAENCHANNEL.ACTUAL_VMON IS NULL AND FWCAENCHANNEL.CHANGE_DATE >:tmin";
    queryV->setCondition( condition , conditionData );
    coral::ICursor& cursorV = queryV->execute();
    while ( cursorV.next() ) {
      const coral::AttributeList& row = cursorV.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float val = row["VMON"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      if (isMajor(ts, tlast)) tlast = ts;
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Vtemp.dpid = id;
      Vtemp.value = val;
      Vtemp.day = ndate;
      Vtemp.time = ntime;
      vmonarray.push_back(Vtemp);
      ++nRows;
    }
  }

  std::cout << ">> Vmon array --> size: " << vmonarray.size() << " >> done." << std::endl;
  delete queryV;
  session->transaction().commit();
  delete session;
  return vmonarray;
}


//------------------------------ S T A T U S ---------------------------------------------------------------------
std::vector<RPCObStatus::S_Item> RPCFwPerRun::createSTATUS(long long since, long long till)
{
  tMIN = UTtoT(since);
  std::cout <<">> Processing since: "<<tMIN.day()<<"/"<<tMIN.month()<<"/"<<tMIN.year()<<" "<<tMIN.hour()<<":"<<tMIN.minute()<<"."<<tMIN.second()<< std::endl;
  
  coral::ISession* session = this->connect( m_connectionString,
                                            m_userName, m_password );
  session->transaction().start( true );
  coral::ISchema& schema = session->nominalSchema();
  int nRows = 0;
  std::cout << ">> creating STATUS object..." << std::endl;
  coral::IQuery* queryS = schema.newQuery();
  queryS->addToTableList( "FWCAENCHANNEL" );
  queryS->addToOutputList( "FWCAENCHANNEL.DPID", "DPID" );
  queryS->addToOutputList( "FWCAENCHANNEL.CHANGE_DATE", "TSTAMP" );
  queryS->addToOutputList( "FWCAENCHANNEL.ACTUAL_STATUS", "STATUS" );
  RPCObStatus::S_Item Stemp;
  std::vector<RPCObStatus::S_Item> statusarray;
  coral::TimeStamp tlast = tMIN;
  if (till > since) {
    tMAX = UTtoT(till);
    std::cout <<">> Processing till: "<<tMAX.day()<<"/"<<tMAX.month()<<"/"<<tMAX.year()<<" "<<tMAX.hour()<<":"<<tMAX.minute()<<"."<<tMAX.second()<< std::endl;
    std::cout << ">> creating STATUS object..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string condition = " NOT FWCAENCHANNEL.ACTUAL_STATUS IS NULL AND FWCAENCHANNEL.CHANGE_DATE >:tmin AND FWCAENCHANNEL.CHANGE_DATE <:tmax";
    queryS->setCondition( condition , conditionData );
    coral::ICursor& cursorS = queryS->execute();
    while ( cursorS.next() ) {
      const coral::AttributeList& row = cursorS.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float val = row["STATUS"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Stemp.dpid = id;
      Stemp.value = val;
      Stemp.day = ndate;
      Stemp.time = ntime;
      statusarray.push_back(Stemp);
      ++nRows;
    } 
  }else {
    std::cout << ">> creating STATUS object..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string condition = " NOT FWCAENCHANNEL.ACTUAL_STATUS IS NULL AND FWCAENCHANNEL.CHANGE_DATE >:tmin";
    queryS->setCondition( condition , conditionData );
    coral::ICursor& cursorS = queryS->execute();
    while ( cursorS.next() ) {
      const coral::AttributeList& row = cursorS.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float val = row["STATUS"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Stemp.dpid = id;
      Stemp.value = val;
      Stemp.day = ndate;
      Stemp.time = ntime;
      statusarray.push_back(Stemp);
      ++nRows;
    }
  }
  std::cout << ">> Staus array --> size: " << statusarray.size() << " >> done." << std::endl << std::endl << std::endl;
  
  delete queryS;
  session->transaction().commit();
  delete session;
  
  return statusarray;
  
}



//------------------------------ G A S ---------------------------------------------------------------------
std::vector<RPCObGas::Item> RPCFwPerRun::createGAS(long long since, long long till)
{
  tMIN = UTtoT(since);
  std::cout <<">> Processing since: "<<tMIN.day()<<"/"<<tMIN.month()<<"/"<<tMIN.year()<<" "<<tMIN.hour()<<":"<<tMIN.minute()<<"."<<tMIN.second()<< std::endl;

  coral::ISession* session = this->connect( m_connectionString,
                                            m_userName, m_password );
  session->transaction().start( true );
  coral::ISchema& schema = session->nominalSchema();
  
  coral::TimeStamp tlast = tMIN;  

  // FLOWIN
  coral::IQuery* querySIN = schema.newQuery();
  querySIN->addToTableList( "RPCGASCHANNEL" );
  querySIN->addToOutputList( "RPCGASCHANNEL.DPID", "DPID" );
  querySIN->addToOutputList( "RPCGASCHANNEL.CHANGE_DATE", "TSTAMP" );
  querySIN->addToOutputList( "RPCGASCHANNEL.FLOWIN", "FLOWIN" );

  // FLOWOUT
  coral::IQuery* querySOUT = schema.newQuery();
  querySOUT->addToTableList( "RPCGASCHANNEL" );
  querySOUT->addToOutputList( "RPCGASCHANNEL.DPID", "DPID" );
  querySOUT->addToOutputList( "RPCGASCHANNEL.CHANGE_DATE", "TSTAMP" );
  querySOUT->addToOutputList( "RPCGASCHANNEL.FLOWOUT", "FLOWOUT" );


  RPCObGas::Item gastemp;
  std::vector<RPCObGas::Item> gasarray;

  if (till > since) {
    tMAX = UTtoT(till);
    std::cout <<">> Processing till: "<<tMAX.day()<<"/"<<tMAX.month()<<"/"<<tMAX.year()<<" "<<tMAX.hour()<<":"<<tMAX.minute()<<"."<<tMAX.second()<< std::endl;
    std::cout << ">> creating GAS object..." << std::endl;
    std::cout << ">> processing FLOWIN..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string conditionIN = "RPCGASCHANNEL.FLOWIN IS NOT NULL AND CHANGE_DATE >:tmin AND CHANGE_DATE <:tmax";
    
    querySIN->setCondition( conditionIN, conditionData );
    coral::ICursor& cursorSIN = querySIN->execute();
    while ( cursorSIN.next() ) {
      gastemp.dpid=0;gastemp.flowin=0;gastemp.flowout=0;gastemp.day=0;gastemp.time=0;
      const coral::AttributeList& row = cursorSIN.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float val = row["FLOWIN"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      gastemp.dpid = id;
      gastemp.flowin = val;
      gastemp.day = ndate;
      gastemp.time = ntime;
      gasarray.push_back(gastemp);
    }
  } else {
    std::cout << ">> creating GAS object..." << std::endl;
    std::cout << ">> processing FLOWIN..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string conditionIN = "RPCGASCHANNEL.FLOWIN IS NOT NULL AND CHANGE_DATE >:tmin";
    std::cout << "processing FLOWIN..." << std::endl;
    querySIN->setCondition( conditionIN, conditionData );
    coral::ICursor& cursorSIN = querySIN->execute();
    while ( cursorSIN.next() ) {
      gastemp.dpid=0;gastemp.flowin=0;gastemp.flowout=0;gastemp.day=0;gastemp.time=0;
      const coral::AttributeList& row = cursorSIN.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float val = row["FLOWIN"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      gastemp.dpid = id;
      gastemp.flowin = val;
      gastemp.day = ndate;
      gastemp.time = ntime;
      gasarray.push_back(gastemp);
    }
  }


    if (till > since) {
    tMAX = UTtoT(till);
    //    std::cout <<">> Processing till: "<<tMAX.day()<<"/"<<tMAX.month()<<"/"<<tMAX.year()<<" "<<tMAX.hour()<<":"<<tMAX.minute()<<"."<<tMAX.second()<< std::endl;
    std::cout << ">> processing FLOWOUT..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string conditionOUT = "RPCGASCHANNEL.FLOWOUT IS NOT NULL AND CHANGE_DATE >:tmin AND CHANGE_DATE <:tmax";
  
    querySOUT->setCondition( conditionOUT, conditionData );
    coral::ICursor& cursorSOUT = querySOUT->execute();
    while ( cursorSOUT.next() ) {
      gastemp.dpid=0;gastemp.flowin=0;gastemp.flowout=0;gastemp.day=0;gastemp.time=0;
      const coral::AttributeList& row = cursorSOUT.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float val = row["FLOWOUT"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      gastemp.dpid = id;
      gastemp.flowout = val;
      gastemp.day = ndate;
      gastemp.time = ntime;
      gasarray.push_back(gastemp);
    } 
    } else {
      std::cout << ">> processing FLOWOUT..." << std::endl;
      coral::AttributeList conditionData;
      conditionData.extend<coral::TimeStamp>( "tmin" );
      conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
      std::string conditionOUT = "RPCGASCHANNEL.FLOWOUT IS NOT NULL AND CHANGE_DATE >:tmin";
      querySOUT->setCondition( conditionOUT, conditionData );
      coral::ICursor& cursorSOUT = querySOUT->execute();
      while ( cursorSOUT.next() ) {
	gastemp.dpid=0;gastemp.flowin=0;gastemp.flowout=0;gastemp.day=0;gastemp.time=0;
	const coral::AttributeList& row = cursorSOUT.currentRow();
	float idoub = row["DPID"].data<float>();
	int id = static_cast<int>(idoub);
	float val = row["FLOWOUT"].data<float>();
	coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
	int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
	int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
	gastemp.dpid = id;
	gastemp.flowout = val;
	gastemp.day = ndate;
	gastemp.time = ntime;
	gasarray.push_back(gastemp);
      }
    }
    
    
    N_IOV = TtoUT(tlast);
    if (till > since) N_IOV = till;
    std::cout << ">> Gas array --> size: " << gasarray.size() << " >> done." << std::endl << std::endl << std::endl;
    
    delete querySIN;
    delete querySOUT;
    session->transaction().commit();
    delete session;
    
    return gasarray;
    
}



//------------------------------ T E M P E R A T U R E ---------------------------------------------------------------------
std::vector<RPCObTemp::T_Item> RPCFwPerRun::createT(long long since, long long till)
{
  tMIN = UTtoT(since);
  std::cout <<">> Processing since: "<<tMIN.day()<<"/"<<tMIN.month()<<"/"<<tMIN.year()<<" "<<tMIN.hour()<<":"<<tMIN.minute()<<"."<<tMIN.second()<< std::endl;

  coral::ISession* session = this->connect( m_connectionString,
                                            m_userName, m_password );
  session->transaction().start( true );
  coral::ISchema& schema = session->nominalSchema();
  int nRows = 0;
  coral::IQuery* queryS = schema.newQuery();
  queryS->addToTableList( "FWCAENCHANNELADC" );
  queryS->addToOutputList( "FWCAENCHANNELADC.DPID", "DPID" );
  queryS->addToOutputList( "FWCAENCHANNELADC.CHANGE_DATE", "TSTAMP" );
  queryS->addToOutputList( "FWCAENCHANNELADC.ACTUAL_TEMPERATURE", "TEMPERATURE" );
  RPCObTemp::T_Item Ttemp;
  std::vector<RPCObTemp::T_Item> temparray;
  coral::TimeStamp tlast = tMIN;
  if (till > since) {
    tMAX = UTtoT(till);
    std::cout <<">> Processing till: "<<tMAX.day()<<"/"<<tMAX.month()<<"/"<<tMAX.year()<<" "<<tMAX.hour()<<":"<<tMAX.minute()<<"."<<tMAX.second()<< std::endl;
    std::cout << ">> creating TEMPERATURE object..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string condition = "FWCAENCHANNELADC.ACTUAL_TEMPERATURE IS NOT NULL AND CHANGE_DATE >:tmin AND CHANGE_DATE <:tmax";
    queryS->setCondition( condition , conditionData );
    coral::ICursor& cursorS = queryS->execute();
    while ( cursorS.next() ) {
      const coral::AttributeList& row = cursorS.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float val = row["TEMPERATURE"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      
      Ttemp.dpid = id;
      Ttemp.value = val;
      Ttemp.day = ndate;
      Ttemp.time = ntime;
      temparray.push_back(Ttemp);
      
      ++nRows;
    }
  } else {
    std::cout << ">> creating TEMPERATURE object..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string condition = "FWCAENCHANNELADC.ACTUAL_TEMPERATURE IS NOT NULL AND CHANGE_DATE >:tmin";
    queryS->setCondition( condition , conditionData );
    coral::ICursor& cursorS = queryS->execute();
    while ( cursorS.next() ) {
      const coral::AttributeList& row = cursorS.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float val = row["TEMPERATURE"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      
      Ttemp.dpid = id;
      Ttemp.value = val;
      Ttemp.day = ndate;
      Ttemp.time = ntime;
      temparray.push_back(Ttemp);
      
      ++nRows;
    }
  }
  N_IOV = TtoUT(tlast);
  if (till > since) N_IOV = till;
  std::cout << ">> Temperature array --> size: " << temparray.size() << " >> done." << std::endl << std::endl << std::endl;

  delete queryS;
  session->transaction().commit();
  delete session;

  return temparray;

}


//----------------------------- I D   M A P ------------------------------------------------------------------------
std::vector<RPCObPVSSmap::Item> RPCFwPerRun::createIDMAP()
{
  //  float tMINi = 0;
  std::cout <<">> Processing data..." << std::endl;
  
  coral::ISession* session = this->connect( m_connectionString,
                                            m_userName, m_password );
  session->transaction().start( true );
  coral::ISchema& schema = session->nominalSchema();
  int nRows = 0;
  std::cout << ">> creating IDMAP object..." << std::endl;
  coral::IQuery* queryM = schema.newQuery();
  queryM->addToTableList( "RPCPVSSDETID");
  queryM->addToOutputList( "RPCPVSSDETID.SINCE", "SINCE" );
  queryM->addToOutputList( "RPCPVSSDETID.PVSS_ID", "PVSS_ID" );
  queryM->addToOutputList( "RPCPVSSDETID.REGION", "REGION" );
  queryM->addToOutputList( "RPCPVSSDETID.RING", "RING" );
  queryM->addToOutputList( "RPCPVSSDETID.STATION", "STATION" );
  queryM->addToOutputList( "RPCPVSSDETID.SECTOR", "SECTOR" );
  queryM->addToOutputList( "RPCPVSSDETID.LAYER", "LAYER" );
  queryM->addToOutputList( "RPCPVSSDETID.SUBSECTOR", "SUBSECTOR" );
  queryM->addToOutputList( "RPCPVSSDETID.SUPPLYTYPE", "SUPPLYTYPE" );

  std::string condM = "RPCPVSSDETID.PVSS_ID is not NULL";

  //  queryM->setCondition(condM);
  coral::ICursor& cursorM = queryM->execute();

  RPCObPVSSmap::Item Itemp;
  std::vector<RPCObPVSSmap::Item> idmaparray;
  while ( cursorM.next() ) {
    const coral::AttributeList& row = cursorM.currentRow();
    int id = row["PVSS_ID"].data<int>();
    std::string reg_s = row["REGION"].data<std::string>();
    std::string rin_s = row["RING"].data<std::string>();
    std::string sta_s = row["STATION"].data<std::string>();
    std::string sec_s = row["SECTOR"].data<std::string>();
    std::string lay_s = row["LAYER"].data<std::string>();
    std::string sub_s = row["SUBSECTOR"].data<std::string>();
    std::string sup_s = row["SUPPLYTYPE"].data<std::string>();

    int reg = atoi(reg_s.c_str()); 
    int rin = atoi(rin_s.c_str()); 
    int sta = atoi(sta_s.c_str()); 
    int sec = atoi(sec_s.c_str()); 
    int lay = atoi(lay_s.c_str()); 
    int sub = atoi(sub_s.c_str());
    int sup = 5;

    if (sup_s == "HV")  sup = 0;
    if (sup_s == "LVA") sup = 1;
    if (sup_s == "LVD") sup = 2;
    if (sup_s == "LB")  sup = 3;
    if (sup_s == "T")   sup = 4;


    coral::TimeStamp ts =  row["SINCE"].data<coral::TimeStamp>();

    std::ostringstream dday;
    std::ostringstream dmon;
    std::ostringstream dyea;
    dday << ts.day();
    dmon << ts.month();
    dyea << ts.year();
    std::string date = dday.str() + "-" + dmon.str() + "-" + dyea.str();

    Itemp.since = atoi(date.c_str());
    Itemp.dpid = id;
    Itemp.region = reg;
    Itemp.ring = rin;
    Itemp.station = sta;
    Itemp.sector = sec;
    Itemp.layer = lay;
    Itemp.subsector = sub;
    Itemp.suptype = sup;

    idmaparray.push_back(Itemp);

    ++nRows;
  }


  std::cout << ">> IDMAP array --> size: " << idmaparray.size() << " >> done." << std::endl;
  delete queryM;
  session->transaction().commit();
  delete session;
  return idmaparray;
}




//----------------------------- F E B ------------------------------------------------------------------------
std::vector<RPCObFebmap::Feb_Item> RPCFwPerRun::createFEB(long long since, long long till)
{
  tMIN = UTtoT(since);
  std::cout <<">> Processing since: "<<tMIN.day()<<"/"<<tMIN.month()<<"/"<<tMIN.year()<<" "<<tMIN.hour()<<":"<<tMIN.minute()<<"."<<tMIN.second()<< std::endl;

  coral::ISession* session = this->connect( m_connectionString,
                                            m_userName, m_password );
  session->transaction().start( true );
  coral::ISchema& schema = session->nominalSchema();
  std::cout << ">> creating FEB object..." << std::endl;

  // VTH1
  coral::IQuery* queryFVTH1 = schema.newQuery();
  queryFVTH1->addToTableList( "RPCFEB");
  queryFVTH1->addToOutputList( "RPCFEB.DPID", "DPID" );
  queryFVTH1->addToOutputList( "RPCFEB.CHANGE_DATE", "TSTAMP" );
  queryFVTH1->addToOutputList( "RPCFEB.VTH1", "VTH1" );

  // VTH2
  coral::IQuery* queryFVTH2 = schema.newQuery();
  queryFVTH2->addToTableList( "RPCFEB");
  queryFVTH2->addToOutputList( "RPCFEB.DPID", "DPID" );
  queryFVTH2->addToOutputList( "RPCFEB.CHANGE_DATE", "TSTAMP" );
  queryFVTH2->addToOutputList( "RPCFEB.VTH2", "VTH2" );

  // VTH3
  coral::IQuery* queryFVTH3 = schema.newQuery();
  queryFVTH3->addToTableList( "RPCFEB");
  queryFVTH3->addToOutputList( "RPCFEB.DPID", "DPID" );
  queryFVTH3->addToOutputList( "RPCFEB.CHANGE_DATE", "TSTAMP" );
  queryFVTH3->addToOutputList( "RPCFEB.VTH3", "VTH3" );

  // VTH4
  coral::IQuery* queryFVTH4 = schema.newQuery();
  queryFVTH4->addToTableList( "RPCFEB");
  queryFVTH4->addToOutputList( "RPCFEB.DPID", "DPID" );
  queryFVTH4->addToOutputList( "RPCFEB.CHANGE_DATE", "TSTAMP" );
  queryFVTH4->addToOutputList( "RPCFEB.VTH4", "VTH4" );

  // VMON1
  coral::IQuery* queryFVMON1 = schema.newQuery();
  queryFVMON1->addToTableList( "RPCFEB");
  queryFVMON1->addToOutputList( "RPCFEB.DPID", "DPID" );
  queryFVMON1->addToOutputList( "RPCFEB.CHANGE_DATE", "TSTAMP" );
  queryFVMON1->addToOutputList( "RPCFEB.VMON1", "VMON1" );

  // VMON2
  coral::IQuery* queryFVMON2 = schema.newQuery();
  queryFVMON2->addToTableList( "RPCFEB");
  queryFVMON2->addToOutputList( "RPCFEB.DPID", "DPID" );
  queryFVMON2->addToOutputList( "RPCFEB.CHANGE_DATE", "TSTAMP" );
  queryFVMON2->addToOutputList( "RPCFEB.VMON2", "VMON2" );

  // VMON3
  coral::IQuery* queryFVMON3 = schema.newQuery();
  queryFVMON3->addToTableList( "RPCFEB");
  queryFVMON3->addToOutputList( "RPCFEB.DPID", "DPID" );
  queryFVMON3->addToOutputList( "RPCFEB.CHANGE_DATE", "TSTAMP" );
  queryFVMON3->addToOutputList( "RPCFEB.VMON3", "VMON3" );

  // VMON4
  coral::IQuery* queryFVMON4 = schema.newQuery();
  queryFVMON4->addToTableList( "RPCFEB");
  queryFVMON4->addToOutputList( "RPCFEB.DPID", "DPID" );
  queryFVMON4->addToOutputList( "RPCFEB.CHANGE_DATE", "TSTAMP" );
  queryFVMON4->addToOutputList( "RPCFEB.VMON4", "VMON4" );

  // TEMP1
  coral::IQuery* queryFTEMP1 = schema.newQuery();
  queryFTEMP1->addToTableList( "RPCFEB");
  queryFTEMP1->addToOutputList( "RPCFEB.DPID", "DPID" );
  queryFTEMP1->addToOutputList( "RPCFEB.CHANGE_DATE", "TSTAMP" );
  queryFTEMP1->addToOutputList( "RPCFEB.TEMPERATURE1", "TEMP1" );

  // TEMP2
  coral::IQuery* queryFTEMP2 = schema.newQuery();
  queryFTEMP2->addToTableList( "RPCFEB");
  queryFTEMP2->addToOutputList( "RPCFEB.DPID", "DPID" );
  queryFTEMP2->addToOutputList( "RPCFEB.CHANGE_DATE", "TSTAMP" );
  queryFTEMP2->addToOutputList( "RPCFEB.TEMPERATURE2", "TEMP2" );

  RPCObFebmap::Feb_Item Itemp;
  std::vector<RPCObFebmap::Feb_Item> febarray;
  coral::TimeStamp tlast = tMIN;
  if (till > since) {
    tMAX = UTtoT(till);
    std::cout <<">> Processing till: "<<tMAX.day()<<"/"<<tMAX.month()<<"/"<<tMAX.year()<<" "<<tMAX.hour()<<":"<<tMAX.minute()<<"."<<tMAX.second()<< std::endl;
    std::cout << "Processing VTH1..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string conditionVTH1 = "RPCFEB.VTH1 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin AND RPCFEB.CHANGE_DATE <:tmax";
    queryFVTH1->setCondition( conditionVTH1, conditionData );
    coral::ICursor& cursorFVTH1 = queryFVTH1->execute();
    while ( cursorFVTH1.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVTH1.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vth1 = row["VTH1"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.thr1 = vth1;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    } 
  }else {
    std::cout << ">> Processing VTH1..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string conditionVTH1 = "RPCFEB.VTH1 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin";
    queryFVTH1->setCondition( conditionVTH1, conditionData );
    coral::ICursor& cursorFVTH1 = queryFVTH1->execute();
    while ( cursorFVTH1.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVTH1.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vth1 = row["VTH1"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.thr1 = vth1;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  }
  
  if (till > since) {
    tMAX = UTtoT(till);
    std::cout << "Processing VTH2..." << std::endl;
     coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string conditionVTH2 = "RPCFEB.VTH2 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin AND RPCFEB.CHANGE_DATE <:tmax";
    queryFVTH2->setCondition( conditionVTH2, conditionData );
    coral::ICursor& cursorFVTH2 = queryFVTH2->execute();
    while ( cursorFVTH2.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVTH2.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vth2 = row["VTH2"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.thr2 = vth2;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }  
  } else {
    std::cout << ">> Processing VTH2..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string conditionVTH2 = "RPCFEB.VTH2 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin";
    queryFVTH2->setCondition( conditionVTH2, conditionData );
    coral::ICursor& cursorFVTH2 = queryFVTH2->execute();
    while ( cursorFVTH2.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVTH2.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vth2 = row["VTH2"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.thr2 = vth2;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  }
    
  
  if (till > since) {
    tMAX = UTtoT(till);
    std::cout << "Processing VTH3..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string conditionVTH3 = "RPCFEB.VTH3 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin AND RPCFEB.CHANGE_DATE <:tmax";
    queryFVTH3->setCondition( conditionVTH3, conditionData );
    coral::ICursor& cursorFVTH3 = queryFVTH3->execute();
    while ( cursorFVTH3.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVTH3.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vth3 = row["VTH3"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.thr3 = vth3;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  } else {
    std::cout << ">> Processing VTH3..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string conditionVTH3 = "RPCFEB.VTH3 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin";
    queryFVTH3->setCondition( conditionVTH3, conditionData );
    coral::ICursor& cursorFVTH3 = queryFVTH3->execute();
    while ( cursorFVTH3.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVTH3.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vth3 = row["VTH3"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.thr3 = vth3;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  }
  

  if (till > since) {
    tMAX = UTtoT(till);
    std::cout << "Processing VTH4..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string conditionVTH4 = "RPCFEB.VTH4 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin AND RPCFEB.CHANGE_DATE <:tmax";
    queryFVTH4->setCondition( conditionVTH4, conditionData );
    coral::ICursor& cursorFVTH4 = queryFVTH4->execute();
    while ( cursorFVTH4.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVTH4.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vth4 = row["VTH4"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.thr4 = vth4;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  } else {
    std::cout << ">> Processing VTH4..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string conditionVTH4 = "RPCFEB.VTH4 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin";
    queryFVTH4->setCondition( conditionVTH4, conditionData );
    coral::ICursor& cursorFVTH4 = queryFVTH4->execute();
    while ( cursorFVTH4.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVTH4.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vth4 = row["VTH4"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.thr4 = vth4;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  }
  
  if (till > since) {
    tMAX = UTtoT(till);
    std::cout << "Processing VMON1..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string conditionVMON1 = "RPCFEB.VMON1 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin AND RPCFEB.CHANGE_DATE <:tmax";
    queryFVMON1->setCondition( conditionVMON1, conditionData );
    coral::ICursor& cursorFVMON1 = queryFVMON1->execute();
    while ( cursorFVMON1.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVMON1.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vmon1 = row["VMON1"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.vmon1 = vmon1;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  } else {
    std::cout << ">> Processing VMON1..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string conditionVMON1 = "RPCFEB.VMON1 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin";
    queryFVMON1->setCondition( conditionVMON1, conditionData );
    coral::ICursor& cursorFVMON1 = queryFVMON1->execute();
    while ( cursorFVMON1.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVMON1.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vmon1 = row["VMON1"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.vmon1 = vmon1;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  }
  
  
  
  if (till > since) {
    tMAX = UTtoT(till);
    std::cout << "Processing VMON2..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string conditionVMON2 = "RPCFEB.VMON2 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin AND RPCFEB.CHANGE_DATE <:tmax";
    queryFVMON2->setCondition( conditionVMON2, conditionData );
    coral::ICursor& cursorFVMON2 = queryFVMON2->execute();
    while ( cursorFVMON2.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVMON2.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vmon2 = row["VMON2"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.vmon2 = vmon2;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  } else {
    std::cout << ">> Processing VMON2..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string conditionVMON2 = "RPCFEB.VMON2 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin";
    queryFVMON2->setCondition( conditionVMON2, conditionData );
    coral::ICursor& cursorFVMON2 = queryFVMON2->execute();
    while ( cursorFVMON2.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVMON2.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vmon2 = row["VMON2"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.vmon2 = vmon2;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  }


  if (till > since) {
    tMAX = UTtoT(till);
    std::cout << "Processing VMON3..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string conditionVMON3 = "RPCFEB.VMON3 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin AND RPCFEB.CHANGE_DATE <:tmax";
    queryFVMON3->setCondition( conditionVMON3, conditionData );
    coral::ICursor& cursorFVMON3 = queryFVMON3->execute();
    while ( cursorFVMON3.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVMON3.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vmon3 = row["VMON3"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.vmon3 = vmon3;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  } else {
    std::cout << ">> Processing VMON3..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string conditionVMON3 = "RPCFEB.VMON3 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin";
    queryFVMON3->setCondition( conditionVMON3, conditionData );
    coral::ICursor& cursorFVMON3 = queryFVMON3->execute();
    while ( cursorFVMON3.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVMON3.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vmon3 = row["VMON3"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.vmon3 = vmon3;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  }
  

  if (till > since) {
    tMAX = UTtoT(till);
    std::cout << "Processing VMON4..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string conditionVMON4 = "RPCFEB.VMON4 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin AND RPCFEB.CHANGE_DATE <:tmax";
    queryFVMON4->setCondition( conditionVMON4, conditionData );
    coral::ICursor& cursorFVMON4 = queryFVMON4->execute();
    while ( cursorFVMON4.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVMON4.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vmon4 = row["VMON4"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.vmon4 = vmon4;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  } else {
    std::cout << ">> Processing VMON4..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string conditionVMON4 = "RPCFEB.VMON4 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin";
    queryFVMON4->setCondition( conditionVMON4, conditionData );
    coral::ICursor& cursorFVMON4 = queryFVMON4->execute();
    while ( cursorFVMON4.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFVMON4.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float vmon4 = row["VMON4"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.vmon4 = vmon4;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  }
  
  
  if (till > since) {
    tMAX = UTtoT(till);
    std::cout << "Processing TEMP1..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string conditionTEMP1 = "RPCFEB.TEMPERATURE1 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin AND RPCFEB.CHANGE_DATE <:tmax";
    queryFTEMP1->setCondition( conditionTEMP1, conditionData );
    coral::ICursor& cursorFTEMP1 = queryFTEMP1->execute();
    while ( cursorFTEMP1.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFTEMP1.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float temp1 = row["TEMP1"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.temp1 = temp1;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  } else {
    std::cout << ">> Processing TEMP1..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string conditionTEMP1 = "RPCFEB.TEMPERATURE1 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin";
    queryFTEMP1->setCondition( conditionTEMP1, conditionData );
    coral::ICursor& cursorFTEMP1 = queryFTEMP1->execute();
    while ( cursorFTEMP1.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFTEMP1.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float temp1 = row["TEMP1"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.temp1 = temp1;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  }


  if (till > since) {
    tMAX = UTtoT(till);
    std::cout << "Processing TEMP2..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string conditionTEMP2 = "RPCFEB.TEMPERATURE2 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin AND RPCFEB.CHANGE_DATE <:tmax";
    queryFTEMP2->setCondition( conditionTEMP2, conditionData );
    coral::ICursor& cursorFTEMP2 = queryFTEMP2->execute();
    while ( cursorFTEMP2.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFTEMP2.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float temp2 = row["TEMP2"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.temp2 = temp2;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  } else {
    std::cout << ">> Processing TEMP2..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    std::string conditionTEMP2 = "RPCFEB.TEMPERATURE2 IS NOT NULL AND RPCFEB.CHANGE_DATE >:tmin";
    queryFTEMP2->setCondition( conditionTEMP2, conditionData );
    coral::ICursor& cursorFTEMP2 = queryFTEMP2->execute();
        while ( cursorFTEMP2.next() ) {
      Itemp.thr1=0;Itemp.thr2=0;Itemp.thr3=0;Itemp.thr4=0;Itemp.vmon1=0;Itemp.vmon2=0;Itemp.vmon3=0;
      Itemp.vmon4=0;Itemp.temp1=0;Itemp.temp2=0;Itemp.noise1=0;Itemp.noise2=0;Itemp.noise3=0;Itemp.noise4=0;
      const coral::AttributeList& row = cursorFTEMP2.currentRow();
      float idoub = row["DPID"].data<float>();
      int id = static_cast<int>(idoub);
      float temp2 = row["TEMP2"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      int ndate = (ts.day() * 10000) + (ts.month() * 100) + (ts.year()-2000);
      int ntime = (ts.hour() * 10000) + (ts.minute() * 100) + ts.second();
      Itemp.dpid = id;
      Itemp.temp2 = temp2;
      Itemp.day = ndate;
      Itemp.time = ntime;
      febarray.push_back(Itemp);
    }
  }

  std::cout << ">> FEB array --> size: " << febarray.size() << " >> done." << std::endl;
  delete queryFVTH1;
  delete queryFVTH2;
  delete queryFVTH3;
  delete queryFVTH4;
  delete queryFTEMP1;
  session->transaction().commit();
  delete session;
  return febarray;
}



//----------------------------- U X C ------------------------------------------------------------------------
std::vector<RPCObUXC::Item> RPCFwPerRun::createUXC(long long since, long long till)
{
  tMIN = UTtoT(since);
  std::cout <<">> Processing since: "<<tMIN.day()<<"/"<<tMIN.month()<<"/"<<tMIN.year()<<" "<<tMIN.hour()<<":"<<tMIN.minute()<<"."<<tMIN.second()<< std::endl;
    
  coral::ISession* session = this->connect( m_connectionString,
                                            m_userName, m_password );
  session->transaction().start( true );  
  coral::ISchema& schema = session->nominalSchema();
  std::cout << ">> creating UXC object..." << std::endl;
  
  // UXCT
  coral::IQuery* queryUXCP = schema.newQuery();
  queryUXCP->addToTableList( "RPCGASPARAMETERS");
  queryUXCP->addToTableList( "DP_NAME2ID" );
  queryUXCP->addToOutputList( "DP_NAME2ID.DPNAME", "DPNAME" );
  queryUXCP->addToOutputList("DP_NAME2ID.ID","ID");
  queryUXCP->addToOutputList( "RPCGASPARAMETERS.DPID", "DPID" );
  queryUXCP->addToOutputList( "RPCGASPARAMETERS.CHANGE_DATE", "TSTAMP" );
  queryUXCP->addToOutputList( "RPCGASPARAMETERS.VALUE", "VALUE" );
  coral::IQuery* queryUXCT = schema.newQuery();
  queryUXCT->addToTableList( "RPCCOOLING");
  queryUXCT->addToTableList( "DP_NAME2ID" );
  queryUXCT->addToOutputList( "DP_NAME2ID.DPNAME", "DPNAME" );
  queryUXCT->addToOutputList("DP_NAME2ID.ID","ID");
  queryUXCT->addToOutputList( "RPCCOOLING.DPID", "DPID" );
  queryUXCT->addToOutputList( "RPCCOOLING.CHANGE_DATE", "TSTAMP" );
  queryUXCT->addToOutputList( "RPCCOOLING.VALUE", "VALUE" );
  coral::IQuery* queryUXCH = schema.newQuery();
  queryUXCH->addToTableList( "RPCCOOLING");
  queryUXCH->addToTableList( "DP_NAME2ID" );
  queryUXCH->addToOutputList( "DP_NAME2ID.DPNAME", "DPNAME" );
  queryUXCH->addToOutputList("DP_NAME2ID.ID","ID");
  queryUXCH->addToOutputList( "RPCCOOLING.DPID", "DPID" );
  queryUXCH->addToOutputList( "RPCCOOLING.CHANGE_DATE", "TSTAMP" );
  queryUXCH->addToOutputList( "RPCCOOLING.VALUE", "VALUE" );


  RPCObUXC::Item Itemp;
  std::vector<RPCObUXC::Item> uxcarray;
  coral::TimeStamp tlast = tMIN;
  if (till > since) {
    tMAX = UTtoT(till);
    std::cout <<">> Processing till: "<<tMAX.day()<<"/"<<tMAX.month()<<"/"<<tMAX.year()<<" "<<tMAX.hour()<<":"<<tMAX.minute()<<"."<<tMAX.second()<< std::endl;
    std::cout << "Processing UXC..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );   
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string conditionUXCP = "RPCGASPARAMETERS.DPID = DP_NAME2ID.ID AND RPCGASPARAMETERS.CHANGE_DATE >:tmin AND RPCGASPARAMETERS.CHANGE_DATE <:tmax AND (DP_NAME2ID.DPNAME like '%UXCPressure%')";
    queryUXCP->setCondition( conditionUXCP, conditionData );
    coral::ICursor& cursorUXCP = queryUXCP->execute();
    while ( cursorUXCP.next() ) {
      Itemp.temperature=0;Itemp.pressure=0;Itemp.dewpoint=0;
      const coral::AttributeList& row = cursorUXCP.currentRow();
      float value = row["VALUE"].data<float>(); 
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      unsigned long long ut_time = TtoUT(ts);
      Itemp.pressure = value;
      Itemp.unixtime = ut_time;
      uxcarray.push_back(Itemp);
    }
    std::string conditionUXCT = "RPCCOOLING.DPID = DP_NAME2ID.ID AND RPCCOOLING.CHANGE_DATE >:tmin AND RPCCOOLING.CHANGE_DATE <:tmax AND (DP_NAME2ID.DPNAME like '%TempUXC%')";
    queryUXCT->setCondition( conditionUXCT, conditionData );
    coral::ICursor& cursorUXCT = queryUXCT->execute();
    while ( cursorUXCT.next() ) {
      Itemp.temperature=0;Itemp.pressure=0;Itemp.dewpoint=0;
      const coral::AttributeList& row = cursorUXCT.currentRow();
      float value = row["VALUE"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      unsigned long long ut_time = TtoUT(ts);
      Itemp.temperature = value;                                                                                                                                                 
      Itemp.unixtime = ut_time;                                                                                                                                                  
      uxcarray.push_back(Itemp);                                                                                                                                                 
      }
    std::string conditionUXCH = "RPCCOOLING.DPID = DP_NAME2ID.ID AND RPCCOOLING.CHANGE_DATE >:tmin AND RPCCOOLING.CHANGE_DATE <:tmax AND (DP_NAME2ID.DPNAME like '%DewpointUXC%')";
    queryUXCH->setCondition( conditionUXCH, conditionData );
    coral::ICursor& cursorUXCH = queryUXCH->execute();
    while ( cursorUXCH.next() ) {
      Itemp.temperature=0;Itemp.pressure=0;Itemp.dewpoint=0;
      const coral::AttributeList& row = cursorUXCH.currentRow();
      float value = row["VALUE"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      unsigned long long ut_time = TtoUT(ts);
      Itemp.dewpoint = value;
      Itemp.unixtime = ut_time;
      uxcarray.push_back(Itemp);
    }
  }else {
  
    std::cout << "This objects cannot be copied in this mode. Use RANGE mode!" << std::endl;
    
  }
  
  
  std::cout << ">> UXC array --> size: " << uxcarray.size() << " >> done." << std::endl;
  delete queryUXCT;
  delete queryUXCP;
  delete queryUXCH;
  session->transaction().commit();
  delete session;
  return uxcarray;

}



//----------------------------- M I X ------------------------------------------------------------------------                                                                     
std::vector<RPCObGasMix::Item> RPCFwPerRun::createMix(long long since, long long till)
{
  tMIN = UTtoT(since);
  std::cout <<">> Processing since: "<<tMIN.day()<<"/"<<tMIN.month()<<"/"<<tMIN.year()<<" "<<tMIN.hour()<<":"<<tMIN.minute()<<"."<<tMIN.second()<< std::endl;

  coral::ISession* session = this->connect( m_connectionString,
                                            m_userName, m_password );
  session->transaction().start( true );
  coral::ISchema& schema = session->nominalSchema();
  std::cout << ">> creating UXC object..." << std::endl;

  coral::IQuery* queryMix = schema.newQuery();
  queryMix->addToTableList( "RPCGASPARAMETERS");
  queryMix->addToTableList( "DP_NAME2ID" );
  queryMix->addToOutputList( "DP_NAME2ID.DPNAME", "DPNAME" );
  queryMix->addToOutputList("DP_NAME2ID.ID","ID");
  queryMix->addToOutputList( "RPCGASPARAMETERS.DPID", "DPID" );
  queryMix->addToOutputList( "RPCGASPARAMETERS.CHANGE_DATE", "TSTAMP" );
  queryMix->addToOutputList( "RPCGASPARAMETERS.VALUE", "VALUE" );

  coral::IQuery* queryMix2 = schema.newQuery();
  queryMix2->addToTableList( "RPCGASPARAMETERS");
  queryMix2->addToTableList( "DP_NAME2ID" );
  queryMix2->addToOutputList( "DP_NAME2ID.DPNAME", "DPNAME" );
  queryMix2->addToOutputList("DP_NAME2ID.ID","ID");
  queryMix2->addToOutputList( "RPCGASPARAMETERS.DPID", "DPID" );
  queryMix2->addToOutputList( "RPCGASPARAMETERS.CHANGE_DATE", "TSTAMP" );
  queryMix2->addToOutputList( "RPCGASPARAMETERS.VALUE", "VALUE" );

  coral::IQuery* queryMix3 = schema.newQuery();
  queryMix3->addToTableList( "RPCGASPARAMETERS");
  queryMix3->addToTableList( "DP_NAME2ID" );
  queryMix3->addToOutputList( "DP_NAME2ID.DPNAME", "DPNAME" );
  queryMix3->addToOutputList("DP_NAME2ID.ID","ID");
  queryMix3->addToOutputList( "RPCGASPARAMETERS.DPID", "DPID" );
  queryMix3->addToOutputList( "RPCGASPARAMETERS.CHANGE_DATE", "TSTAMP" );
  queryMix3->addToOutputList( "RPCGASPARAMETERS.VALUE", "VALUE" );

  RPCObGasMix::Item Mtemp;
  std::vector<RPCObGasMix::Item> marray;
  coral::TimeStamp tlast = tMIN;
  if (till > since) {
    tMAX = UTtoT(till);
    std::cout <<">> Processing till: "<<tMAX.day()<<"/"<<tMAX.month()<<"/"<<tMAX.year()<<" "<<tMAX.hour()<<":"<<tMAX.minute()<<"."<<tMAX.second()<< std::endl;
    std::cout << "Processing UXC..." << std::endl;
    coral::AttributeList conditionData;
    conditionData.extend<coral::TimeStamp>( "tmin" );
    conditionData.extend<coral::TimeStamp>( "tmax" );
    conditionData["tmin"].data<coral::TimeStamp>() = tMIN;
    conditionData["tmax"].data<coral::TimeStamp>() = tMAX;
    std::string conditionM1 = "RPCGASPARAMETERS.DPID = DP_NAME2ID.ID AND RPCGASPARAMETERS.CHANGE_DATE >:tmin AND RPCGASPARAMETERS.CHANGE_DATE <:tmax AND (DP_NAME2ID.DPNAME like '%IC4H10Ratio%')";
    queryMix->setCondition( conditionM1, conditionData );
    coral::ICursor& cursorMix = queryMix->execute();
    while ( cursorMix.next() ) {
      Mtemp.gas1=0;Mtemp.gas2=0;Mtemp.gas3=0;
      const coral::AttributeList& row = cursorMix.currentRow();
      float value = row["VALUE"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      unsigned long long ut_time = TtoUT(ts);
      Mtemp.gas1 = value;
      Mtemp.unixtime = ut_time;
      marray.push_back(Mtemp);
    }
    std::string conditionM2 = "RPCGASPARAMETERS.DPID = DP_NAME2ID.ID AND RPCGASPARAMETERS.CHANGE_DATE >:tmin AND RPCGASPARAMETERS.CHANGE_DATE <:tmax AND (DP_NAME2ID.DPNAME like '%C2H2F4Ratio%')";
    queryMix2->setCondition( conditionM2, conditionData );
    coral::ICursor& cursorMix2 = queryMix2->execute();
    while ( cursorMix2.next() ) {
      Mtemp.gas1=0;Mtemp.gas2=0;Mtemp.gas3=0;
      const coral::AttributeList& row = cursorMix2.currentRow();
      float value = row["VALUE"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      unsigned long long ut_time = TtoUT(ts);
      Mtemp.gas2 = value;
      Mtemp.unixtime = ut_time;
      marray.push_back(Mtemp);
    }
    std::string conditionM3 = "RPCGASPARAMETERS.DPID = DP_NAME2ID.ID AND RPCGASPARAMETERS.CHANGE_DATE >:tmin AND RPCGASPARAMETERS.CHANGE_DATE <:tmax AND (DP_NAME2ID.DPNAME like '%SF6Ratio%')";
    queryMix3->setCondition( conditionM3, conditionData );
    coral::ICursor& cursorMix3 = queryMix3->execute();
    while ( cursorMix3.next() ) {
      Mtemp.gas1=0;Mtemp.gas2=0;Mtemp.gas3=0;
      const coral::AttributeList& row = cursorMix3.currentRow();
      float value = row["VALUE"].data<float>();
      coral::TimeStamp ts =  row["TSTAMP"].data<coral::TimeStamp>();
      unsigned long long ut_time = TtoUT(ts);
      Mtemp.gas3 = value;
      Mtemp.unixtime = ut_time;
      marray.push_back(Mtemp);
    }
  } else {
    
    std::cout << "This objects cannot be copied in this mode. Use RANGE mode!" << std::endl;
    
  }
  
  
  std::cout << ">> GasMix array --> size: " << marray.size() << " >> done." << std::endl;
  delete queryMix;
  session->transaction().commit();
  delete session;
  return marray;
  
}



//----------------------------------------------------------------------------------------------
coral::TimeStamp RPCFwPerRun::UTtoT(long long utime) 
{
  
  
  int yea = static_cast<int>(trunc(utime/31536000) + 1970);
  int yes = (yea-1970)*31536000;
  int cony = ((yea-1972)%4)+1;
  if (cony == 0) yes = yes + (yea-1972)/4*86400; 
  else yes = yes +  static_cast<int>(trunc((yea-1972)/4))*86400;
  int day = static_cast<int>(trunc((utime - yes)/86400));
  int rest = static_cast<int>(utime - yes - day*86400);
  int mon = 0;
  // BISESTILE YEAR
  if (cony == 0) {
    day = day + 1; 
    if (day < 32){
      mon = 1;
      day = day - 0;
    }
    if (day >= 32 && day < 61){
      mon = 2;
      day = day - 31;
    }
    if (day >= 61 && day < 92){
      mon = 3;
      day = day - 60;
    }
    if (day >= 92 && day < 122){
      mon = 4;
      day = day - 91;
    }
    if (day >= 122 && day < 153){
      mon = 5;
      day = day - 121;
    }
    if (day >= 153 && day < 183){
      mon = 6;
      day = day - 152;
    }
    if (day >= 183 && day < 214){
      mon = 7;
      day = day - 182;
    }
    if (day >= 214 && day < 245){
      mon = 8;
      day = day - 213;
    }
    if (day >= 245 && day < 275){
      mon = 9;
      day = day - 244;
    }
    if (day >= 275 && day < 306){
      mon = 10;
      day = day - 274;
    }
    if (day >= 306 && day < 336){
      mon = 11;
      day = day - 305;
    }
    if (day >= 336){
      mon = 12;
      day = day - 335;
    }
  }
  // NOT BISESTILE YEAR
  else {
    if (day < 32){
      mon = 1;   
      day = day - 0;
    }
    if (day >= 32 && day < 60){
      mon = 2;
      day = day - 31;
    }
    if (day >= 60 && day < 91){
      mon = 3;
      day = day - 59;
    }
    if (day >= 91 && day < 121){
      mon = 4;
      day = day - 90;
    }
    if (day >= 121 && day < 152){
      mon = 5;
      day = day - 120;
    }
    if (day >= 152 && day < 182){
      mon = 6;
      day = day - 151;
    }
    if (day >= 182 && day < 213){
      mon = 7;
      day = day - 181;
    }
    if (day >= 213 && day < 244){
      mon = 8;
      day = day - 212;
    }
    if (day >= 244 && day < 274){
      mon = 9;
      day = day - 243;
    }
    if (day >= 274 && day < 305){
      mon = 10;
      day = day - 273;
    }
    if (day >= 305 && day < 335){
      mon = 11;
      day = day - 304;
    }
    if (day >= 335){
      mon = 12;
      day = day - 334;
    }
  }
  
  int hou = static_cast<int>(trunc(rest/3600)); 
  rest = rest - hou*3600;
  int min = static_cast<int>(trunc(rest/60));
  rest = rest - min*60;
  int sec = rest; 
  int nan = 0;

  //  std::cout <<">> Processing since: "<<day<<"/"<<mon<<"/"<<yea<<" "<<hou<<":"<<min<<"."<<sec<< std::endl;

  coral::TimeStamp Tthr;  

  Tthr = coral::TimeStamp(yea, mon, day, hou, min, sec, nan);
  return Tthr;
}



//----------------------------------------------------------------------------------------------
unsigned long long RPCFwPerRun::TtoUT(coral::TimeStamp time) 
{
  
  long long utime = (time.year()-1970)*31536000+static_cast<int>(trunc((time.year()-1972)/4))*86400+
    (((time.month()-1)*31)*86400)+((time.day()-1)*86400)+time.hour()*3600+time.minute()*60+time.second();
  
  if (time.month() == 3) utime = utime - 3*86400;
  if (time.month() == 4) utime = utime - 3*86400;
  if (time.month() == 5) utime = utime - 4*86400;
  if (time.month() == 6) utime = utime - 4*86400;
  if (time.month() == 7) utime = utime - 5*86400;
  if (time.month() == 8) utime = utime - 5*86400;
  if (time.month() == 9) utime = utime - 5*86400;
  if (time.month() == 10) utime = utime - 6*86400;
  if (time.month() == 11) utime = utime - 6*86400;
  if (time.month() == 12) utime = utime - 7*86400;
  
  return utime;
}


/*coral::TimeStamp RPCFwPerRun::addNWeeks(coral::TimeStamp time, int nWeeks =0) 
{
  
  long long utime = (time.year()-1970)*31536000+static_cast<int>(trunc((time.year()-1972)/4))*86400+
    (((time.month()-1)*31)*86400)+((time.day()-1)*86400)+time.hour()*3600+time.minute()*60+time.second();
  
  if (time.month() == 3) utime = utime - 3*86400;
  if (time.month() == 4) utime = utime - 3*86400;
  if (time.month() == 5) utime = utime - 4*86400;
  if (time.month() == 6) utime = utime - 4*86400;
  if (time.month() == 7) utime = utime - 5*86400;
  if (time.month() == 8) utime = utime - 5*86400;
  if (time.month() == 9) utime = utime - 5*86400;
  if (time.month() == 10) utime = utime - 6*86400;
  if (time.month() == 11) utime = utime - 6*86400;
  if (time.month() == 12) utime = utime - 7*86400;
  
  utime;
  }*/



bool RPCFwPerRun::isMajor(coral::TimeStamp fir, coral::TimeStamp sec) 
{

  double first  = fir.year()*1e10+fir.month()+1e8+fir.day()*1e6+fir.hour()*1e4+fir.minute()*1e2+fir.second();
  double second = sec.year()*1e10+sec.month()+1e8+sec.day()*1e6+sec.hour()*1e4+sec.minute()*1e2+sec.second();

  if (first > second) return true;

      return false;
}
