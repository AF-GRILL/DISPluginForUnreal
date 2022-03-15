// Fill out your copyright notice in the Description page of Project Settings.

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

void UPDUProcessor::ProcessDISPacket(TArray<uint8> InData)
{
	int bytesArrayLength = InData.Num();

	if (bytesArrayLength < 1)
	{
		return;
	}

	EPDUType receivedPDUType = static_cast<EPDUType>(InData[PDU_TYPE_POSITION]);

	//For list of enums for PDU type refer to SISO-REF-010-2015, ANNEX A
	switch (receivedPDUType)
	{
	case EPDUType::EntityState:
	{
		DIS::EntityStatePdu* receivedESPDU = new DIS::EntityStatePdu();
		DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
		receivedESPDU->unmarshal(ds);

		FEntityStatePDU entityStatePDU;
		entityStatePDU.SetupFromOpenDIS(receivedESPDU);

		OnEntityStatePDUProcessed.Broadcast(entityStatePDU);

		break;
	}
	case EPDUType::Fire:
	{
		DIS::FirePdu* receivedFirePDU = new DIS::FirePdu();
		DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
		receivedFirePDU->unmarshal(ds);

		FFirePDU firePDU;
		firePDU.SetupFromOpenDIS(receivedFirePDU);

		OnFirePDUProcessed.Broadcast(firePDU);

		break;
	}
	case EPDUType::Detonation:
	{
		DIS::DetonationPdu* receivedDetonationPDU = new DIS::DetonationPdu();
		DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
		receivedDetonationPDU->unmarshal(ds);

		FDetonationPDU detonationPDU;
		detonationPDU.SetupFromOpenDIS(receivedDetonationPDU);

		OnDetonationPDUProcessed.Broadcast(detonationPDU);

		break;
	}
	case EPDUType::RemoveEntity:
	{
		DIS::RemoveEntityPdu* receivedRemoveEntityPDU = new DIS::RemoveEntityPdu();
		DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
		receivedRemoveEntityPDU->unmarshal(ds);

		FRemoveEntityPDU removeEntityPDU;
		removeEntityPDU.SetupFromOpenDIS(receivedRemoveEntityPDU);

		OnRemoveEntityPDUProcessed.Broadcast(removeEntityPDU);

		break;
	}
	case EPDUType::Start_Resume:
	{
		DIS::StartResumePdu* receivedStartResumePDU = new DIS::StartResumePdu();
		DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
		receivedStartResumePDU->unmarshal(ds);

		FStartResumePDU StartResumePDU;
		StartResumePDU.SetupFromOpenDIS(receivedStartResumePDU);

		OnStartResumePDUProcessed.Broadcast(StartResumePDU);

		break;
	}
	case EPDUType::Stop_Freeze:
	{
		DIS::StopFreezePdu* receivedStopFreezePDU = new DIS::StopFreezePdu();
		DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
		receivedStopFreezePDU->unmarshal(ds);

		FStopFreezePDU StopFreezePDU;
		StopFreezePDU.SetupFromOpenDIS(receivedStopFreezePDU);

		OnStopFreezePDUProcessed.Broadcast(StopFreezePDU);

		break;
	}
	case EPDUType::EntityStateUpdate:
	{
		DIS::EntityStateUpdatePdu* receivedESUPDU = new DIS::EntityStateUpdatePdu();
		DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
		receivedESUPDU->unmarshal(ds);

		FEntityStateUpdatePDU entityStateUpdatePDU;
		entityStateUpdatePDU.SetupFromOpenDIS(receivedESUPDU);

		OnEntityStateUpdatePDUProcessed.Broadcast(entityStateUpdatePDU);

		break;
	}
	default:
	{
		break;
	}
	}
}