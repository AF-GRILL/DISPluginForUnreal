// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

//All low level custom PDUs
#include "PDUs/GRILL_PDU.h"
#include "PDUs/EntityInfoFamily/GRILL_EntityInformationFamilyPDU.h"
#include "PDUs/EntityInfoFamily/GRILL_EntityStatePDU.h"
#include "PDUs/EntityInfoFamily/GRILL_EntityStateUpdatePDU.h"
#include "PDUs/SimManagementFamily/GRILL_RemoveEntityPDU.h"
#include "PDUs/SimManagementFamily/GRILL_SimulationManagementFamilyPDU.h"
#include "PDUs/SimManagementFamily/GRILL_StartResumePDU.h"
#include "PDUs/SimManagementFamily/GRILL_StopFreezePDU.h"
#include "PDUs/WarfareFamily/GRILL_DetonationPDU.h"
#include "PDUs/WarfareFamily/GRILL_FirePDU.h"
#include "PDUs/WarfareFamily/GRILL_WarfareFamilyPDU.h"
#include "PDUMasterInclude.generated.h"

/**
 *
 */
UCLASS()
class DISRUNTIME_API UPDUMasterInclude : public UObject
{
	GENERATED_BODY()
};