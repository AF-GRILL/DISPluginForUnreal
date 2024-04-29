// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once
//dis headers
#include <dis6/Pdu.h>
#include <dis6/utils/PDUBank.h>
#include <dis6/utils/DataStream.h>

//PDU type headers, add as more pdu types are supported!
#include <dis6/EntityStatePdu.h>   
#include <dis6/FirePdu.h>
#include <dis6/DetonationPdu.h>
#include <dis6/RemoveEntityPdu.h>
#include <dis6/EntityStateUpdatePdu.h>
#include <dis6/StartResumePdu.h>
#include <dis6/StopFreezePdu.h>
#include <dis6/BeamData.h>
#include <dis6/FundamentalParameterData.h>
#include <dis6/TrackJamTarget.h>
#include <dis6/EmitterSystem.h>
#include <dis6/ElectromagneticEmissionSystemData.h> 
#include <dis6/ElectromagneticEmissionBeamData.h> 

#include "EnumsAndStructs/Enums/EmitterName.h"
#include "EnumsAndStructs/Enums/FuseType.h"
#include "EnumsAndStructs/Enums/VariableRecordTypes.h"
#include "EnumsAndStructs/Enums/WarheadType.h"
#include "Kismet/KismetStringLibrary.h"
#include "CoreMinimal.h"
#include "DISEnumsAndStructs.generated.h"

UENUM(BlueprintType)
enum class EEntityStateSendingMode : uint8
{
	None						UMETA(Tooltip = "Don't send out any automatic Entity State PDU updates."),
	EntityStatePDU				UMETA(Tooltip = "Send out automatic Entity State PDU updates."),
	EntityStateUpdatePDU		UMETA(Tooltip = "Send out automatic Entity State Update PDU updates.")
};

UENUM(BlueprintType)
enum class EDISCullingMode : uint8
{
	None						UMETA(Tooltip = "Don't cull any DIS updates."),
	CullDeadReckoning			UMETA(Tooltip = "Cull only dead reckoning updates."),
	CullAll						UMETA(Tooltip = "Cull all DIS updates. Currently only performs dead reckoning culling.")
};

UENUM(BlueprintType)
enum class EGroundClampingMode : uint8
{
	None						UMETA(Tooltip = "Never ground clamp this entity."),
	GroundClampWithDISOptions	UMETA(Tooltip = "Ground clamp this entity if it is of the ground domain and not a munition."),
	AlwaysGroundClamp			UMETA(Tooltip = "Always ground clamp this entity.")
};

UENUM(BlueprintType)
enum class EDeadReckoningAlgorithm : uint8
{
	Other		UMETA(Tooltip = "Using a custom DR algorithm."),
	Static		UMETA(Tooltip = "Static. Entity does not move."),
	FPW			UMETA(Tooltip = "Fixed Position World"),
	RPW			UMETA(Tooltip = "Rotation Position World"),
	RVW			UMETA(Tooltip = "Rotation Velocity World"),
	FVW			UMETA(Tooltip = "Fixed Velocity World"),
	FPB			UMETA(Tooltip = "Fixed Position Body"),
	RPB			UMETA(Tooltip = "Rotation Position Body"),
	RVB			UMETA(Tooltip = "Rotation Velocity Body"),
	FVB			UMETA(Tooltip = "Fixed Velocity Body")
};

UENUM(BlueprintType)
enum class EForceID : uint8
{
	Other		UMETA(DisplayName = "Other"),
	Friendly	UMETA(DisplayName = "Friendly"),
	Opposing	UMETA(DisplayName = "Opposing"),
	Neutral		UMETA(DisplayName = "Neutral"),
	Friendly2	UMETA(DisplayName = "Friendly 2"),
	Opposing2	UMETA(DisplayName = "Opposing 2"),
	Neutral2	UMETA(DisplayName = "Neutral 2"),
	Friendly3	UMETA(DisplayName = "Friendly 3"),
	Opposing3	UMETA(DisplayName = "Opposing 3"),
	Neutral3	UMETA(DisplayName = "Neutral 3"),
	Friendly4	UMETA(DisplayName = "Friendly 4"),
	Opposing4	UMETA(DisplayName = "Opposing 4"),
	Neutral4	UMETA(DisplayName = "Neutral 4"),
	Friendly5	UMETA(DisplayName = "Friendly 5"),
	Opposing5	UMETA(DisplayName = "Opposing 5"),
	Neutral5	UMETA(DisplayName = "Neutral 5"),
	Friendly6	UMETA(DisplayName = "Friendly 6"),
	Opposing6	UMETA(DisplayName = "Opposing 6"),
	Neutral6	UMETA(DisplayName = "Neutral 6"),
	Friendly7	UMETA(DisplayName = "Friendly 7"),
	Opposing7	UMETA(DisplayName = "Opposing 7"),
	Neutral7	UMETA(DisplayName = "Neutral 7"),
	Friendly8	UMETA(DisplayName = "Friendly 8"),
	Opposing8	UMETA(DisplayName = "Opposing 8"),
	Neutral8	UMETA(DisplayName = "Neutral 8"),
	Friendly9	UMETA(DisplayName = "Friendly 9"),
	Opposing9	UMETA(DisplayName = "Opposing 9"),
	Neutral9	UMETA(DisplayName = "Neutral 9"),
	Friendly10	UMETA(DisplayName = "Friendly 10"),
	Opposing10	UMETA(DisplayName = "Opposing 10"),
	Neutral10	UMETA(DisplayName = "Neutral 10")
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

UENUM(BlueprintType)
enum class EDetonationResult : uint8
{
	Other								UMETA(DisplayName = "Other"),
	EntityImpact						UMETA(DisplayName = "Entity Impact"),
	EntityProximateDetonation			UMETA(DisplayName = "Entity Proximate Detonation"),
	GroundImpact						UMETA(DisplayName = "Ground Impact"),
	GroundProximateImpact				UMETA(DisplayName = "Ground Proximate Impact"),
	Detonation							UMETA(DisplayName = "Detonation"),
	Dud									UMETA(DisplayName = "None or No Detonation (Dud)"),
	HE_hit_sm							UMETA(DisplayName = "HE hit, small"),
	HE_hit_md							UMETA(DisplayName = "HE hit, medium"),
	HE_hit_lg							UMETA(DisplayName = "HE hit, large"),
	ArmorPiercingHit					UMETA(DisplayName = "Armor-piercing hit"),
	DirtBlast_sm						UMETA(DisplayName = "Dirt blast, small"),
	DirtBlast_md						UMETA(DisplayName = "Dirt blast, medium"),
	DirtBlast_lg						UMETA(DisplayName = "Dirt blast, large"),
	WaterBlast_sm						UMETA(DisplayName = "Water blast, small"),
	WaterBlast_md						UMETA(DisplayName = "Water blast, medium"),
	WaterBlast_lg						UMETA(DisplayName = "Water blast, large"),
	AirHit								UMETA(DisplayName = "Air hit"),
	BuildingHit_sm						UMETA(DisplayName = "Building hit, small"),
	BuildingHit_md						UMETA(DisplayName = "Building hit, medium"),
	BuildingHit_lg						UMETA(DisplayName = "Building hit, large"),
	MineClearingLineCharge				UMETA(DisplayName = "Mine-clearing line charge"),
	EnvironmentObjectImpact				UMETA(DisplayName = "Environment object impact"),
	EnvironmentObjectProximateImpact	UMETA(DisplayName = "Environment object proximate detonation"),
	WaterImpact							UMETA(DisplayName = "Water impact"),
	AirBurst							UMETA(DisplayName = "Air Burst"),
	KillWithFragmentType1				UMETA(DisplayName = "Kill with fragment type 1"),
	KillWithFragmentType2				UMETA(DisplayName = "Kill with fragment type 2"),
	KillWithFragmentType3				UMETA(DisplayName = "Kill with fragment type 3"),
	KillWithFragmentType1FlyOutFailure	UMETA(DisplayName = "Kill with fragment type 1 after fly-out failure"),
	KillWithFragmentType2FlyOutFailure	UMETA(DisplayName = "Kill with fragment type 2 after fly-out failure"),
	MissDueToFlyOutFailure				UMETA(DisplayName = "Miss due to fly-out failure"),
	MissDueToEndGameFailure				UMETA(DisplayName = "Miss due to end-game failure"),
	MissDueToFlyOutAndEndGameFailure	UMETA(DisplayName = "Miss due to fly-out and end-game failure")
};

UENUM(BlueprintType)
enum class EEntityDamage : uint8
{
	NoDamage,
	SlightDamage,
	ModerateDamage,
	Destroyed
};

UENUM(BlueprintType)
enum class EPaintScheme : uint8
{
	UniformColor,
	Camouflage
};

UENUM(BlueprintType)
enum class ETrailingEffects : uint8
{
	None,
	Small,
	Medium,
	Large
};

UENUM(BlueprintType)
enum class EHatchState : uint8
{
	NotApplicable,
	Closed,
	Popped,
	PoppedAndPersonVisible,
	Open,
	OpenAndPersonVisible
};

UENUM(BlueprintType)
enum class EVariableParameterRecordType : uint8
{
	ArticulatedPart,
	AttachedPart,
	Separation,
	EntityType,
	EntityAssociation
};

UENUM(BlueprintType)
enum class EBeamFunction : uint8
{
	Other,
	Search,
	HeightFinding,
	Acquisition,
	Tracking,
	AcquisitionAndTracking,
	CommandGuidance,
	Illumination,
	Ranging,
	MissileBeacon,
	MissileFusing,
	ActiveRadarMissileSeeker,
	Jamming,
	IFF,
	NavigationWeather,
	Meteorological,
	DataTransmission,
	NavigationalDirectionalBeacon,
	TimeSharedSearch,
	TimeSharedAcquisition,
	TimeSharedTrack,
	TimeSharedCommandGuidance,
	TimeSharedIllumination,
	TimeSharedJamming
};

UENUM(BlueprintType)
enum class EEncodingClass : uint8
{
	EncodedAudio,
	RawBinaryData,
	Application_SpecificData,
	DatabaseIndex
};

UENUM(BlueprintType)
enum class EEncodingType : uint8
{
	Other,
	EightBit_MuLaw,
	CVSD,
	ADPCM,
	SixteenBitLinearPcmTwosComplement_BigEndian,
	EightBitLinearPcmUnsigned,
	VectorQuantization_Deprecated,
	GsmFull_Rate = 8U,
	GsmHalfRate,
	SpeexNarrowBand,
	SixteenBitLinearPcmTwosComplement_LittleEndian = 100U
};

UENUM(BlueprintType)
enum class ETDLType : uint8
{
	Other,
	PADIL,
	NATOLink1,
	ATDL1,
	Link11B,
	SituationalAwarenessDataLink,
	Link16LegacyFormat_JTIDS_TADILJ,
	Link16LegacyFormat_JTIDS_FDL_TADILJ,
	Link11A,
	IJMS,
	Link4A,
	Link4C,
	TIBS,
	ATL,
	ConstantSource,
	AbbreviatedCommandandControl,
	MILSTAR,
	ATHS,
	OTHGOLD,
	TACELINT,
	WeaponsDataLink_AWW13,
	AbbreviatedCommandAndControl_Deprecated,
	EnhancedPositionLocationReportingSystem,
	PositionLocationReportingSystem,
	SINCGARS,
	HaveQuickI,
	HaveQuickII,
	HaveQuickIIA,
	IntraFlightDataLink1,
	IntraFlightDataLink2,
	ImprovedDataModem,
	AirForceApplicationProgramDevelopment,
	CooperativeEngagementCapability,
	ForwardAreaAirDefenseDataLink,
	GroundBasedDataLink,
	IntraVehicularInfoSystem,
	MarineTacticalSystem,
	TacticalFireDirectionSystem,
	IntegratedBroadcastService,
	AirborneInformationTransfer,
	AdvancedTacticalAirborneReconnaissanceSystemDataLink,
	BattleGroupPassiveHorizonExtensionSystemDataLink,
	CommonHighBandwidthDataLink,
	GuardrailInteroperableDataLink,
	GuardrailCSS1DataLink,
	GuardrailCSS2DataLink,
	GuardrailCSS2MultiRoleDataLink,
	GuardrailCSS2DirectAirtoSatelliteRelayDataLink,
	LineofSightDataLinkImplementation,
	LightweightCDL,
	L52M,
	RivetReach_RivetOwlDataLink,
	SeniorSpan,
	SeniorSpur,
	SeniorStretch,
	SeniorYearInteroperableDataLink,
	SpaceCDL,
	TR1modeMISTAirborneDataLink,
	KubandSATCOMDataLinkImplementation,
	MissionEquipmentControlDatalink,
	RadarDataTransmittingSetDataLink,
	SurveillanceandControlDataLink,
	TacticalUAVVideo,
	UHFSATCOMDataLinkImplementation,
	TacticalCommonDataLink,
	LowLevelAirPictureInterface,
	WeaponsDataLink_AGM130,
	AutomaticIdentificationSystem,
	WeaponsDataLink_AIM120,
	WeaponsDataLink_AIM9,
	GC3 = 99U,
	Link16StandardizedFormat,
	Link16EnhancedDataRate,
	JTIDS_MIDSNetDataLoad,
	Link22,
	AFIWCIADSCommunicationsLinks,
	F22IntraFlightDataLink,
	LBandSATCOM,
	TSAFCommunicationsLink,
	EnhancedSINCGARS7_3,
	F35MultifunctionAdvancedDataLink,
	CursoronTarget,
	AllPurposeStructuredEurocontrolSurveillanceInformationExchange
};

UENUM(BlueprintType)
enum class EStateUpdateIndicator : uint8
{
	HeartbeatUpdate,
	ChangedDataUpdate
};

UENUM(BlueprintType)
enum class EEmitterSystemFunction : uint8
{
	Other,
	MultiFunction,
	EarlyWarning_Surveillance,
	HeightFinder,
	FireControl,
	Acquisition_Detection,
	Tracker,
	Guidance_Illumination,
	Firingpoint_launchpointlocation,
	RangeOnly,
	RadarAltimeter,
	Imaging,
	MotionDetection,
	Navigation,
	Weather_Meteorological,
	Instrumentation,
	Identification_Classification,
	AntiAircraftArtilleryFireControl,
	AirSearch_Bomb,
	AirIntercept,
	Altimeter,
	AirMapping,
	AirTrafficControl,
	Beacon,
	BattlefieldSurveillance,
	GroundControlApproach,
	GroundControlIntercept,
	CoastalSurveillance,
	Decoy_Mimic,
	DataTransmission,
	EarthSurveillance,
	GunLayBeacon,
	GroundMapping,
	HarborSurveillance,
	IdentifyFriendOrFoe_Deprecated,
	InstrumentLandingSystem,
	IonosphericSound,
	Interrogator, 
	BarrageJamming_Deprecated,
	ClickJamming_Deprecated,
	DeceptiveJamming_Deprecated,
	FrequencySweptJamming_Deprecated,
	Jammer,
	NoiseJamming_Deprecated,
	PulsedJamming_Deprecated,
	RepeaterJamming_Deprecated,
	SpotNoiseJamming_Deprecated,
	MissileAcquisition,
	MissileDownlink,
	Meteorological_Deprecated,
	Space,
	SurfaceSearch,
	ShellTracking,
	Television = 56U,
	Unknown,
	VideoRemoting,
	ExperimentalOrTraining,
	MissileGuidance,
	MissileHoming,
	MissileTracking,
	JammingNoise_Deprecated = 64U,
	JammingDeception_Deprecated,
	Decoy_Deprecated,
	Navigation_DistanceMeasuringEquipment = 71U,
	TerrainFollowing,
	WeatherAvoidance,
	ProximityFuse,
	Instrumentation_Deprecated,
	Radiosonde,
	Sonobuoy,
	BathythermalSensor,
	TowedCounterMeasure,
	WeaponNonLethal = 96U,
	WeaponLethal
};

UENUM(BlueprintType)
enum class EHighDensityTrackJam : uint8
{
	NotSelected,
	Selected
};

USTRUCT(BlueprintType)
struct FEastNorthUp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	FVector EastVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	FVector NorthVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	FVector NorthVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	FVector EastVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float Heading;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float Psi;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float Theta;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
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

	FPsiThetaPhi(FRotator UnrealRotation)
	{
		this->Psi = UnrealRotation.Yaw;
		this->Theta = UnrealRotation.Pitch;
		this->Phi = UnrealRotation.Roll;
	}
};

USTRUCT(BlueprintType)
struct FClockTime
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	int32 Hour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "4294967295"), Category = "GRILL DIS|Structs")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "65535"), Category = "GRILL DIS|Structs")
	int32 Site;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "65535"), Category = "GRILL DIS|Structs")
	int32 Application;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "65535"), Category = "GRILL DIS|Structs")
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
		return (static_cast<uint64>(Site) << 32) | (static_cast<uint64>(Application) << 16) | (static_cast<uint64>(Entity));
	}
};

USTRUCT(BlueprintType)
struct FEventID
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "65535"), Category = "GRILL DIS|Structs")
	int32 Site;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "65535"), Category = "GRILL DIS|Structs")
	int32 Application;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "65535"), Category = "GRILL DIS|Structs")
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255"), Category = "GRILL DIS|Structs")
	int32 EntityKind;
	/** Domain of entity (air, surface, subsurface, space, etc) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255"), Category = "GRILL DIS|Structs")
	int32 Domain;
	/** Country to which the design of the entity is attributed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "65535"), Category = "GRILL DIS|Structs")
	int32 Country;
	/** The main category that describes the entity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255"), Category = "GRILL DIS|Structs")
	int32 Category;
	/** The particular subcategory to which the entity belongs based on the `Category` field */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255"), Category = "GRILL DIS|Structs")
	int32 Subcategory;
	/** Specific information about the entity based on `Subcategory` field */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255"), Category = "GRILL DIS|Structs")
	int32 Specific;
	/** Extra information required to describe a particular entity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255"), Category = "GRILL DIS|Structs")
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

	FEntityType(double EntityType)
	{
		uint8_t* EntityTypeInBytes = reinterpret_cast<uint8_t*>(&EntityType);

		if (sizeof(EntityTypeInBytes) != 8)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to convert double to Entity Type. Resorting to default values."));

			EntityKind = -1;
			Domain = -1;
			Country = -1;
			Category = -1;
			Subcategory = -1;
			Specific = -1;
			Extra = -1;

			return;
		}

		EntityKind = EntityTypeInBytes[7];
		Domain = EntityTypeInBytes[6];
		Country = ((uint16_t)EntityTypeInBytes[5] << 8) | EntityTypeInBytes[4];
		Category = EntityTypeInBytes[3];
		Subcategory = EntityTypeInBytes[2];
		Specific = EntityTypeInBytes[1];
		Extra = EntityTypeInBytes[0];
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

	double ToDouble() const
	{
		uint8_t temp[8];
		temp[0] = Extra;
		temp[1] = Specific;
		temp[2] = Subcategory;
		temp[3] = Category;
		temp[4] = Country & 0xFF;
		temp[5] = Country >> 8;
		temp[6] = Domain;
		temp[7] = EntityKind;

		double EntityTypeAsDouble = reinterpret_cast<double&>(*temp);

		return EntityTypeAsDouble;
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

	/**  Identification of whether the Parameter Type Record is for an articulated (0) or attached part (1) shall be designated by this field */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	EVariableParameterRecordType ParameterTypeDesignator;
	/** Indicates the change of any paramater for any articulated part. Increments by 1 for each change. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255"), Category = "GRILL DIS|Structs")
	int32 ChangeIndicator;
	/** The ID of the part to which this part is attached */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "65535"), Category = "GRILL DIS|Structs")
	int32 PartAttachedTo;
	/** The type class (multiples of 32 in the range 1,024 - 4,294,967,264) and type metric (0 - 31) of the articulated part. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	int32 ParameterType;
	//EVariableRecordTypes ParameterType;
	/** The parameter value as defined by the ParameterType variable. Will only be utilized if the Parameter Type Designator is an articulated part (0). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float ParameterValue;
	/** The type of the entity. Will only be utilized if the Parameter Type Designator is an attached part (1). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	FEntityType AttachedPartType;

	FArticulationParameters()
	{
		ParameterTypeDesignator = EVariableParameterRecordType::ArticulatedPart;
		ChangeIndicator = 0;
		PartAttachedTo = 0;
		ParameterType = 0;
		//ParameterType = EVariableRecordTypes::Other_Zero;
		ParameterValue = 0.f;
	}

	FArticulationParameters(DIS::ArticulationParameter ArticulatedPart)
	{
		ParameterTypeDesignator = static_cast<EVariableParameterRecordType>(ArticulatedPart.getParameterTypeDesignator());
		ChangeIndicator = ArticulatedPart.getChangeIndicator();
		PartAttachedTo = ArticulatedPart.getPartAttachedTo();
		ParameterType = ArticulatedPart.getParameterType();
		//ParameterType = static_cast<EVariableRecordTypes>(ArticulatedPart.getParameterType());

		if (ParameterTypeDesignator == EVariableParameterRecordType::ArticulatedPart)
		{
			ParameterValue = ArticulatedPart.getParameterValue();
		}
		else
		{
			AttachedPartType = ArticulatedPart.getParameterValue();
		}
	}

	DIS::ArticulationParameter ToOpenDIS() const
	{
		DIS::ArticulationParameter OutParam;
		OutParam.setParameterTypeDesignator(static_cast<unsigned char>(ParameterTypeDesignator));
		OutParam.setChangeIndicator(ChangeIndicator);
		OutParam.setPartAttachedTo(PartAttachedTo);
		OutParam.setParameterType(ParameterType);
		//OutParam.setParameterType(static_cast<int>(ParameterType));

		if (ParameterTypeDesignator == EVariableParameterRecordType::ArticulatedPart)
		{
			OutParam.setParameterValue(ParameterValue);
		}
		else
		{
			OutParam.setParameterValue(AttachedPartType.ToDouble());
		}

		return OutParam;
	}
};

USTRUCT(BlueprintType)
struct FBurstDescriptor
{
	GENERATED_BODY()

	/** The type of the entity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	FEntityType EntityType;
	/** The type of warhead (0 - 65,535) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "65535"), Category = "GRILL DIS|Structs")
	int32 Warhead;
	//EWarheadType Warhead;
	/** The type of fuse (0 - 65,535) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "65535"), Category = "GRILL DIS|Structs")
	int32 Fuse;
	//EFuseType Fuse;
	/** The number of bursts represented (0 - 65,535) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "65535"), Category = "GRILL DIS|Structs")
	int32 Quantity;
	/** The number of rounds per minute for the munition (0 - 65,535) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "65535"), Category = "GRILL DIS|Structs")
	int32 Rate;

	FBurstDescriptor()
	{
		Warhead = 0;
		//Warhead = EWarheadType::Other;
		Fuse = 0;
		//Fuse = EFuseType::Other;
		Quantity = 0;
		Rate = 0;
	}

	FBurstDescriptor(DIS::BurstDescriptor BurstDescriptor)
	{
		EntityType = BurstDescriptor.getMunition();
		Warhead = BurstDescriptor.getWarhead();
		//Warhead = static_cast<EWarheadType>(BurstDescriptor.getWarhead());
		Fuse = BurstDescriptor.getFuse();
		//Fuse = static_cast<EFuseType>(BurstDescriptor.getFuse());
		Quantity = BurstDescriptor.getQuantity();
		Rate = BurstDescriptor.getRate();
	}

	DIS::BurstDescriptor ToOpenDIS() const
	{
		DIS::BurstDescriptor OutDescriptor;
		OutDescriptor.setMunition(EntityType.ToOpenDIS());
		OutDescriptor.setWarhead(Warhead);
		//OutDescriptor.setWarhead(static_cast<unsigned short>(Warhead));
		OutDescriptor.setFuse(Fuse);
		//OutDescriptor.setFuse(static_cast<unsigned short>(Fuse));
		OutDescriptor.setQuantity(Quantity);
		OutDescriptor.setRate(Rate);
		return OutDescriptor;
	}
};

USTRUCT(BlueprintType)
struct FDeadReckoningParameters
{
	GENERATED_BODY()

	/** The type of dead reackoning algorithm used by the entity (0 - 9) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "9"), Category = "GRILL DIS|Structs")
	EDeadReckoningAlgorithm DeadReckoningAlgorithm;
	/** Field used to specify other dead reckoning parameters which are currently undefined */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	TArray<uint8> OtherParameters;
	/** The entity's linear acceleration in m/s^2 in either the World Coordinate System or the Entity's Coordinate System depending on the `DeadReckoningAlgorithm` field. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	FVector EntityLinearAcceleration;
	/** The entity's angular acceleration in radians per second about each of the entity's own coordinate axes. Positive acceleration is defined by the right hand rule. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	FVector EntityAngularVelocity;

	FDeadReckoningParameters()
	{
		DeadReckoningAlgorithm = EDeadReckoningAlgorithm::Static;
		OtherParameters.Init(0, 15);
		EntityLinearAcceleration = FVector(0, 0, 0);
		EntityAngularVelocity = FVector(0, 0, 0);
	}

	FDeadReckoningParameters(DIS::DeadReckoningParameter DeadReckoningParameter)
	{
		DeadReckoningAlgorithm = static_cast<EDeadReckoningAlgorithm>(DeadReckoningParameter.getDeadReckoningAlgorithm());
		OtherParameters = TArray<uint8>(reinterpret_cast<const uint8*>(DeadReckoningParameter.getOtherParameters()), 15);
		EntityLinearAcceleration[0] = DeadReckoningParameter.getEntityLinearAcceleration().getX();
		EntityLinearAcceleration[1] = DeadReckoningParameter.getEntityLinearAcceleration().getY();
		EntityLinearAcceleration[2] = DeadReckoningParameter.getEntityLinearAcceleration().getZ();
		EntityAngularVelocity[0] = DeadReckoningParameter.getEntityAngularVelocity().getX();
		EntityAngularVelocity[1] = DeadReckoningParameter.getEntityAngularVelocity().getY();
		EntityAngularVelocity[2] = DeadReckoningParameter.getEntityAngularVelocity().getZ();
	}

	DIS::DeadReckoningParameter ToOpenDIS() const
	{
		DIS::DeadReckoningParameter OutParam;
		OutParam.setDeadReckoningAlgorithm((uint8)DeadReckoningAlgorithm);
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

//Only common entity appearance fields as this is dependant on domain
USTRUCT(BlueprintType)
struct FEntityAppearance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	EPaintScheme PaintScheme;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	bool MobilityKilled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	bool FirePowerKilled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	EEntityDamage Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	bool IsSmoking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	bool IsEngineSmoking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	int Trailing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	int HatchState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	bool LightPrimary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	bool LightSecondary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	bool LightCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	bool IsFlaming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	bool IsFrozen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	bool IsDeactivated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	bool IsLandingGearExtended;

	int32 RawVal = 0;

	FEntityAppearance() 
	{
		PaintScheme = EPaintScheme::UniformColor;
		MobilityKilled = false;
		FirePowerKilled = false;
		Damage = EEntityDamage::NoDamage;
		IsSmoking = false;
		IsEngineSmoking = false;
		Trailing = false;
		HatchState = false;
		LightPrimary = false;
		LightSecondary = false;
		LightCollision = false;
		IsFlaming = false;

		IsFrozen = false;
		IsDeactivated = false;

		IsLandingGearExtended = false;
	}

	FEntityAppearance(uint32 val)
		: RawVal(val)
	{
		PaintScheme = static_cast<EPaintScheme>(getField(val, 0));
		MobilityKilled = getField(val, 1);
		FirePowerKilled = getField(val, 2);
		Damage = static_cast<EEntityDamage>(getField(val, 0b11, 3));
		IsSmoking = getField(val, 5);
		IsEngineSmoking = getField(val, 6);
		Trailing = getField(val, 0b11, 7);
		HatchState = getField(val, 0b111, 9);
		LightPrimary = getField(val, 12);
		LightSecondary = getField(val, 13);
		LightCollision = getField(val, 14);
		IsFlaming = getField(val, 15);

		IsFrozen = getField(val, 21);
		IsDeactivated = getField(val, 23);

		IsLandingGearExtended = getField(val, 25);
	}

	static int getField(uint32 val, int mask, int pos)
	{
		return (val & (mask << pos)) >> pos;
	}
	static bool getField(uint32 val, int pos)
	{
		return getField(val, 0b1, pos) != 0;
	}

	int32 UpdateValue()
	{
		RawVal |= bool(PaintScheme) << 0;
		RawVal |= MobilityKilled << 1;
		RawVal |= FirePowerKilled << 2;
		RawVal |= int(Damage) << 3;
		RawVal |= IsSmoking << 5;
		RawVal |= IsEngineSmoking << 6;
		RawVal |= Trailing << 7;
		RawVal |= HatchState << 9;
		RawVal |= LightPrimary << 12;
		RawVal |= LightSecondary << 13;
		RawVal |= LightCollision << 14;
		RawVal |= IsFlaming << 15;
		RawVal |= IsFrozen << 21;
		RawVal |= IsDeactivated << 23;
		RawVal |= IsLandingGearExtended << 25;
		return RawVal;
	}
};

USTRUCT(BlueprintType)
struct FEncodingScheme
{
	GENERATED_BODY()

	/** This field shall the following encoding class of the encoding scheme. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	EEncodingClass EncodingClass;
	/** This field shall represent the encoding type of the encoding scheme. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	EEncodingType EncodingType;

	FEncodingScheme()
	{
		EncodingClass = EEncodingClass::EncodedAudio;
		EncodingType = EEncodingType::Other;
	}

	FEncodingScheme(unsigned short EncodingScheme)
	{
		//Mask for getting first 14 bits of Encoding Scheme.
		unsigned short mask = 0;
		mask = ~mask;
		mask = mask >> 2;

		EncodingClass = static_cast<EEncodingClass>(EncodingScheme >> 14);
		EncodingType = static_cast<EEncodingType>(EncodingScheme & mask);
	}

	unsigned short ToOpenDIS()
	{
		unsigned short outEncodingClass = static_cast<unsigned short>(EncodingClass);
		unsigned short outEncodingType = static_cast<unsigned short>(EncodingType);

		return (outEncodingClass << 14) | outEncodingType;
	}
};

USTRUCT(BlueprintType)
struct FBeamData
{
	GENERATED_BODY()

	/** Specifies the beam azimuth an elevation centers and corresponding half-angles to describe the scan volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float BeamAzimuthCenter;
	/** Specifies the beam azimuth sweep to determine scan volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float BeamAzimuthSweep;
	/** Specifies the beam elevation center to determine scan volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float BeamElevationCenter;
	/** Specifies the beam elevation sweep to determine scan volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float BeamElevationSweep;
	/**  This field is provided to allow a receiver to synchronize its regenerated scan pattern to that of the emitter. This field when employed shall specify the percentage of time a scan is through its pattern from its origin. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float BeamSweepSync;

	FBeamData()
	{
		BeamAzimuthCenter = 0;
		BeamAzimuthSweep = 0;
		BeamElevationCenter = 0;
		BeamElevationSweep = 0;
		BeamSweepSync = 0;
	}

	FBeamData(DIS::BeamData BeamData)
	{
		BeamAzimuthCenter = BeamData.getBeamAzimuthCenter();
		BeamAzimuthSweep = BeamData.getBeamAzimuthSweep();
		BeamElevationCenter = BeamData.getBeamElevationCenter();
		BeamElevationSweep = BeamData.getBeamElevationSweep();
		BeamSweepSync = BeamData.getBeamSweepSync();
	}

	DIS::BeamData ToOpenDIS()
	{
		DIS::BeamData outBeamData;

		outBeamData.setBeamAzimuthCenter(BeamAzimuthCenter);
		outBeamData.setBeamAzimuthSweep(BeamAzimuthSweep);
		outBeamData.setBeamElevationCenter(BeamElevationCenter);
		outBeamData.setBeamElevationSweep(BeamElevationSweep);
		outBeamData.setBeamSweepSync(BeamSweepSync);

		return outBeamData;
	}
};

USTRUCT(BlueprintType)
struct FFundamentalParameterData
{
	GENERATED_BODY()

	/** Center frequency of the emission in hertz. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float Frequency;
	/** Bandwidth of the frequencies corresponding to the fequency field. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float FrequencyRange;
	/** Effective radiated power for the emission in DdBm. For a radar noise jammer, indicates the peak of the transmitted power. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float EffectiveRadiatedPower;
	/** Average repetition frequency of the emission in hertz. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float PulseRepetitionFrequency;
	/** Average pulse width  of the emission in microseconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float PulseWidth;
	/** Specifies the beam azimuth an elevation centers and corresponding half-angles to describe the scan volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float BeamAzimuthCenter;
	/** Specifies the beam azimuth sweep to determine scan volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float BeamAzimuthSweep;
	/** Specifies the beam elevation center to determine scan volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float BeamElevationCenter;
	/** Specifies the beam elevation sweep to determine scan volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float BeamElevationSweep;
	/**  This field is provided to allow a receiver to synchronize its regenerated scan pattern to that of the emitter. This field when employed shall specify the percentage of time a scan is through its pattern from its origin. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	float BeamSweepSync;

	FFundamentalParameterData()
	{
		Frequency = 0;
		FrequencyRange = 0;
		EffectiveRadiatedPower = 0;
		PulseRepetitionFrequency = 0;
		PulseWidth = 0;
		BeamAzimuthCenter = 0;
		BeamAzimuthSweep = 0;
		BeamElevationCenter = 0;
		BeamElevationSweep = 0;
		BeamSweepSync = 0;
	}

	FFundamentalParameterData(DIS::FundamentalParameterData FundamentalParameterData)
	{
		Frequency = FundamentalParameterData.getFrequency();
		FrequencyRange = FundamentalParameterData.getFrequencyRange();
		EffectiveRadiatedPower = FundamentalParameterData.getEffectiveRadiatedPower();
		PulseRepetitionFrequency = FundamentalParameterData.getPulseRepetitionFrequency();
		PulseWidth = FundamentalParameterData.getPulseWidth();
		BeamAzimuthCenter = FundamentalParameterData.getBeamAzimuthCenter();
		BeamAzimuthSweep = FundamentalParameterData.getBeamAzimuthSweep();
		BeamElevationCenter = FundamentalParameterData.getBeamElevationCenter();
		BeamElevationSweep = FundamentalParameterData.getBeamElevationSweep();
		BeamSweepSync = FundamentalParameterData.getBeamSweepSync();
	}

	DIS::FundamentalParameterData ToOpenDIS()
	{
		DIS::FundamentalParameterData outFundamentalParameterData;

		outFundamentalParameterData.setFrequency(Frequency);
		outFundamentalParameterData.setFrequencyRange(FrequencyRange);
		outFundamentalParameterData.setEffectiveRadiatedPower(EffectiveRadiatedPower);
		outFundamentalParameterData.setPulseRepetitionFrequency(PulseRepetitionFrequency);
		outFundamentalParameterData.setPulseWidth(PulseWidth);
		outFundamentalParameterData.setBeamAzimuthCenter(BeamAzimuthCenter);
		outFundamentalParameterData.setBeamAzimuthSweep(BeamAzimuthSweep);
		outFundamentalParameterData.setBeamElevationCenter(BeamElevationCenter);
		outFundamentalParameterData.setBeamElevationSweep(BeamElevationSweep);
		outFundamentalParameterData.setBeamSweepSync(BeamSweepSync);

		return outFundamentalParameterData;
	}
};

USTRUCT(BlueprintType)
struct FTrackJamTarget
{
	GENERATED_BODY()

	/** This field shall identify the targets in an emitter track or emitters a system is attempting to jam. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	FEntityID TrackJam;
	/** The Emitter Identifier shall be the Emitter ID number of the emitter for which the jamming emission is intended. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255"), Category = "GRILL DIS|Structs")
	int32 EmitterID;
	/** The Beam Identifier shall be Beam ID number of the emitter beam for which the jamming emission is intended. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255"), Category = "GRILL DIS|Structs")
	int32 BeamID;

	FTrackJamTarget()
	{
		EmitterID = 0;
		BeamID = 0;
	}

	FTrackJamTarget(DIS::TrackJamTarget TrackJamTarget)
	{
		TrackJam = TrackJamTarget.getTrackJam();
		EmitterID = TrackJamTarget.getEmitterID();
		BeamID = TrackJamTarget.getBeamID();
	}
	
	DIS::TrackJamTarget ToOpenDIS()
	{
		DIS::TrackJamTarget outTrackJamTarget;

		outTrackJamTarget.setTrackJam(TrackJam.ToOpenDIS());
		outTrackJamTarget.setEmitterID(EmitterID);
		outTrackJamTarget.setBeamID(BeamID);

		return outTrackJamTarget;
	}
};

USTRUCT(BlueprintType)
struct FEmitterSystem
{
	GENERATED_BODY()

	/** This field shall specify the emitter name for a particular emitter. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "65535"), Category = "GRILL DIS|Structs")
	int32 EmitterName;
	/** This field shall specify the function for a particular emitter.  This field is intended to help receiving entities determine if the Electromagnetic Emission PDU is of interest to the systems simulated by that entity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs")
	EEmitterSystemFunction Function;
	/** This field shall specify the emitter identification number for a specific emitter system. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255"), Category = "GRILL DIS|Structs")
	int32 EmitterIDNumber;

	FEmitterSystem()
	{
		EmitterName = 0;
		//EmitterName = EEmitterName::Other;
		Function = EEmitterSystemFunction::Other;
		EmitterIDNumber = 0;
	}

	FEmitterSystem(DIS::EmitterSystem EmitterSystem)
	{
		EmitterName = EmitterSystem.getEmitterName();
		//EmitterName = static_cast<EEmitterName>(EmitterSystem.getEmitterName());
		Function = static_cast<EEmitterSystemFunction>(EmitterSystem.getFunction());
		EmitterIDNumber = EmitterSystem.getEmitterIdNumber();
	}

	DIS::EmitterSystem ToOpenDIS()
	{
		DIS::EmitterSystem outEmitterSystem;

		outEmitterSystem.setEmitterName(static_cast<unsigned short>(EmitterName));
		outEmitterSystem.setFunction(static_cast<unsigned char>(Function));
		outEmitterSystem.setEmitterIdNumber(EmitterIDNumber);

		return outEmitterSystem;
	}
};

USTRUCT(BlueprintType)
struct FElectromagneticEmissionBeamData
{
	GENERATED_BODY()
	
	/** This field shall specify the length of this beam's data (including track/jam information) in 32-bit words. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255"), Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
	int32 BeamDataLength;
	/** This field shall specify a unique emitter database number assigned to differentiate between otherwise similar or identical emitter beams within an emitter system. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255"), Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
	int32 BeamIDNumber;
	/** This field shall specify a Beam Parameter Index number that shall be used by receiving entities in conjunction with the Emitter Name field to provide a pointer to the stored database parameters required to regenerate the beam. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "65535"), Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
	int32 BeamParameterIndex;
	/** This field shall specify dynamic parameters of the emitter. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
	FFundamentalParameterData FundamentalParameterData;
	/** beam function of a particular beam */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
	EBeamFunction BeamFunction;
	/** This field shall be used to indicate whether or not the receiving simulation applications can assume that all targets, in the scan pattern which the sending emitter can track or jam, are being tracked or jammed respectively. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
	EHighDensityTrackJam HighDensityTrackJam;
	/** An 8 bit field of unused padding */
	UPROPERTY()
	int32 Pad4;
	/** This field shall be used to identify one or multiple jamming techniques being applied. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "4294967295"), Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
	int64 JammingModeSequence;
	/** This field shall identify the targets in an emitter track or emitters a system is attempting to jam. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
	TArray<FTrackJamTarget> TrackJamTargets;

	FElectromagneticEmissionBeamData()
	{
		BeamDataLength = 0;
		BeamIDNumber = 0;
		BeamParameterIndex = 0;
		BeamFunction = EBeamFunction::Other;
		HighDensityTrackJam = EHighDensityTrackJam::NotSelected;
		Pad4 = 0;
		JammingModeSequence = 0;
	}

	void SetupFromOpenDIS(const DIS::ElectromagneticEmissionBeamData& Data)
	{
		BeamDataLength = Data.getBeamDataLength();
		BeamIDNumber = Data.getBeamIDNumber();
		BeamParameterIndex = Data.getBeamParameterIndex();
		FundamentalParameterData = Data.getFundamentalParameterData();
		BeamFunction = static_cast<EBeamFunction>(Data.getBeamFunction());
		HighDensityTrackJam = static_cast<EHighDensityTrackJam>(Data.getHighDensityTrackJam());
		Pad4 = Data.getPad4();
		JammingModeSequence = Data.getJammingModeSequence();

		TrackJamTargets.Empty();
		for (int i = 0; i < Data.getNumberOfTrackJamTargets(); i++)
		{
			TrackJamTargets.Add(Data.getTrackJamTargets()[i]);
		}
	}

	DIS::ElectromagneticEmissionBeamData ToOpenDIS()
	{
		DIS::ElectromagneticEmissionBeamData outBeamData;

		outBeamData.setBeamDataLength(BeamDataLength);
		outBeamData.setBeamIDNumber(BeamIDNumber);
		outBeamData.setBeamParameterIndex(BeamParameterIndex);
		outBeamData.setFundamentalParameterData(FundamentalParameterData.ToOpenDIS());
		outBeamData.setBeamFunction(static_cast<unsigned char>(BeamFunction));
		outBeamData.setHighDensityTrackJam(static_cast<unsigned char>(HighDensityTrackJam));
		outBeamData.setPad4(Pad4);
		outBeamData.setJammingModeSequence(JammingModeSequence);

		std::vector<DIS::TrackJamTarget> outTrackJamTargets;
		for (FTrackJamTarget Param : TrackJamTargets)
		{
			outTrackJamTargets.push_back(Param.ToOpenDIS());
		}
		outBeamData.setTrackJamTargets(outTrackJamTargets);

		return outBeamData;
	}
};

USTRUCT(BlueprintType)
struct FElectromagneticEmissionSystemData
{
	GENERATED_BODY()

	/**  This field shall specify the length of this emitter system's data. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "255"), Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
	int32 SystemDataLength;
	/** A 16 bit field of unused padding */
	UPROPERTY()
	int32 EmissionsPadding;
	/** This field shall specify information about a particular emitter system. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
	FEmitterSystem EmitterSystem;
	/** This field shall specify the location of the antenna beam source with respect to the emitting entity's coordinate system. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
	FVector Location;
	/** Variable length list of track/jam targets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
	TArray<FElectromagneticEmissionBeamData> BeamDataRecords;

	FElectromagneticEmissionSystemData()
	{
		SystemDataLength = 0;
		EmissionsPadding = 0;
		Location = FVector::Zero();
	}

	void SetupFromOpenDIS(const DIS::ElectromagneticEmissionSystemData& Data)
	{
		SystemDataLength = Data.getSystemDataLength();
		EmissionsPadding = Data.getEmissionsPadding2();
		EmitterSystem = Data.getEmitterSystem();

		Location[0] = Data.getLocation().getX();
		Location[1] = Data.getLocation().getY();
		Location[2] = Data.getLocation().getZ();

		BeamDataRecords.Empty();
		for (int i = 0; i < Data.getNumberOfBeams(); i++)
		{
			FElectromagneticEmissionBeamData Beam;
			Beam.SetupFromOpenDIS(Data.getBeamDataRecords()[i]);
			BeamDataRecords.Add(Beam);
		}
	}

	DIS::ElectromagneticEmissionSystemData ToOpenDIS()
	{
		DIS::ElectromagneticEmissionSystemData outSystemData;

		outSystemData.setSystemDataLength(SystemDataLength);
		outSystemData.setEmissionsPadding2(EmissionsPadding);
		outSystemData.setEmitterSystem(EmitterSystem.ToOpenDIS());

		outSystemData.getLocation().setX(Location[0]);
		outSystemData.getLocation().setY(Location[1]);
		outSystemData.getLocation().setZ(Location[2]);

		std::vector<DIS::ElectromagneticEmissionBeamData> outBeamData;
		for (FElectromagneticEmissionBeamData Param : BeamDataRecords)
		{
			outBeamData.push_back(Param.ToOpenDIS());
		}
		outSystemData.setBeamDataRecords(outBeamData);

		return outSystemData;
	}
};