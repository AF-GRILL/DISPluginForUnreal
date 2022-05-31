// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.


#include "PDUConversions_BPFL.h"

TArray<uint8> UPDUConversions_BPFL::ConvertPDUToBytes(FPDU PDUIn)
{
	return PDUIn.ToBytes();
}

TArray<uint8> UPDUConversions_BPFL::ConvertEntityInfoFamilyPDUToBytes(FEntityInformationFamilyPDU EntityInfoFamilyPDUIn)
{
	return EntityInfoFamilyPDUIn.ToBytes();
}

TArray<uint8> UPDUConversions_BPFL::ConvertSimManagementFamilyPDUToBytes(FSimulationManagementFamilyPDU SimManagementFamilyPDUIn)
{
	return SimManagementFamilyPDUIn.ToBytes();
}

TArray<uint8> UPDUConversions_BPFL::ConvertWarfareFamilyPDUToBytes(FWarfareFamilyPDU WarfareFamilyPDUIn)
{
	return WarfareFamilyPDUIn.ToBytes();
}

TArray<uint8> UPDUConversions_BPFL::ConvertEntityStatePDUToBytes(FEntityStatePDU EntityStatePDUIn)
{
    return EntityStatePDUIn.ToBytes();
}

TArray<uint8> UPDUConversions_BPFL::ConvertEntityStateUpdatePDUToBytes(FEntityStateUpdatePDU EntityStateUpdatePDUIn)
{
	return EntityStateUpdatePDUIn.ToBytes();
}

TArray<uint8> UPDUConversions_BPFL::ConvertFirePDUToBytes(FFirePDU FirePDUIn)
{
	return FirePDUIn.ToBytes();
}

TArray<uint8> UPDUConversions_BPFL::ConvertDetonationPDUToBytes(FDetonationPDU DetonationPDUIn)
{
	return DetonationPDUIn.ToBytes();
}

TArray<uint8> UPDUConversions_BPFL::ConvertRemoveEntityPDUToBytes(FRemoveEntityPDU RemoveEntityPDUIn)
{
	return RemoveEntityPDUIn.ToBytes();
}

TArray<uint8> UPDUConversions_BPFL::ConvertStartResumePDUToBytes(FStartResumePDU StartResumePDUIn)
{
	return StartResumePDUIn.ToBytes();
}

TArray<uint8> UPDUConversions_BPFL::ConvertStopFreezePDUToBytes(FStopFreezePDU StopFreezePDUIn)
{
	return StopFreezePDUIn.ToBytes();
}
