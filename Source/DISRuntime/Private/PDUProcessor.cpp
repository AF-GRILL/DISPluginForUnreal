// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#include "PDUProcessor.h"
#include "UDPSubsystem.h"
#include <stdexcept>
#include "Engine/GameInstance.h"
#include "Subsystems/SubsystemCollection.h"

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
		try
		{
			DIS::EntityStatePdu receivedESPDU;
			receivedESPDU.unmarshal(ds);

			FEntityStatePDU entityStatePDU;
			entityStatePDU.SetupFromOpenDIS(receivedESPDU);

			OnEntityStatePDUProcessed.Broadcast(entityStatePDU);

			return;
		}
		catch ([[maybe_unused]] const std::out_of_range& ex)
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Entity State PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}
	}
	case EPDUType::Fire:
	{
		try
		{
			DIS::FirePdu receivedFirePDU;
			receivedFirePDU.unmarshal(ds);

			FFirePDU firePDU;
			firePDU.SetupFromOpenDIS(receivedFirePDU);

			OnFirePDUProcessed.Broadcast(firePDU);

			return;
		}
		catch ([[maybe_unused]] const std::out_of_range& ex)
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Fire PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}
	}
	case EPDUType::Detonation:
	{
		try
		{
			DIS::DetonationPdu receivedDetonationPDU;
			receivedDetonationPDU.unmarshal(ds);

			FDetonationPDU detonationPDU;
			detonationPDU.SetupFromOpenDIS(receivedDetonationPDU);

			OnDetonationPDUProcessed.Broadcast(detonationPDU);

			return;
		}
		catch ([[maybe_unused]] const std::out_of_range& ex)
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Detonation PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}
	}
	case EPDUType::RemoveEntity:
	{
		try
		{
			DIS::RemoveEntityPdu receivedRemoveEntityPDU;
			receivedRemoveEntityPDU.unmarshal(ds);

			FRemoveEntityPDU removeEntityPDU;
			removeEntityPDU.SetupFromOpenDIS(receivedRemoveEntityPDU);

			OnRemoveEntityPDUProcessed.Broadcast(removeEntityPDU);

			return;
		}
		catch ([[maybe_unused]] const std::out_of_range& ex)
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Remove Entity PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}
	}
	case EPDUType::Start_Resume:
	{
		try
		{
			DIS::StartResumePdu receivedStartResumePDU;
			receivedStartResumePDU.unmarshal(ds);

			FStartResumePDU StartResumePDU;
			StartResumePDU.SetupFromOpenDIS(receivedStartResumePDU);

			OnStartResumePDUProcessed.Broadcast(StartResumePDU);

			return;
		}
		catch ([[maybe_unused]] const std::out_of_range& ex)
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Start Resume PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}
	}
	case EPDUType::Stop_Freeze:
	{
		try
		{
			DIS::StopFreezePdu receivedStopFreezePDU;
			receivedStopFreezePDU.unmarshal(ds);

			FStopFreezePDU StopFreezePDU;
			StopFreezePDU.SetupFromOpenDIS(receivedStopFreezePDU);

			OnStopFreezePDUProcessed.Broadcast(StopFreezePDU);

			return;
		}
		catch ([[maybe_unused]] const std::out_of_range& ex)
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Stop Freeze PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}
	}
	case EPDUType::EntityStateUpdate:
	{
		try
		{
			DIS::EntityStateUpdatePdu receivedESUPDU;
			receivedESUPDU.unmarshal(ds);

			FEntityStateUpdatePDU entityStateUpdatePDU;
			entityStateUpdatePDU.SetupFromOpenDIS(receivedESUPDU);

			OnEntityStateUpdatePDUProcessed.Broadcast(entityStateUpdatePDU);

			return;
		}
		catch ([[maybe_unused]] const std::out_of_range& ex)
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Entity State Update PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}
	}
	case EPDUType::ElectromagneticEmission:
	{
		try
		{
			DIS::ElectromagneticEmissionsPdu receivedPDU;
			receivedPDU.unmarshal(ds);

			FElectromagneticEmissionsPDU pdu;
			pdu.SetupFromOpenDIS(receivedPDU);

			OnElectromagneticEmissionsPDUProcessed.Broadcast(pdu);

			return;
		}
		catch ([[maybe_unused]] const std::out_of_range& ex)
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Electromagnetic Emission PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}
	}
	case EPDUType::Designator:
	{
		try
		{
			DIS::DesignatorPdu receivedPDU;
			receivedPDU.unmarshal(ds);

			FDesignatorPDU pdu;
			pdu.SetupFromOpenDIS(receivedPDU);

			OnDesignatorPDUProcessed.Broadcast(pdu);

			return;
		}
		catch ([[maybe_unused]] const std::out_of_range& ex)
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Designator PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}
	}
	case EPDUType::Signal:
	{
		try
		{
			DIS::SignalPdu receivedPDU;
			receivedPDU.unmarshal(ds);

			FSignalPDU pdu;
			pdu.SetupFromOpenDIS(receivedPDU);

			OnSignalPDUProcessed.Broadcast(pdu);

			return;
		}
		catch ([[maybe_unused]] const std::out_of_range& ex)
		{
			UE_LOG(LogPDUProcessor, Error, TEXT("Received Signal PDU packet with an invalid length! Ignoring the PDU."));
			return;
		}
	}
	}
}
