// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DISEnumsAndStructs.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PDUProcessor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEntityStatePDUProcessed, FEntityStatePDU, EntityStatePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDetonationPDUProcessed, FDetonationPDU, DetonationPDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFirePDUProcessed, FFirePDU, FirePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveEntityPDUProcessed, FRemoveEntityPDU, RemoveEntityPDU);

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

protected:
	UFUNCTION()
		void HandleOnReceivedUDPBytes(const TArray<uint8>& Bytes, const FString& IPAddress);

private:
	FEntityStatePDU ConvertESPDUtoBPStruct(DIS::EntityStatePdu& EntityStatePDUOut);
	FFirePDU ConvertFirePDUtoBPStruct(DIS::FirePdu& FirePDUOut);
	FDetonationPDU ConvertDetonationPDUtoBPStruct(DIS::DetonationPdu& DetPDUOut);
	FRemoveEntityPDU ConvertRemoveEntityPDUtoBPStruct(DIS::RemoveEntityPdu& RemovePDUOut);

	DIS::Endian BigEndian = DIS::BIG;
	const unsigned int PDU_TYPE_POSITION = 2;
};
