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

#include "Kismet/KismetStringLibrary.h"
#include "CoreMinimal.h"
#include "DISEnumsAndStructs.generated.h"

UENUM(BlueprintType)
enum class EDISCullingMode : uint8
{
	None,
	CullDeadReckoning,
	CullAll
};

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

	DIS::ClockTime ToOpenDIS() const
	{
		DIS::ClockTime OutClockTime;

		OutClockTime.setHour(Hour);
		OutClockTime.setTimePastHour(TimePastHour);

		return OutClockTime;
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

	FEntityID(DIS::EntityID EntityID)
	{
		this->Site = EntityID.getSite();
		this->Application = EntityID.getApplication();
		this->Entity = EntityID.getEntity();
	}

	bool operator== (const FEntityID other) const
	{
		return Site == other.Site
			&& Application == other.Application
			&& Entity == other.Entity;
	}

	bool operator!= (const FEntityID Other) const
	{
		return !(operator==(Other));
	}

	bool operator< (const FEntityID Other) const
	{
		return ToUInt64() < Other.ToUInt64();
	}

	bool operator> (const FEntityID Other) const
	{
		return Other.operator<(*this);
	}

	bool operator<= (const FEntityID Other) const
	{
		return !(operator>(Other));
	}

	bool operator>= (const FEntityID Other) const
	{
		return !(operator<(Other));
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

	DIS::EntityID ToOpenDIS() const
	{
		DIS::EntityID OutID;
		OutID.setSite(Site);
		OutID.setApplication(Application);
		OutID.setEntity(Entity);
		return OutID;
	}

	uint64 ToUInt64() const
	{
		return (static_cast<uint64>(Site)<< 32) | (static_cast<uint64>(Application)<< 16) | (static_cast<uint64>(Entity));
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
		int32 EventNumber;

	FEventID()
	{
		Site = 0;
		Application = 0;
		EventNumber = 0;
	}

	FEventID(DIS::EventID EventID)
	{
		this->Site = EventID.getSite();
		this->Application = EventID.getApplication();
		this->EventNumber = EventID.getEventNumber();
	}

	DIS::EventID ToOpenDIS() const
	{
		DIS::EventID OutID;
		OutID.setSite(Site);
		OutID.setApplication(Application);
		OutID.setEventNumber(EventNumber);
		return OutID;
	}
};

USTRUCT(BlueprintType)
struct FEntityType
{
	GENERATED_BODY()

	/** Kind of entity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)		
		int32 EntityKind;
	/** Domain of entity (air, surface, subsurface, space, etc) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Domain;
	/** Country to which the design of the entity is attributed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Country;
	/** Category of entity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Category;
	/** Subcategory of entity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Subcategory;
	/** Specific info based on subcategory field */
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

	FEntityType(DIS::EntityType EntityType)
	{
		this->EntityKind = EntityType.getEntityKind();
		this->Domain = EntityType.getDomain();
		this->Country = EntityType.getCountry();
		this->Category = EntityType.getCategory();
		this->Subcategory = EntityType.getSubcategory();
		this->Specific = EntityType.getSpecific();
		this->Extra = EntityType.getExtra();
	}

	FEntityType FillWildcards(const FEntityType Other)
	{
		if (EntityKind == -1)
		{
			EntityKind = Other.EntityKind;
		}
		if (Domain == -1)
		{
			Domain = Other.Domain;
		}
		if (Country == -1)
		{
			Country = Other.Country;
		}
		if (Category == -1)
		{
			Category = Other.Category;
		}
		if (Subcategory == -1)
		{
			Subcategory = Other.Subcategory;
		}
		if (Specific == -1)
		{
			Specific = Other.Specific;
		}
		if (Extra == -1)
		{
			Extra = Other.Extra;
		}

		return FEntityType(*this);
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

	bool operator!= (const FEntityType& Other) const
	{
		return !(operator==(Other));
	}

	bool operator< (const FEntityType& Other) const
	{
		const bool bIsLessThan = ToUInt64() < Other.ToUInt64();
		return bIsLessThan;
	}

	bool operator> (const FEntityType& Other) const
	{
		return Other.operator<(*this);
	}

	bool operator<= (const FEntityType& Other) const
	{
		return !(operator>(Other));
	}

	bool operator>= (const FEntityType& Other) const
	{
		return !(operator<(Other));
	}

	friend uint32 GetTypeHash(const FEntityType& Other)
	{
		const FString EntityTypeString = FString::Printf(TEXT("%d.%d.%d.%d.%d.%d.%d"),
			Other.EntityKind,
			Other.Domain,
			Other.Country,
			Other.Category,
			Other.Subcategory,
			Other.Specific,
			Other.Extra
		);
		return GetTypeHash(EntityTypeString);
	}

	FString ToString() const
	{
		return FString::FromInt(EntityKind) + ":" + FString::FromInt(Domain) + ':' + FString::FromInt(Country) + ":" +
			FString::FromInt(Category) + ":" + FString::FromInt(Subcategory) + ':' + FString::FromInt(Specific) + ":" + FString::FromInt(Extra);
	}

	uint64 ToUInt64() const
	{
		const uint64 BitString = ((static_cast<uint64>(Extra) & 0xFF) << 0) | ((static_cast<uint64>(Specific) & 0xFF) << 8) | ((static_cast<uint64>(Subcategory) & 0xFF) << 16) |
			((static_cast<uint64>(Category) & 0xFF) << 24) | ((static_cast<uint64>(Country) & 0xFF) << 32) | ((static_cast<uint64>(Domain) & 0xFF) << 48) | ((static_cast<uint64>(EntityKind) & 0xFF) << 56);
		
		return BitString;
	}

	FString ToBitString() const
	{
		uint64 BitString = ToUInt64();
		return BytesToHex(reinterpret_cast<uint8*>(&BitString), 8);
	}

	DIS::EntityType ToOpenDIS() const
	{
		if (*this == FEntityType())
		{
			DIS::EntityType OutType;
			OutType.setEntityKind(0);
			OutType.setDomain(0);
			OutType.setCountry(0);
			OutType.setCategory(0);
			OutType.setSubcategory(0);
			OutType.setSpecific(0);
			OutType.setExtra(0);
			return OutType;
		}
		else
		{
			DIS::EntityType OutType;
			OutType.setEntityKind(EntityKind);
			OutType.setDomain(Domain);
			OutType.setCountry(Country);
			OutType.setCategory(Category);
			OutType.setSubcategory(Subcategory);
			OutType.setSpecific(Specific);
			OutType.setExtra(Extra);
			return OutType;
		}
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

	DIS::ArticulationParameter ToOpenDIS() const
	{
		DIS::ArticulationParameter OutParam;
		OutParam.setParameterTypeDesignator(ParameterTypeDesignator);
		OutParam.setChangeIndicator(ChangeIndicator);
		OutParam.setPartAttachedTo(PartAttachedTo);
		OutParam.setParameterType(ParameterType);
		OutParam.setParameterValue(ParameterValue);
		return OutParam;
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

	DIS::BurstDescriptor ToOpenDIS() const
	{
		DIS::BurstDescriptor OutDescriptor;
		OutDescriptor.setMunition(EntityType.ToOpenDIS());
		OutDescriptor.setWarhead(Warhead);
		OutDescriptor.setFuse(Fuse);
		OutDescriptor.setQuantity(Quantity);
		OutDescriptor.setRate(Rate);
		return OutDescriptor;
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

	DIS::DeadReckoningParameter ToOpenDIS() const
	{
		DIS::DeadReckoningParameter OutParam;
		OutParam.setDeadReckoningAlgorithm(DeadReckoningAlgorithm);
		OutParam.setOtherParameters(reinterpret_cast<const char*>(OtherParameters.GetData()));
		DIS::Vector3Float OutLinearAcceleration;
		OutLinearAcceleration.setX(EntityLinearAcceleration.X);
		OutLinearAcceleration.setY(EntityLinearAcceleration.Y);
		OutLinearAcceleration.setZ(EntityLinearAcceleration.Z);
		OutParam.setEntityLinearAcceleration(OutLinearAcceleration);
		DIS::Vector3Float OutAngularVelocity;
		OutAngularVelocity.setX(EntityAngularVelocity.X);
		OutAngularVelocity.setY(EntityAngularVelocity.Y);
		OutAngularVelocity.setZ(EntityAngularVelocity.Z);
		OutParam.setEntityAngularVelocity(OutAngularVelocity);
		return OutParam;
	}
};