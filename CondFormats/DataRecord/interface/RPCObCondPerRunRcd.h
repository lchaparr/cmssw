#ifndef DataRecord_RPCObCondPerRunRcd_h
#define DataRecord_RPCObCondPerRunRcd_h
// -*- C++ -*-
//
// Package:     DataRecord
// Class  :     RPCObCondRcd
// 
/**\class RPCObCondRcd RPCObCondRcd.h CondFormats/DataRecord/interface/RPCObCondRcd.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      
// Created:     Fri Oct 10 20:02:37 CEST 2008
// $Id$
//

#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"

class RPCObImonPerRunRcd : public edm::eventsetup::EventSetupRecordImplementation<RPCObImonPerRunRcd> {};

class RPCObImonPerLumiRcd : public edm::eventsetup::EventSetupRecordImplementation<RPCObImonPerLumiRcd> {};

class RPCObVmonPerRunRcd : public edm::eventsetup::EventSetupRecordImplementation<RPCObVmonPerRunRcd> {};

class RPCObTempPerRunRcd : public edm::eventsetup::EventSetupRecordImplementation<RPCObTempPerRunRcd> {};

#endif
