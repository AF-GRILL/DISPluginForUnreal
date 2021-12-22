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

#include "CoreMinimal.h"
#include "DISEnumsAndStructs.generated.h"

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

UENUM(BlueprintType)
enum class EPDUType : uint8
{
	None,
	EntityState,
	Fire,
	Detonation,
	Collision,
	ServiceRequest,
	ResupplyOffer,
	RessuplyReceived,
	ResupplyCancel,
	RepairComplete,
	RepairResponse,
	CreateEntity,
	RemoveEntity,
	Start_Resume,
	Stop_Freeze,
	Acknowledge,
	ActionRequest,
	ActionResponse,
	DataQuery,
	SetData,
	Data,
	EventReport,
	Comment,
	ElectromagneticEmission,
	Designator,
	Transmitter,
	Signal,
	Receiver,
	IFF,
	UnderwaterAcoustic,
	SupplementalEmission_EntityState,
	IntercomSignal,
	IntercomControl,
	AggregateState,
	IsGroupOf,
	TransferOwnership,
	IsPartOf,
	MinefieldState,
	MinefieldQuery,
	MinefieldData,
	MinefieldResponseNegativeAcknowledgement,
	EnvironmentalProcess,
	GriddedData,
	PointObjectState,
	LinearObjectState,
	ArealObjectState,
	TimeSpacePositionInformation,
	Appearance,
	ArticulatedParts,
	LEFire,
	LEDetonate,
	CreateEntity_R,
	RemoveEntity_R,
	Start_Resume_R,
	Stop_Freeze_R,
	Acknowledge_R,
	ActionRequest_R,
	ActionResponse_R,
	DataQuery_R,
	SetData_R,
	Data_R,
	EventReport_R,
	Comment_R_Message,
	Record_R,
	SetRecord_R,
	RecordQuery_R,
	Collision_Elastic,
	EntityStateUpdate,
	DirectedEnergyFire,
	EntityDamageStatus,
	InformationOperationsAction,
	InformationOperationsReport,
	Attribute
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
		int32 Warhead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Fuse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Quantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Rate;

	FBurstDescriptor()
	{
		Warhead = 0;
		Fuse = 0;
		Quantity = 0;
		Rate = 0;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPDUType PduType;
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
		PduType = EPDUType::EntityState;
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
		EPDUType PduType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 FireMissionIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID MunitionEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Velocity;
	UPROPERTY()
		TArray<double> LocationDouble;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEventID EventID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBurstDescriptor BurstDescriptor;

	FFirePDU()
	{
		PduType = EPDUType::Fire;
		FireMissionIndex = 0;
		Range = 0.0;
		LocationDouble.Init(0, 3);
		Location = FVector(0, 0, 0);
		Velocity = FVector(0, 0, 0);
	}
};

USTRUCT(BlueprintType)
struct FRemoveEntityPDU
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPDUType PduType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID OriginatingEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID ReceivingEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 RequestID;

	FRemoveEntityPDU()
	{
		PduType = EPDUType::RemoveEntity;
		RequestID = 0;
	}
};

USTRUCT(BlueprintType)
struct FDetonationPDU
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPDUType PduType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID MunitionEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEventID EventID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Velocity;
	UPROPERTY()
		TArray<double> LocationDouble;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBurstDescriptor BurstDescriptor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector LocationInEntityCoords;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 DetonationResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 NumberOfArticulationParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Pad;

	FDetonationPDU()
	{
		PduType = EPDUType::Detonation;
		Velocity = FVector(0, 0, 0);
		LocationDouble.Init(0, 3);
		Location = FVector(0, 0, 0);
		LocationInEntityCoords = FVector(0, 0, 0);
		DetonationResult = 0U;
		NumberOfArticulationParameters = 0;
		Pad = 0;
	}
};