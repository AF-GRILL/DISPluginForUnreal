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

	DIS::Pdu* pdu = NULL;

	pdu = DIS::PduBank::GetStaticPDU(static_cast<DIS::PDUType>(InData[PDU_TYPE_POSITION]));
	if (pdu)
	{
		//For list of enums for PDU type refer to SISO-REF-010-2015, ANNEX A
		switch (static_cast<EPDUType>(pdu->getPduType()))
		{
		case EPDUType::EntityState:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);

			FEntityStatePDU entityStatePDU;
			entityStatePDU.SetupFromOpenDIS(static_cast<DIS::EntityStatePdu*>(pdu));

			OnEntityStatePDUProcessed.Broadcast(entityStatePDU);

			break;
		}
		case EPDUType::Fire:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);

			FFirePDU firePDU;
			firePDU.SetupFromOpenDIS(static_cast<DIS::FirePdu*>(pdu));

			OnFirePDUProcessed.Broadcast(firePDU);

			break;
		}
		case EPDUType::Detonation:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);

			FDetonationPDU detonationPDU;
			detonationPDU.SetupFromOpenDIS(static_cast<DIS::DetonationPdu*>(pdu));

			OnDetonationPDUProcessed.Broadcast(detonationPDU);

			break;
		}
		case EPDUType::RemoveEntity:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);

			FRemoveEntityPDU removeEntityPDU;
			removeEntityPDU.SetupFromOpenDIS(static_cast<DIS::RemoveEntityPdu*>(pdu));

			OnRemoveEntityPDUProcessed.Broadcast(removeEntityPDU);

			break;
		}
		case EPDUType::Start_Resume:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);

			FStartResumePDU StartResumePDU;
			StartResumePDU.SetupFromOpenDIS(static_cast<DIS::StartResumePdu*>(pdu));

			OnStartResumePDUProcessed.Broadcast(StartResumePDU);

			break;
		}
		case EPDUType::Stop_Freeze:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);

			FStopFreezePDU StopFreezePDU;
			StopFreezePDU.SetupFromOpenDIS(static_cast<DIS::StopFreezePdu*>(pdu));

			OnStopFreezePDUProcessed.Broadcast(StopFreezePDU);

			break;
		}
		case EPDUType::EntityStateUpdate:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);

			FEntityStateUpdatePDU entityStateUpdatePDU;
			entityStateUpdatePDU.SetupFromOpenDIS(static_cast<DIS::EntityStateUpdatePdu*>(pdu));

			OnEntityStateUpdatePDUProcessed.Broadcast(entityStateUpdatePDU);

			break;
		}
		default:
		{
			break;
		}
		}
	}
}