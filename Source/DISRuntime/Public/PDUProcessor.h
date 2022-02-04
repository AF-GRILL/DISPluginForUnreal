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
	 * Converts the given Entity State PDU to bytes. Useful if sending the packet over UDP is desired.
	 * @param Exercise - The exercise number that the given Entity State PDU is associated with.
	 * @param EntityStatePDUIn - The Entity State PDU to convert to bytes.
	 * @param BytesOut - The given Entity State PDU in bytes.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|PDU Processor")
		void ConvertEntityStatePDUtoBytes(int Exercise, FEntityStatePDU EntityStatePDUIn, TArray<uint8>& BytesOut);

	/**
	 * Converts the given Entity State Update PDU to bytes. Useful if sending the packet over UDP is desired.
	 * @param Exercise - The exercise number that the given Entity State PDU is associated with.
	 * @param EntityStateUpdatePDUIn - The Entity State Update PDU to convert to bytes.
	 * @param BytesOut - The given Entity State Update PDU in bytes.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|PDU Processor")
		void ConvertEntityStateUpdatePDUtoBytes(int Exercise, FEntityStateUpdatePDU EntityStateUpdatePDUIn, TArray<uint8>& BytesOut);
	/**
	 * Converts the given Fire PDU to bytes. Useful if sending the packet over UDP is desired.
	 * @param Exercise - The exercise number that the given Entity State PDU is associated with.
	 * @param FirePDUIn - The Fire PDU to convert to bytes.
	 * @param BytesOut - The given Fire PDU in bytes.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|PDU Processor")
		void ConvertFirePDUtoBytes(int Exercise, FFirePDU FirePDUIn, TArray<uint8>& BytesOut);
	/**
	 * Converts the given Remove Entity PDU to bytes. Useful if sending the packet over UDP is desired.
	 * @param Exercise - The exercise number that the given Entity State PDU is associated with.
	 * @param RemoveEntityPDUIn - The Remove Entity PDU to convert to bytes.
	 * @param BytesOut - The given Remove Entity PDU in bytes.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|PDU Processor")
		void ConvertRemoveEntityPDUtoBytes(int Exercise, FRemoveEntityPDU RemoveEntityPDUIn, TArray<uint8>& BytesOut);
	/**
	 * Converts the given Detonation PDU to bytes. Useful if sending the packet over UDP is desired.
	 * @param Exercise - The exercise number that the given Entity State PDU is associated with.
	 * @param DetonationPDUIn - The Detonation PDU to convert to bytes.
	 * @param BytesOut - The given Detonation PDU in bytes.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|PDU Processor")
		void ConvertDetonationPDUtoBytes(int Exercise, FDetonationPDU DetonationPDUIn, TArray<uint8>& BytesOut);
	/**
	 * Converts the given Start Resume PDU to bytes. Useful if sending the packet over UDP is desired.
	 * @param Exercise - The exercise number that the given Entity State PDU is associated with.
	 * @param StartResumePDUIn - The Start Resume PDU to convert to bytes.
	 * @param BytesOut - The given Start Resume PDU in bytes.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|PDU Processor")
		void ConvertStartResumePDUtoBytes(int Exercise, FStartResumePDU StartResumePDUIn, TArray<uint8>& BytesOut);
	/**
	 * Converts the given Stop Freeze PDU to bytes. Useful if sending the packet over UDP is desired.
	 * @param Exercise - The exercise number that the given Entity State PDU is associated with.
	 * @param StopFreezePDUIn - The Stop Freeze PDU to convert to bytes.
	 * @param BytesOut - The given Stop Freeze PDU in bytes.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|PDU Processor")
		void ConvertStopFreezePDUtoBytes(int Exercise, FStopFreezePDU StopFreezePDUIn, TArray<uint8>& BytesOut);
	
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
	FEntityStatePDU ConvertEntityStatePDUtoBPStruct(DIS::EntityStatePdu* EntityStatePDUIn);
	FEntityStateUpdatePDU ConvertEntityStateUpdatePDUtoBPStruct(DIS::EntityStateUpdatePdu* EntityStateUpdatePDUIn);
	FFirePDU ConvertFirePDUtoBPStruct(DIS::FirePdu* FirePDUIn);
	FDetonationPDU ConvertDetonationPDUtoBPStruct(DIS::DetonationPdu* DetPDUIn);
	FRemoveEntityPDU ConvertRemoveEntityPDUtoBPStruct(DIS::RemoveEntityPdu* RemovePDUIn);
	FStartResumePDU ConvertStartResumePDUtoBPStruct(DIS::StartResumePdu* StartResumePDUIn);
	FStopFreezePDU ConvertStopFreezePDUtoBPStruct(DIS::StopFreezePdu* StopFreezePDUIn);

	DIS::Endian BigEndian = DIS::BIG;
	const unsigned int PDU_TYPE_POSITION = 2;
};
