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

	DIS::ClockTime ToDIS() const
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

	DIS::EntityID ToDIS() const
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

	DIS::EventID ToDIS() const
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
		return ToUInt64() < Other.ToUInt64();
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
		const uint64 BitString = (static_cast<uint64>(Extra) << 56) | (static_cast<uint64>(Specific) << 48) | (static_cast<uint64>(Subcategory) << 40) | 
			(static_cast<uint64>(Category) << 32) | (static_cast<uint64>(Country) << 16) | (static_cast<uint64>(Domain) << 8) | static_cast<uint64>(EntityKind);
		return BitString;
	}

	FString ToBitString() const
	{
		const uint64 BitString = ToUInt64();
		const uint32 UpperBitString = static_cast<uint32>(BitString >> 32);
		const uint32 LowerBitString = static_cast<uint32>(BitString & 0xFFFF);
		return FString::FromInt(UpperBitString) + FString::FromInt(LowerBitString);
	}

	DIS::EntityType ToDIS() const
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

	DIS::ArticulationParameter ToDIS() const
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

	DIS::BurstDescriptor ToDIS() const
	{
		DIS::BurstDescriptor OutDescriptor;
		OutDescriptor.setMunition(EntityType.ToDIS());
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

	DIS::DeadReckoningParameter ToDIS() const
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

USTRUCT(BlueprintType)
struct FPdu
{
	GENERATED_BODY()

	/** The version of the protocol. 5=DIS-1995, 6=DIS-1998. */
	unsigned char ProtocolVersion;

	unsigned char ExerciseID;

	/** Type of pdu, unique for each PDU struct */
	EPDUType PduType;

	/** Value that refers to the protocol family, e.g. SimulationManagement, etc. */
	unsigned char ProtocolFamily;

	unsigned int Timestamp;

	/** Length, in bytes, of the PDU */
	unsigned short Length;

	/** Zero-filled array of padding */
	short Padding;

	FPdu()
	{
		ProtocolVersion = 6;
		ExerciseID = 0;
		PduType = EPDUType::Other;
		ProtocolFamily = 0;
		Timestamp = 0;
		Length = 0;
		Padding = 0;
	}
};

USTRUCT(BlueprintType)
struct FWarfareFamilyPdu : public FPdu
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID FiringEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID TargetEntityID;
};

USTRUCT(BlueprintType)
struct FSimulationManagementFamilyPdu : public FPdu
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID OriginatingEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID ReceivingEntityID;
};

USTRUCT(BlueprintType)
struct FEntityInformationFamilyPdu : public FPdu
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FEntityStatePDU : public FEntityInformationFamilyPdu
{
	GENERATED_BODY()

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
		int32 EntityAppearance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Capabilities;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityType AlternativeEntityType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FArticulationParameters> ArticulationParameters;

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
		Capabilities = 0;
	}

	DIS::EntityStatePdu ToDIS() const
	{
		DIS::EntityStatePdu OutPdu;

		// Common PDU setup
		OutPdu.setProtocolVersion(ProtocolVersion);
		OutPdu.setExerciseID(ExerciseID);
		OutPdu.setPduType(static_cast<unsigned char>(PduType));
		OutPdu.setProtocolFamily(ProtocolFamily);
		OutPdu.setTimestamp(Timestamp);
		OutPdu.setLength(Length);
		OutPdu.setPadding(Padding);

		// Specific PDU setup
		OutPdu.setEntityID(EntityID.ToDIS());
		OutPdu.setForceId(static_cast<unsigned char>(ForceID));
		OutPdu.setEntityType(EntityType.ToDIS());
		OutPdu.setAlternativeEntityType(AlternativeEntityType.ToDIS());
		DIS::Vector3Float OutLinearVelocity;
		OutLinearVelocity.setX(EntityLinearVelocity.X);
		OutLinearVelocity.setY(EntityLinearVelocity.Y);
		OutLinearVelocity.setZ(EntityLinearVelocity.Z);
		OutPdu.setEntityLinearVelocity(OutLinearVelocity);
		DIS::Vector3Double OutLocation;
		if (FMath::IsNearlyEqual(static_cast<float>(EntityLocationDouble[0]), EntityLocation.X) &&
			FMath::IsNearlyEqual(static_cast<float>(EntityLocationDouble[1]), EntityLocation.Y) &&
			FMath::IsNearlyEqual(static_cast<float>(EntityLocationDouble[2]), EntityLocation.Z))
		{
			OutLocation.setX(EntityLocationDouble[0]);
			OutLocation.setY(EntityLocationDouble[1]);
			OutLocation.setZ(EntityLocationDouble[2]);
		}
		else
		{
			OutLocation.setX(EntityLocation.X);
			OutLocation.setY(EntityLocation.Y);
			OutLocation.setZ(EntityLocation.Z);
		}
		OutPdu.setEntityLocation(OutLocation);
		DIS::Orientation OutOrientation;
		OutOrientation.setPsi(EntityOrientation.Yaw);
		OutOrientation.setTheta(EntityOrientation.Pitch);
		OutOrientation.setPhi(EntityOrientation.Roll);
		OutPdu.setEntityOrientation(OutOrientation);
		OutPdu.setEntityAppearance(EntityAppearance);
		OutPdu.setDeadReckoningParameters(DeadReckoningParameters.ToDIS());
		DIS::Marking OutMarking;
		OutMarking.setCharacterSet(1);
		OutMarking.setByStringCharacters(TCHAR_TO_ANSI(*Marking.Left(11)));
		OutPdu.setMarking(OutMarking);
		OutPdu.setCapabilities(Capabilities);
		std::vector<DIS::ArticulationParameter> OutArtParams;
		for (FArticulationParameters Param : ArticulationParameters)
		{
			OutArtParams.push_back(Param.ToDIS());
		}
		OutPdu.setArticulationParameters(OutArtParams);
		return OutPdu;
	}
};

USTRUCT(BlueprintType)
struct FEntityStateUpdatePDU : public FEntityInformationFamilyPdu
{
	GENERATED_BODY()

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
		int32 EntityAppearance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Padding1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FArticulationParameters> ArticulationParameters;

	FEntityStateUpdatePDU()
	{
		//checked
		PduType = EPDUType::EntityStateUpdate;
		EntityLocation = FVector(0, 0, 0);
		EntityOrientation = FRotator(0, 0, 0);
		EntityLocationDouble.Init(0, 3);
		EntityLinearVelocity = FVector(0, 0, 0);
		EntityAppearance = 0;
		Padding = 0;
		Padding1 = 0;
	}

	operator FEntityStatePDU() const 
	{
		FEntityStatePDU newEntityStatePDU;

		newEntityStatePDU.EntityID = EntityID;
		newEntityStatePDU.EntityLocationDouble = EntityLocationDouble;
		newEntityStatePDU.EntityLocation = EntityLocation;
		newEntityStatePDU.EntityOrientation = EntityOrientation;
		newEntityStatePDU.EntityLinearVelocity = EntityLinearVelocity;
		newEntityStatePDU.EntityAppearance = EntityAppearance;
		newEntityStatePDU.ArticulationParameters = ArticulationParameters;

		return newEntityStatePDU;
	}

	DIS::EntityStateUpdatePdu ToDIS() const
	{
		DIS::EntityStateUpdatePdu OutPdu;

		// Common PDU setup
		OutPdu.setProtocolVersion(ProtocolVersion);
		OutPdu.setExerciseID(ExerciseID);
		OutPdu.setPduType(static_cast<unsigned char>(PduType));
		OutPdu.setProtocolFamily(ProtocolFamily);
		OutPdu.setTimestamp(Timestamp);
		OutPdu.setLength(Length);
		OutPdu.setPadding(Padding);

		// Specific PDU setup
		OutPdu.setEntityID(EntityID.ToDIS());
		OutPdu.setPadding1(Padding1);
		DIS::Vector3Float OutLinearVelocity;
		OutLinearVelocity.setX(EntityLinearVelocity.X);
		OutLinearVelocity.setY(EntityLinearVelocity.Y);
		OutLinearVelocity.setZ(EntityLinearVelocity.Z);
		OutPdu.setEntityLinearVelocity(OutLinearVelocity);
		DIS::Vector3Double OutLocation;
		if (FMath::IsNearlyEqual(static_cast<float>(EntityLocationDouble[0]), EntityLocation.X) &&
			FMath::IsNearlyEqual(static_cast<float>(EntityLocationDouble[1]), EntityLocation.Y) &&
			FMath::IsNearlyEqual(static_cast<float>(EntityLocationDouble[2]), EntityLocation.Z))
		{
			OutLocation.setX(EntityLocationDouble[0]);
			OutLocation.setY(EntityLocationDouble[1]);
			OutLocation.setZ(EntityLocationDouble[2]);
		}
		else
		{
			OutLocation.setX(EntityLocation.X);
			OutLocation.setY(EntityLocation.Y);
			OutLocation.setZ(EntityLocation.Z);
		}
		OutPdu.setEntityLocation(OutLocation);
		DIS::Orientation OutOrientation;
		OutOrientation.setPsi(EntityOrientation.Yaw);
		OutOrientation.setTheta(EntityOrientation.Pitch);
		OutOrientation.setPhi(EntityOrientation.Roll);
		OutPdu.setEntityOrientation(OutOrientation);
		OutPdu.setEntityAppearance(EntityAppearance);
		std::vector<DIS::ArticulationParameter> OutArtParams;
		for (FArticulationParameters ArticulationParameter : ArticulationParameters)
		{
			OutArtParams.push_back(ArticulationParameter.ToDIS());
		}
		OutPdu.setArticulationParameters(OutArtParams);
		return OutPdu;
	}
};

USTRUCT(BlueprintType)
struct FFirePDU : public FWarfareFamilyPdu
{
	GENERATED_BODY()

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

	DIS::FirePdu ToDIS() const
	{
		DIS::FirePdu OutPdu;

		// Common PDU setup
		OutPdu.setProtocolVersion(ProtocolVersion);
		OutPdu.setExerciseID(ExerciseID);
		OutPdu.setPduType(static_cast<unsigned char>(PduType));
		OutPdu.setProtocolFamily(ProtocolFamily);
		OutPdu.setTimestamp(Timestamp);
		OutPdu.setLength(Length);
		OutPdu.setPadding(Padding);

		// Warfare Family PDU setup
		OutPdu.setFiringEntityID(FiringEntityID.ToDIS());
		OutPdu.setTargetEntityID(TargetEntityID.ToDIS());

		// Specific PDU setup
		OutPdu.setMunitionID(MunitionEntityID.ToDIS());
		OutPdu.setFireMissionIndex(FireMissionIndex);
		OutPdu.setRange(Range);
		DIS::Vector3Float OutVelocity;
		OutVelocity.setX(Velocity.X);
		OutVelocity.setY(Velocity.Y);
		OutVelocity.setZ(Velocity.Z);
		DIS::Vector3Double OutLocation;
		if (FMath::IsNearlyEqual(static_cast<float>(LocationDouble[0]), Location.X) &&
			FMath::IsNearlyEqual(static_cast<float>(LocationDouble[1]), Location.Y) &&
			FMath::IsNearlyEqual(static_cast<float>(LocationDouble[2]), Location.Z))
		{
			OutLocation.setX(LocationDouble[0]);
			OutLocation.setY(LocationDouble[1]);
			OutLocation.setZ(LocationDouble[2]);
		}
		else
		{
			OutLocation.setX(Location.X);
			OutLocation.setY(Location.Y);
			OutLocation.setZ(Location.Z);
		}
		OutPdu.setLocationInWorldCoordinates(OutLocation);
		OutPdu.setEventID(EventID.ToDIS());
		OutPdu.setBurstDescriptor(BurstDescriptor.ToDIS());
		OutPdu.setFiringEntityID(FiringEntityID.ToDIS());
		OutPdu.setTargetEntityID(TargetEntityID.ToDIS());
		return OutPdu;
	}
};

USTRUCT(BlueprintType)
struct FRemoveEntityPDU : public FSimulationManagementFamilyPdu
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 RequestID;

	FRemoveEntityPDU()
	{
		PduType = EPDUType::RemoveEntity;
		RequestID = 0;
	}

	DIS::RemoveEntityPdu ToDIS() const
	{
		DIS::RemoveEntityPdu OutPdu;

		// Common PDU setup
		OutPdu.setProtocolVersion(ProtocolVersion);
		OutPdu.setExerciseID(ExerciseID);
		OutPdu.setPduType(static_cast<unsigned char>(PduType));
		OutPdu.setProtocolFamily(ProtocolFamily);
		OutPdu.setTimestamp(Timestamp);
		OutPdu.setLength(Length);
		OutPdu.setPadding(Padding);

		// Simulation Management Family PDU setup
		OutPdu.setOriginatingEntityID(OriginatingEntityID.ToDIS());
		OutPdu.setReceivingEntityID(ReceivingEntityID.ToDIS());

		// Remove entity specific PDU setup
		OutPdu.setOriginatingEntityID(OriginatingEntityID.ToDIS());
		OutPdu.setReceivingEntityID(ReceivingEntityID.ToDIS());
		OutPdu.setRequestID(RequestID);

		return OutPdu;
	}
};

USTRUCT(BlueprintType)
struct FDetonationPDU : public FWarfareFamilyPdu
{
	GENERATED_BODY()

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
		int32 Pad;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FArticulationParameters> ArticulationParameters;

	FDetonationPDU()
	{
		PduType = EPDUType::Detonation;
		Velocity = FVector(0, 0, 0);
		LocationDouble.Init(0, 3);
		Location = FVector(0, 0, 0);
		LocationInEntityCoords = FVector(0, 0, 0);
		DetonationResult = 0U;
		Pad = 0;
	}

	DIS::DetonationPdu ToDIS() const
	{
		DIS::DetonationPdu OutPdu;

		// Common PDU setup
		OutPdu.setProtocolVersion(ProtocolVersion);
		OutPdu.setExerciseID(ExerciseID);
		OutPdu.setPduType(static_cast<unsigned char>(PduType));
		OutPdu.setProtocolFamily(ProtocolFamily);
		OutPdu.setTimestamp(Timestamp);
		OutPdu.setLength(Length);
		OutPdu.setPadding(Padding);

		// Warfare Family PDU setup
		OutPdu.setFiringEntityID(FiringEntityID.ToDIS());
		OutPdu.setTargetEntityID(TargetEntityID.ToDIS());

		// Specific PDU setup
		OutPdu.setEventID(EventID.ToDIS());
		OutPdu.setMunitionID(MunitionEntityID.ToDIS());
		DIS::Vector3Float OutVelocity;
		OutVelocity.setX(Velocity.X);
		OutVelocity.setY(Velocity.Y);
		OutVelocity.setZ(Velocity.Z);
		OutPdu.setVelocity(OutVelocity);
		DIS::Vector3Double OutLocation;
		if (FMath::IsNearlyEqual(static_cast<float>(LocationDouble[0]), Location.X) &&
			FMath::IsNearlyEqual(static_cast<float>(LocationDouble[1]), Location.Y) &&
			FMath::IsNearlyEqual(static_cast<float>(LocationDouble[2]), Location.Z))
		{
			OutLocation.setX(LocationDouble[0]);
			OutLocation.setY(LocationDouble[1]);
			OutLocation.setZ(LocationDouble[2]);
		}
		else
		{
			OutLocation.setX(Location.X);
			OutLocation.setY(Location.Y);
			OutLocation.setZ(Location.Z);
		}
		OutPdu.setLocationInWorldCoordinates(OutLocation);
		DIS::Vector3Float OutLocationInEntityCoords;
		OutLocationInEntityCoords.setX(LocationInEntityCoords.X);
		OutLocationInEntityCoords.setY(LocationInEntityCoords.Y);
		OutLocationInEntityCoords.setZ(LocationInEntityCoords.Z);
		OutPdu.setLocationInEntityCoordinates(OutLocationInEntityCoords);
		OutPdu.setDetonationResult(DetonationResult);
		OutPdu.setPad(Pad);
		OutPdu.setFiringEntityID(FiringEntityID.ToDIS());
		OutPdu.setTargetEntityID(TargetEntityID.ToDIS());
		std::vector<DIS::ArticulationParameter> OutArtParams;
		for (FArticulationParameters ArticulationParameter : ArticulationParameters)
		{
			OutArtParams.push_back(ArticulationParameter.ToDIS());
		}
		OutPdu.setArticulationParameters(OutArtParams);

		return OutPdu;
	}
};

USTRUCT(BlueprintType)
struct FStartResumePDU : public FSimulationManagementFamilyPdu
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
		PduType = EPDUType::Start_Resume;
		RequestID = 0;
	}

	DIS::StartResumePdu ToDIS() const
	{
		DIS::StartResumePdu OutPdu;

		// Common PDU setup
		OutPdu.setProtocolVersion(ProtocolVersion);
		OutPdu.setExerciseID(ExerciseID);
		OutPdu.setPduType(static_cast<unsigned char>(PduType));
		OutPdu.setProtocolFamily(ProtocolFamily);
		OutPdu.setTimestamp(Timestamp);
		OutPdu.setLength(Length);
		OutPdu.setPadding(Padding);

		// Simulation Management Family PDU setup
		OutPdu.setOriginatingEntityID(OriginatingEntityID.ToDIS());
		OutPdu.setReceivingEntityID(ReceivingEntityID.ToDIS());

		// Specific PDU setup
		OutPdu.setReceivingEntityID(ReceivingEntityID.ToDIS());
		OutPdu.setOriginatingEntityID(OriginatingEntityID.ToDIS());
		OutPdu.setRealWorldTime(RealWorldTime.ToDIS());
		OutPdu.setSimulationTime(SimulationTime.ToDIS());
		OutPdu.setRequestID(RequestID);

		return OutPdu;
	}
};

USTRUCT(BlueprintType)
struct FStopFreezePDU : public FSimulationManagementFamilyPdu
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FClockTime RealWorldTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EReason Reason;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 FrozenBehavior;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PaddingOne;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 RequestID;

	FStopFreezePDU() 
	{
		PduType = EPDUType::Stop_Freeze;
		Reason = EReason::Other;
		FrozenBehavior = 0;
		PaddingOne = 0;
		RequestID = 0;
	}

	DIS::StopFreezePdu ToDIS() const
	{
		DIS::StopFreezePdu OutPdu;

		// Common PDU setup
		OutPdu.setProtocolVersion(ProtocolVersion);
		OutPdu.setExerciseID(ExerciseID);
		OutPdu.setPduType(static_cast<unsigned char>(PduType));
		OutPdu.setProtocolFamily(ProtocolFamily);
		OutPdu.setTimestamp(Timestamp);
		OutPdu.setLength(Length);
		OutPdu.setPadding(Padding);

		// Simulation Management Family PDU setup
		OutPdu.setOriginatingEntityID(OriginatingEntityID.ToDIS());
		OutPdu.setReceivingEntityID(ReceivingEntityID.ToDIS());

		// Specific PDU setup
		OutPdu.setReceivingEntityID(ReceivingEntityID.ToDIS());
		OutPdu.setOriginatingEntityID(OriginatingEntityID.ToDIS());
		OutPdu.setRealWorldTime(RealWorldTime.ToDIS());
		OutPdu.setReason(static_cast<unsigned char>(Reason));
		OutPdu.setFrozenBehavior(FrozenBehavior);
		OutPdu.setPadding1(PaddingOne);
		OutPdu.setRequestID(RequestID);

		return OutPdu;
	}
};