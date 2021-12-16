// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//dis headers
#include <dis6/Pdu.h>
#include <utils/PDUBank.h>
#include <utils/DataStream.h>

//PDU type headers, add as more pdu types are support!
#include <dis6/EntityStatePdu.h>   
#include <dis6/FirePdu.h>
#include <dis6/DetonationPdu.h>
#include <dis6/RemoveEntityPdu.h>

//UDP headers
#include "UDPComponent.h"

//default headers
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UEOpenDISGameState.generated.h"

class UOpenDISComponent;

UENUM(BlueprintType)
enum class EForceID : uint8
{
	Other    UMETA(DisplayName = "Other"),
	Friendly     UMETA(DisplayName = "Friendly"),
	Opposing UMETA(DisplayName = "Opposing"),
	Neutral UMETA(DisplayName = "Neutral"),
	Friendly2     UMETA(DisplayName = "Friendly2"),
	Opposing2 UMETA(DisplayName = "Opposing2"),
	Neutral2 UMETA(DisplayName = "Neutral2"),
	Friendly3    UMETA(DisplayName = "Friendly3"),
	Opposing3 UMETA(DisplayName = "Opposing3"),
	Neutral3 UMETA(DisplayName = "Neutral3"),
	Friendly4     UMETA(DisplayName = "Friendly4"),
	Opposing4 UMETA(DisplayName = "Opposing4"),
	Neutral4 UMETA(DisplayName = "Neutral4"),
	Friendly5     UMETA(DisplayName = "Friendly5"),
	Opposing5 UMETA(DisplayName = "Opposing5"),
	Neutral5 UMETA(DisplayName = "Neutral5"),
	Friendly6     UMETA(DisplayName = "Friendly6"),
	Opposing6 UMETA(DisplayName = "Opposing6"),
	Neutral6 UMETA(DisplayName = "Neutral6"),
	Friendly7     UMETA(DisplayName = "Friendly7"),
	Opposing7 UMETA(DisplayName = "Opposing7"),
	Neutral7 UMETA(DisplayName = "Neutral7"),
	Friendly8     UMETA(DisplayName = "Friendly8"),
	Opposing8 UMETA(DisplayName = "Opposing8"),
	Neutral8 UMETA(DisplayName = "Neutral8"),
	Friendly9     UMETA(DisplayName = "Friendly9"),
	Opposing9 UMETA(DisplayName = "Opposing9"),
	Neutral9 UMETA(DisplayName = "Neutral9"),
	Friendly10     UMETA(DisplayName = "Friendly10"),
	Opposing10 UMETA(DisplayName = "Opposing10"),
	Neutral10 UMETA(DisplayName = "Neutral10")
};

USTRUCT(BlueprintType)
struct FEntityID
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Site;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Application;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Entity;

	FEntityID()
	{
		Site = 0;
		Application = 0;
		Entity = 0;
	}

	bool operator== (const FEntityID other) const
	{
		return Site == other.Site
			&& Application == other.Application
			&& Entity == other.Entity;
	}

	friend uint32 GetTypeHash(const FEntityID& other)
	{
		const FString EntityID = FString::Printf(TEXT("%d:%d:%d"),
			other.Site,
			other.Application,
			other.Entity
		);
		return GetTypeHash(EntityID);
	}

	FString ToString()
	{
		return FString::FromInt(Site) + ":" + FString::FromInt(Application) + ':' + FString::FromInt(Entity);
	}
};

USTRUCT(BlueprintType)
struct FEventID
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Site;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Application;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EventID;

	FEventID()
	{
		Site = 0;
		Application = 0;
		EventID = 0;
	}
};

USTRUCT(BlueprintType)
struct FEntityType
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** Kind of entity */
		int32 EntityKind;

	/** Domain of entity (air, surface, subsurface, space, etc) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Domain;

	/** country to which the design of the entity is attributed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Country;

	/** category of entity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Category;

	/** subcategory of entity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Subcategory;

	/** specific info based on subcategory field */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Specific;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Extra;

	FEntityType()
	{
		EntityKind = -1;
		Domain = -1;
		Country = -1;
		Category = -1;
		Subcategory = -1;
		Specific = -1;
		Extra = -1;
	}

	bool operator== (const FEntityType& Other) const
	{
		return EntityKind == Other.EntityKind
			&& Domain == Other.Domain
			&& Country == Other.Country
			&& Category == Other.Category
			&& Subcategory == Other.Subcategory
			&& Specific == Other.Specific
			&& Extra == Other.Extra;
	}

	friend uint32 GetTypeHash(const FEntityType& other)
	{
		const FString EntityTypeString = FString::Printf(TEXT("%d.%d.%d.%d.%d.%d.%d"),
			other.EntityKind,
			other.Domain,
			other.Country,
			other.Category,
			other.Subcategory,
			other.Specific,
			other.Extra
		);
		return GetTypeHash(EntityTypeString);
	}

	FString ToString()
	{
		return FString::FromInt(EntityKind) + ":" + FString::FromInt(Domain) + ':' + FString::FromInt(Country) + ":" +
			FString::FromInt(Category) + ":" + FString::FromInt(Subcategory) + ':' + FString::FromInt(Specific) + ":" + FString::FromInt(Extra);
	}
};

USTRUCT(BlueprintType)
struct FArticulationParameters
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ParameterTypeDesignator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ChangeIndicator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PartAttachedTo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ParameterType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 ParameterValue;

	FArticulationParameters()
	{
		ParameterTypeDesignator = 0;
		ChangeIndicator = 0;
		PartAttachedTo = 0;
		ParameterType = 0;
		ParameterValue = 0;
	}
};

USTRUCT(BlueprintType)
struct FBurstDescriptor
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityType EntityType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 warhead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 fuse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 quantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 rate;

	FBurstDescriptor()
	{
		warhead = 0;
		fuse = 0;
		quantity = 0;
		rate = 0;
	}
};

USTRUCT(BlueprintType)
struct FDeadReckoningParameters
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 DeadReckoningAlgorithm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<uint8> OtherParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EntityLinearAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EntityAngularVelocity;

	FDeadReckoningParameters()
	{
		DeadReckoningAlgorithm = 0U;
		OtherParameters.Init(0, 15);
		EntityLinearAcceleration = FVector(0, 0, 0);
		EntityAngularVelocity = FVector(0, 0, 0);
	}
};

USTRUCT(BlueprintType)
struct FEntityStatePDU
{
	GENERATED_BODY()

		//checked
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PduType; // TODO: Make this an enum
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID EntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EForceID ForceID;
	UPROPERTY()
		TArray<double> EntityLocationDouble;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EntityLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator EntityOrientation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Marking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EntityLinearVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FDeadReckoningParameters DeadReckoningParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityType EntityType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 NumberOfArticulationParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EntityAppearance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Capabilities;

	//unchecked
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityType AlternativeEntityType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FArticulationParameters ArticulationParameters;

	FEntityStatePDU()
	{
		//checked
		PduType = 1;
		ForceID = EForceID::Other;
		Marking = FString("");
		EntityLocation = FVector(0, 0, 0);
		EntityOrientation = FRotator(0, 0, 0);
		EntityLocationDouble.Init(0, 3);
		EntityLinearVelocity = FVector(0, 0, 0);
		EntityAppearance = 0;
		NumberOfArticulationParameters = 0;
		Capabilities = 0;
	}
};

USTRUCT(BlueprintType)
struct FFirePDU
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PduType; // TODO: Make this an enum
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 fireMissionIndex; // TODO: Make this an enum
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID MunitionEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector velocity;
	UPROPERTY()
		TArray<double> locationDouble;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEventID eventID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBurstDescriptor burstDescriptor;

	FFirePDU()
	{
		PduType = 2;
		fireMissionIndex = 0;
		range = 0.0;
		locationDouble.Init(0, 3);
		location = FVector(0, 0, 0);
		velocity = FVector(0, 0, 0);
	}
};

USTRUCT(BlueprintType)
struct FRemoveEntityPDU
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PduType; // TODO: Make this an enum
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID OriginatingEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID ReceivingEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 RequestID;

	FRemoveEntityPDU()
	{
		PduType = 12;
		RequestID = 0;
	}
};

USTRUCT(BlueprintType)
struct FDetonationPDU
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PduType; // TODO: Make this an enum
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID MunitionEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEventID eventID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector velocity;
	UPROPERTY()
		TArray<double> locationDouble;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBurstDescriptor burstDescriptor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector locationInEntityCoords;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 detonationResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 NumberOfArticulationParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 pad;

	FDetonationPDU()
	{
		PduType = 3;
		velocity = FVector(0, 0, 0);
		locationDouble.Init(0, 3);
		location = FVector(0, 0, 0);
		locationInEntityCoords = FVector(0, 0, 0);
		detonationResult = 0U;
		NumberOfArticulationParameters = 0;
		pad = 0;

	}
};

UCLASS(Blueprintable)
class UEOPENDIS_API AUEOpenDISGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AUEOpenDISGameState();

	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void ProcessDISPacket(int ByteArrayLength, TArray<uint8> InData, int& OutType);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void AddDISEntityToMap(FEntityID EntityIDToAdd, AActor* EntityToAdd);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		bool RemoveDISEntityFromMap(FEntityID EntityIDToRemove);

	UFUNCTION(BlueprintCallable, Category = "OpenDIS|UDP")
		bool OpenReceiveSocket(FString InListenIP, int32 InListenPort);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|UDP")
		bool OpenSendSocket(FString InIP, int32 InPort);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|UDP")
		bool CloseReceiveSocket();
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|UDP")
		bool CloseSendSocket();

	UFUNCTION(BlueprintPure, Category = "OpenDIS|UDP")
		void GetUDPSendInformation(bool& CurrentlyConnected, FString& SendIPAddress, int32& SendPort, bool& AutoConnectSend);
	UFUNCTION(BlueprintPure, Category = "OpenDIS|UDP")
		void GetUDPReceiveInformation(bool& CurrentlyConnected, FString& ReceiveIPAddress, int32& ReceivePort, bool& AutoConnectReceive);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
		TMap<FEntityType, TAssetSubclassOf<AActor>> DISClassMappings;
	UPROPERTY(BlueprintReadOnly)
		TMap<FEntityID, AActor*> DISActorMappings;
	UPROPERTY(BlueprintReadOnly)
		int32 ExerciseID;
	UPROPERTY(BlueprintReadOnly)
		int32 SiteID;
	UPROPERTY(BlueprintReadOnly)
		int32 ApplicationID;

	UPROPERTY(BlueprintReadOnly)
		UUDPComponent* UDPWrapper;

private:
	UOpenDISComponent* GetAssociatedOpenDISComponent(FEntityID EntityIDIn);

	void ConvertESPDU2Bytes(int EntityID, int Site, int Application, int Exercise, FEntityStatePDU EntityStatePDUIn, TArray<uint8>& BytesOut);
	FEntityStatePDU ConvertESPDUtoBPStruct(DIS::EntityStatePdu& EntityStatePDUOut);
	FFirePDU ConvertFirePDUtoBPStruct(DIS::FirePdu& FirePDUOut);
	FDetonationPDU ConvertDetonationPDUtoBPStruct(DIS::DetonationPdu& DetPDUOut);
	FRemoveEntityPDU ConvertRemoveEntityPDUtoBPStruct(DIS::RemoveEntityPdu& RemovePDUOut);

	DIS::Endian BigEndian = DIS::BIG;
	const unsigned int PDU_TYPE_POSITION = 2;
};