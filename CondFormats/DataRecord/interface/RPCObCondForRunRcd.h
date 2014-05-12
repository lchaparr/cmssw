#ifndef DataRecord_RPCObCondForRunRcd_h
#define DataRecord_RPCObCondForRunRcd_h
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

class RPCObImonRcd : public edm::eventsetup::EventSetupRecordImplementation<RPCObImonForRunRcd> {};

class RPCObVmonRcd : public edm::eventsetup::EventSetupRecordImplementation<RPCObVmonForRunRcd> {};

class RPCObTempRcd : public edm::eventsetup::EventSetupRecordImplementation<RPCObTempForRunRcd> {};

#endif
