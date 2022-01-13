// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DISEnumsAndStructs.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PDUProcessor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEntityStatePDUProcessed, FEntityStatePDU, EntityStatePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEntityStateUpdatePDUProcessed, FEntityStateUpdatePDU, EntityStateUpdatePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDetonationPDUProcessed, FDetonationPDU, DetonationPDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFirePDUProcessed, FFirePDU, FirePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveEntityPDUProcessed, FRemoveEntityPDU, RemoveEntityPDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartResumePDUProcessed, FStartResumePDU, StartResumePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStopFreezePDUProcessed, FStopFreezePDU, StopFreezePDU);

UCLASS()
class UEOPENDIS_API UPDUProcessor : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	/**
	 * Processes a given DIS packet to determine the type of packet. Delegates handling of the packet to whatever is bound to the associated PDU type's OnPDUProcessed event.
	 * @param InData - The DIS packet in bytes to process.
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void ProcessDISPacket(TArray<uint8> InData);
	/**
	 * Converts the given Entity State PDU to bytes. Useful if sending the packet over UDP is desired.
	 * @param Exercise - The exercise number that the given Entity State PDU is associated with.
	 * @param EntityStatePDUIn - The Entity State PDU to convert to bytes.
	 * @param BytesOut - The given Entity State PDU in bytes.
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void ConvertESPDU2Bytes(int Exercise, FEntityStatePDU EntityStatePDUIn, TArray<uint8>& BytesOut);
	
	/**
	 * Called after an Entity State PDU is processed.
	 * Passes the Entity State PDU as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "PDU Processor Events")
		FEntityStatePDUProcessed OnEntityStatePDUProcessed;
	/**
	 * Called after an Entity State Update PDU is processed.
	 * Passes the Entity State Update PDU as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "PDU Processor Events")
		FEntityStateUpdatePDUProcessed OnEntityStateUpdatePDUProcessed;
	/**
	 * Called after a Detonation PDU is processed.
	 * Passes the Detonation PDU as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "PDU Processor Events")
		FDetonationPDUProcessed OnDetonationPDUProcessed;
	/**
	 * Called after a Fire PDU is processed.
	 * Passes the Fire PDU as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "PDU Processor Events")
		FFirePDUProcessed OnFirePDUProcessed;
	/**
	 * Called after a Remove Entity PDU is processed.
	 * Passes the Remove Entity PDU as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "PDU Processor Events")
		FRemoveEntityPDUProcessed OnRemoveEntityPDUProcessed;
	/**
	 * Called after a Start/Resume PDU is processed.
	 * Passes the Start Resume PDU as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "PDU Processor Events")
		FStartResumePDUProcessed OnStartResumePDUProcessed;
	/**
	 * Called after a Stop/Freeze PDU is processed.
	 * Passes the Stop Freeze PDU as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "PDU Processor Events")
		FStopFreezePDUProcessed OnStopFreezePDUProcessed;

protected:
	UFUNCTION()
		void HandleOnReceivedUDPBytes(const TArray<uint8>& Bytes, const FString& IPAddress);

private:
	FEntityStatePDU ConvertESPDUtoBPStruct(DIS::EntityStatePdu* EntityStatePDUIn);
	FEntityStateUpdatePDU ConvertEntityStateUpdatePDUtoBPStruct(DIS::EntityStateUpdatePdu* EntityStateUpdatePDUIn);
	FFirePDU ConvertFirePDUtoBPStruct(DIS::FirePdu* FirePDUIn);
	FDetonationPDU ConvertDetonationPDUtoBPStruct(DIS::DetonationPdu* DetPDUIn);
	FRemoveEntityPDU ConvertRemoveEntityPDUtoBPStruct(DIS::RemoveEntityPdu* RemovePDUIn);
	FStartResumePDU ConvertStartResumePDUtoBPStruct(DIS::StartResumePdu* StartResumePDUIn);
	FStopFreezePDU ConvertStopFreezePDUtoBPStruct(DIS::StopFreezePdu* StopFreezePDUIn);

	DIS::Endian BigEndian = DIS::BIG;
	const unsigned int PDU_TYPE_POSITION = 2;
};
