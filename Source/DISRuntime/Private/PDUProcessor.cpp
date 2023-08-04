// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#include "PDUProcessor.h"
#include "UDPSubsystem.h"

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
		DIS::EntityStatePdu receivedESPDU;
		receivedESPDU.unmarshal(ds);

		FEntityStatePDU entityStatePDU;
		entityStatePDU.SetupFromOpenDIS(receivedESPDU);

		OnEntityStatePDUProcessed.Broadcast(entityStatePDU);

		return;
	}
	case EPDUType::Fire:
	{
		DIS::FirePdu receivedFirePDU;
		receivedFirePDU.unmarshal(ds);

		FFirePDU firePDU;
		firePDU.SetupFromOpenDIS(receivedFirePDU);

		OnFirePDUProcessed.Broadcast(firePDU);

		return;
	}
	case EPDUType::Detonation:
	{
		DIS::DetonationPdu receivedDetonationPDU;
		receivedDetonationPDU.unmarshal(ds);

		FDetonationPDU detonationPDU;
		detonationPDU.SetupFromOpenDIS(receivedDetonationPDU);

		OnDetonationPDUProcessed.Broadcast(detonationPDU);

		return;
	}
	case EPDUType::RemoveEntity:
	{
		DIS::RemoveEntityPdu receivedRemoveEntityPDU;
		receivedRemoveEntityPDU.unmarshal(ds);

		FRemoveEntityPDU removeEntityPDU;
		removeEntityPDU.SetupFromOpenDIS(receivedRemoveEntityPDU);

		OnRemoveEntityPDUProcessed.Broadcast(removeEntityPDU);

		return;
	}
	case EPDUType::Start_Resume:
	{
		DIS::StartResumePdu receivedStartResumePDU;
		receivedStartResumePDU.unmarshal(ds);

		FStartResumePDU StartResumePDU;
		StartResumePDU.SetupFromOpenDIS(receivedStartResumePDU);

		OnStartResumePDUProcessed.Broadcast(StartResumePDU);

		return;
	}
	case EPDUType::Stop_Freeze:
	{
		DIS::StopFreezePdu receivedStopFreezePDU;
		receivedStopFreezePDU.unmarshal(ds);

		FStopFreezePDU StopFreezePDU;
		StopFreezePDU.SetupFromOpenDIS(receivedStopFreezePDU);

		OnStopFreezePDUProcessed.Broadcast(StopFreezePDU);

		return;
	}
	case EPDUType::EntityStateUpdate:
	{
		DIS::EntityStateUpdatePdu receivedESUPDU;
		receivedESUPDU.unmarshal(ds);

		FEntityStateUpdatePDU entityStateUpdatePDU;
		entityStateUpdatePDU.SetupFromOpenDIS(receivedESUPDU);

		OnEntityStateUpdatePDUProcessed.Broadcast(entityStateUpdatePDU);

		return;
	}
	}
}