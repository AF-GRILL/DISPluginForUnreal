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
#include <dis6/EntityStateUpdatePdu.h>
#include <dis6/StartResumePdu.h>
#include <dis6/StopFreezePdu.h>

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
	Other,
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

UENUM(BlueprintType)
enum class EReason : uint8
{
	Other,
	Recess,
	Termination,
	SystemFailure,
	SecurityViolation,
	EntityReconstruction,
	StopForReset,
	StopForRestart,
	AbortTrainingReturnToTacticalOperations
};

USTRUCT()
struct FEarthCenteredEarthFixedDouble
{
	GENERATED_BODY()

	UPROPERTY()
		double X;
	UPROPERTY()
		double Y;
	UPROPERTY()
		double Z;

	FEarthCenteredEarthFixedDouble()
	{
		X = 0;
		Y = 0;
		Z = 0;
	}

	FEarthCenteredEarthFixedDouble(const double X, const double Y, const double Z)
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}
};

USTRUCT(BlueprintType)
struct FEarthCenteredEarthFixedFloat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Z;

	FEarthCenteredEarthFixedFloat()
	{
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}

	FEarthCenteredEarthFixedFloat(const float X, const float Y, const float Z)
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}
};

USTRUCT()
struct FLatLonHeightDouble
{
	GENERATED_BODY()

	UPROPERTY()
		double Latitude;
	UPROPERTY()
		double Longitude;
	UPROPERTY()
		double Height;

	FLatLonHeightDouble()
	{
		Latitude = 0;
		Longitude = 0;
		Height = 0;
	}

	FLatLonHeightDouble(const double Latitude,const double Longitude, const double Height)
	{
		this->Latitude = Latitude;
		this->Longitude = Longitude;
		this->Height = Height;
	}
};

USTRUCT(BlueprintType)
struct FLatLonHeightFloat 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Latitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Longitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Height;

	FLatLonHeightFloat()
	{
		Latitude = 0.0f;
		Longitude = 0.0f;
		Height = 0.0f;
	}

	FLatLonHeightFloat(const float Latitude, const float Longitude, const float Height)
	{
		this->Latitude = Latitude;
		this->Longitude = Longitude;
		this->Height = Height;
	}
};

USTRUCT(BlueprintType)
struct FEastNorthUp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EastVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector NorthVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector UpVector;

	FEastNorthUp()
	{
		NorthVector = FVector(0, 1, 0);
		EastVector = FVector(1, 0, 0);
		UpVector = FVector(0, 0, 1);
	}

	FEastNorthUp(FVector EastVector, FVector NorthVector, FVector UpVector)
	{
		this->EastVector = EastVector;
		this->NorthVector = NorthVector;
		this->UpVector = UpVector;
	}
};

USTRUCT(BlueprintType)
struct FNorthEastDown
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector NorthVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EastVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector DownVector;

	FNorthEastDown()
	{
		NorthVector = FVector(0, 1, 0);
		EastVector = FVector(1, 0, 0);
		DownVector = FVector(0, 0, -1);
	}

	FNorthEastDown(FVector NorthVector, FVector EastVector, FVector DownVector)
	{
		this->NorthVector = NorthVector;
		this->EastVector = EastVector;
		this->DownVector = DownVector;
	}
};

USTRUCT(BlueprintType)
struct FHeadingPitchRoll
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Heading;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Roll;

	FHeadingPitchRoll()
	{
		Heading = 0.0f;
		Pitch = 0.0f;
		Roll = 0.0f;
	}

	FHeadingPitchRoll(const float Heading, const float Pitch, const float Roll) 
	{
		this->Heading = Heading;
		this->Pitch = Pitch;
		this->Roll = Roll;
	}
};

USTRUCT(BlueprintType)
struct FPsiThetaPhi
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Psi;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Theta;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Phi;

	FPsiThetaPhi() 
	{
		Psi = 0.0f;
		Theta = 0.0f;
		Phi = 0.0f;
	}

	FPsiThetaPhi(const float Psi, const float Theta, const float Phi)
	{
		this->Psi = Psi;
		this->Theta = Theta;
		this->Phi = Phi;
	}
};

USTRUCT(BlueprintType)
struct FWorldOrigin
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLatLonHeightFloat WorldOriginLLH;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FNorthEastDown WorldOriginNED;
};

USTRUCT(BlueprintType)
struct FClockTime
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Hour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 TimePastHour;

	FClockTime()
	{
		Hour = 0;
		TimePastHour = 0;
	}
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
	/** The location of the entity in ECEF - double (64-bit) precision */
	UPROPERTY()
		TArray<double> EntityLocationDouble;
	/** The location of the entity in ECEF - floating point (32-bit) precision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EntityLocation;
	/** The orientation of the entity in Psi (Yaw), Theta (Pitch), Phi (Roll) - in radians */
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
struct FEntityStateUpdatePDU
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPDUType PduType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID EntityID;
	/** The location of the entity in ECEF - double (64-bit) precision */
	UPROPERTY()
		TArray<double> EntityLocationDouble;
	/** The location of the entity in ECEF - floating point (32-bit) precision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EntityLocation;
	/** The orientation of the entity in Psi (Yaw), Theta (Pitch), Phi (Roll) - in radians */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator EntityOrientation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EntityLinearVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 NumberOfArticulationParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EntityAppearance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Padding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Padding1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FArticulationParameters ArticulationParameters;

	FEntityStateUpdatePDU()
	{
		//checked
		PduType = EPDUType::EntityStateUpdate;
		EntityLocation = FVector(0, 0, 0);
		EntityOrientation = FRotator(0, 0, 0);
		EntityLocationDouble.Init(0, 3);
		EntityLinearVelocity = FVector(0, 0, 0);
		EntityAppearance = 0;
		NumberOfArticulationParameters = 0;
	}

	operator FEntityStatePDU() const 
	{
		FEntityStatePDU newEntityStatePDU;

		newEntityStatePDU.EntityID = EntityID;
		newEntityStatePDU.EntityLocationDouble = EntityLocationDouble;
		newEntityStatePDU.EntityLocation = EntityLocation;
		newEntityStatePDU.EntityOrientation = EntityOrientation;
		newEntityStatePDU.EntityLinearVelocity = EntityLinearVelocity;
		newEntityStatePDU.NumberOfArticulationParameters = NumberOfArticulationParameters;
		newEntityStatePDU.EntityAppearance = EntityAppearance;
		newEntityStatePDU.ArticulationParameters = ArticulationParameters;

		return newEntityStatePDU;
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

USTRUCT(BlueprintType)
struct FStartResumePDU
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FClockTime RealWorldTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FClockTime SimulationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 RequestID;

	FStartResumePDU() 
	{
		RequestID = 0;
	}
};

USTRUCT(BlueprintType)
struct FStopFreezePDU
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FClockTime RealWorldTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EReason Reason;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 FrozenBehavior;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Padding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 RequestID;

	FStopFreezePDU() 
	{
		Reason = EReason::Other;
		FrozenBehavior = 0;
		RequestID = 0;
	}
};