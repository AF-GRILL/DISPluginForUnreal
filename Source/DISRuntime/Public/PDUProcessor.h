// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DISEnumsAndStructs.h"

#include "CoreMinimal.h"
#include "PDUMasterInclude.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PDUProcessor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEntityStatePDUProcessed, UGRILL_EntityStatePDU*, EntityStatePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEntityStateUpdatePDUProcessed, UGRILL_EntityStateUpdatePDU*, EntityStateUpdatePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDetonationPDUProcessed, UGRILL_DetonationPDU*, DetonationPDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFirePDUProcessed, UGRILL_FirePDU*, FirePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveEntityPDUProcessed, UGRILL_RemoveEntityPDU*, RemoveEntityPDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartResumePDUProcessed, UGRILL_StartResumePDU*, StartResumePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStopFreezePDUProcessed, UGRILL_StopFreezePDU*, StopFreezePDU);

UCLASS()
class DISRUNTIME_API UPDUProcessor : public UGameInstanceSubsystem
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
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|PDU Processor")
		void ProcessDISPacket(TArray<uint8> InData);
	
	/**
	 * Called after an Entity State PDU is processed.
	 * Passes the Entity State PDU as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|PDU Processor|Events")
		FEntityStatePDUProcessed OnEntityStatePDUProcessed;
	/**
	 * Called after an Entity State Update PDU is processed.
	 * Passes the Entity State Update PDU as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|PDU Processor|Events")
		FEntityStateUpdatePDUProcessed OnEntityStateUpdatePDUProcessed;
	/**
	 * Called after a Detonation PDU is processed.
	 * Passes the Detonation PDU as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|PDU Processor|Events")
		FDetonationPDUProcessed OnDetonationPDUProcessed;
	/**
	 * Called after a Fire PDU is processed.
	 * Passes the Fire PDU as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|PDU Processor|Events")
		FFirePDUProcessed OnFirePDUProcessed;
	/**
	 * Called after a Remove Entity PDU is processed.
	 * Passes the Remove Entity PDU as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|PDU Processor|Events")
		FRemoveEntityPDUProcessed OnRemoveEntityPDUProcessed;
	/**
	 * Called after a Start/Resume PDU is processed.
	 * Passes the Start Resume PDU as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|PDU Processor|Events")
		FStartResumePDUProcessed OnStartResumePDUProcessed;
	/**
	 * Called after a Stop/Freeze PDU is processed.
	 * Passes the Stop Freeze PDU as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|PDU Processor|Events")
		FStopFreezePDUProcessed OnStopFreezePDUProcessed;

protected:
	UFUNCTION()
		void HandleOnReceivedUDPBytes(const TArray<uint8>& Bytes, const FString& IPAddress);

private:
	DIS::Endian BigEndian = DIS::BIG;
	const unsigned int PDU_TYPE_POSITION = 2;
};
