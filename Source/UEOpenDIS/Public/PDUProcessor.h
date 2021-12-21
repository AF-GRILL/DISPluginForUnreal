// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DISEnumsAndStructs.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PDUProcessor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPDUProcessorSubsystem, Log, All);

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

	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void ProcessDISPacket(TArray<uint8> InData);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void ConvertESPDU2Bytes(int EntityID, int Site, int Application, int Exercise, FEntityStatePDU EntityStatePDUIn, TArray<uint8>& BytesOut);
	
	UPROPERTY(BlueprintAssignable, Category = "PDU Processor Events")
		FEntityStatePDUProcessed OnEntityStatePDUProcessed;
	UPROPERTY(BlueprintAssignable, Category = "PDU Processor Events")
		FDetonationPDUProcessed OnDetonationPDUProcessed;
	UPROPERTY(BlueprintAssignable, Category = "PDU Processor Events")
		FFirePDUProcessed OnFirePDUProcessed;
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
