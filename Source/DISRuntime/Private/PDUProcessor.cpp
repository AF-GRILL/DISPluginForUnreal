// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#include "PDUProcessor.h"
#include "UDPSubsystem.h"

DEFINE_LOG_CATEGORY(LogPDUProcessor);

void UPDUProcessor::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency(UUDPSubsystem::StaticClass());
	Super::Initialize(Collection);

	//Get the UDP Subsystem and bind to receiving UDP Bytes
	GetGameInstance()->GetSubsystem<UUDPSubsystem>()->OnReceivedBytes.AddDynamic(this, &UPDUProcessor::HandleOnReceivedUDPBytes);
}

void UPDUProcessor::Deinitialize()
{
	Super::Deinitialize();
}

void UPDUProcessor::HandleOnReceivedUDPBytes(const TArray<uint8>& Bytes, const FString& IPAddress)
{
	ProcessDISPacket(Bytes);
}

void UPDUProcessor::ProcessDISPacket(const TArray<uint8>& InData)
{
	SCOPE_CYCLE_COUNTER(STAT_ProcessDISPacket);
	int bytesArrayLength = InData.Num();

	if (bytesArrayLength < 1)
	{
		return;
	}

	const EPDUType receivedPDUType = static_cast<EPDUType>(InData[PDU_TYPE_POSITION]);

	DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);

	//For list of enums for PDU type refer to SISO-REF-010-2015, ANNEX A
	switch (receivedPDUType)
	{
	case EPDUType::EntityState:
	{
		if (!CheckPDUProperLengthWithArticulationParams(bytesArrayLength, ENTITY_STATE_PDU_BYTES))
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Entity State PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}

		DIS::EntityStatePdu receivedESPDU;
		receivedESPDU.unmarshal(ds);

		FEntityStatePDU entityStatePDU;
		entityStatePDU.SetupFromOpenDIS(receivedESPDU);

		OnEntityStatePDUProcessed.Broadcast(entityStatePDU);

		return;
	}
	case EPDUType::Fire:
	{
		if (bytesArrayLength != FIRE_PDU_BYTES)
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Fire PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}

		DIS::FirePdu receivedFirePDU;
		receivedFirePDU.unmarshal(ds);

		FFirePDU firePDU;
		firePDU.SetupFromOpenDIS(receivedFirePDU);

		OnFirePDUProcessed.Broadcast(firePDU);

		return;
	}
	case EPDUType::Detonation:
	{
		if (!CheckPDUProperLengthWithArticulationParams(bytesArrayLength, DETONATION_PDU_BYTES))
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Detonation PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}

		DIS::DetonationPdu receivedDetonationPDU;
		receivedDetonationPDU.unmarshal(ds);

		FDetonationPDU detonationPDU;
		detonationPDU.SetupFromOpenDIS(receivedDetonationPDU);

		OnDetonationPDUProcessed.Broadcast(detonationPDU);

		return;
	}
	case EPDUType::RemoveEntity:
	{
		if (bytesArrayLength != REMOVE_ENTITY_PDU_BYTES)
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Remove Entity PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}

		DIS::RemoveEntityPdu receivedRemoveEntityPDU;
		receivedRemoveEntityPDU.unmarshal(ds);

		FRemoveEntityPDU removeEntityPDU;
		removeEntityPDU.SetupFromOpenDIS(receivedRemoveEntityPDU);

		OnRemoveEntityPDUProcessed.Broadcast(removeEntityPDU);

		return;
	}
	case EPDUType::Start_Resume:
	{
		if (bytesArrayLength != START_RESUME_PDU_BYTES)
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Start Resume PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}

		DIS::StartResumePdu receivedStartResumePDU;
		receivedStartResumePDU.unmarshal(ds);

		FStartResumePDU StartResumePDU;
		StartResumePDU.SetupFromOpenDIS(receivedStartResumePDU);

		OnStartResumePDUProcessed.Broadcast(StartResumePDU);

		return;
	}
	case EPDUType::Stop_Freeze:
	{
		if (bytesArrayLength != STOP_FREEZE_PDU_BYTES)
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Stop Freeze PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}

		DIS::StopFreezePdu receivedStopFreezePDU;
		receivedStopFreezePDU.unmarshal(ds);

		FStopFreezePDU StopFreezePDU;
		StopFreezePDU.SetupFromOpenDIS(receivedStopFreezePDU);

		OnStopFreezePDUProcessed.Broadcast(StopFreezePDU);

		return;
	}
	case EPDUType::EntityStateUpdate:
	{
		if (!CheckPDUProperLengthWithArticulationParams(bytesArrayLength, ENTITY_STATE_UPDATE_PDU_BYTES))
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Entity State Update PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}

		DIS::EntityStateUpdatePdu receivedESUPDU;
		receivedESUPDU.unmarshal(ds);

		FEntityStateUpdatePDU entityStateUpdatePDU;
		entityStateUpdatePDU.SetupFromOpenDIS(receivedESUPDU);

		OnEntityStateUpdatePDUProcessed.Broadcast(entityStateUpdatePDU);

		return;
	}
	case EPDUType::ElectromagneticEmission:
	{
		if (!CheckElectromagneticEmissionPDUProperLength(InData))
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Electromagnetic Emission PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}

		DIS::ElectromagneticEmissionsPdu receivedPDU;
		receivedPDU.unmarshal(ds);

		FElectromagneticEmissionsPDU pdu;
		pdu.SetupFromOpenDIS(receivedPDU);

		OnElectromagneticEmissionsPDUProcessed.Broadcast(pdu);

		return;
	}
	case EPDUType::Signal:
	{
		if (!CheckSignalPDUProperLength(InData))
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Signal PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}

		DIS::SignalPdu receivedPDU;
		receivedPDU.unmarshal(ds);

		FSignalPDU pdu;
		pdu.SetupFromOpenDIS(receivedPDU);

		OnSignalPDUProcessed.Broadcast(pdu);

		return;
	}
	}
}

bool UPDUProcessor::CheckPDUProperLengthWithArticulationParams(int BytesArrayLength, int PDULengthWithoutArticulationParams)
{
	//Verify that any extra byte length on a PDU is due to articulation parameters
	int extraBytes = BytesArrayLength - PDULengthWithoutArticulationParams;
	int articulationParamAmount = extraBytes / ARTICULATION_PARAMETER_BYTES;
	return (articulationParamAmount * ARTICULATION_PARAMETER_BYTES) == extraBytes;
}

bool UPDUProcessor::CheckElectromagneticEmissionPDUProperLength(const TArray<uint8>& InData)
{
	int bytesArrayLength = InData.Num();
	//Get the number of systems in the PDU
	const int numberOfSystems = static_cast<int>(InData[25]);
	const int lastIndexIfNoEmitterData = 27;
	int currentIndex = lastIndexIfNoEmitterData;

	for (int i = 0; i < numberOfSystems; i++)
	{
		//Increment to get the number of beams in the current system
		currentIndex += 2;
		int numberOfBeams = static_cast<int>(InData[currentIndex]);

		//Increment to get to the end of the emitter system data
		currentIndex += 18;

		for (int j = 0; j < numberOfBeams; j++)
		{
			//Increment to get the number of targets
			currentIndex += 46;
			int numberOfTargets = static_cast<int>(InData[currentIndex]);

			//Increment to get to the end of the beam data
			currentIndex += 6;
			//Skip over number of targets
			currentIndex += 8 * numberOfTargets;
		}
	}

	//Doing currentIndex + 1 to convert from an array index
	return (currentIndex + 1) == bytesArrayLength;
}

bool UPDUProcessor::CheckSignalPDUProperLength(const TArray<uint8>& InData)
{
	int bytesArrayLength = InData.Num();
	//Get the data length in bytes
	const int dataLength = static_cast<int>(InData[28] << 8 | (InData[29])) / 8;
	const int paddingSize = ceil(dataLength / 32.f) * 32 - dataLength;

	return (dataLength + paddingSize + SIGNAL_PDU_BYTES) == bytesArrayLength;
}
