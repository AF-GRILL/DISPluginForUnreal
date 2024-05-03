// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

//Blueprints don't currently support non-uint8 enums.
enum class EFuseType : uint16
{
	Other,
	IntelligentInfluence UMETA(DisplayName = "Intelligent Influence") = 10U,
	Sensor UMETA(DisplayName = "Sensor") = 20U,
	Selfdestruct UMETA(DisplayName = "Self-destruct") = 30U,
	UltraQuick UMETA(DisplayName = "Ultra Quick") = 40U,
	Body UMETA(DisplayName = "Body") = 50U,
	DeepIntrusion UMETA(DisplayName = "Deep Intrusion") = 60U,
	Multifunction UMETA(DisplayName = "Multifunction") = 100U,
	PointDetonationPD UMETA(DisplayName = "Point Detonation (PD)") = 200U,
	BaseDetonationBD UMETA(DisplayName = "Base Detonation (BD)") = 300U,
	Contact UMETA(DisplayName = "Contact") = 1000U,
	Contact_InstantImpact UMETA(DisplayName = "Contact, Instant (Impact)") = 1100U,
	Contact_Delayed UMETA(DisplayName = "Contact, Delayed") = 1200U,
	TenMsDelay_New UMETA(DisplayName = "10 ms delay (New)") = 1201U,
	TwentyMsDelay_New UMETA(DisplayName = "20 ms delay (New)") = 1202U,
	FiftyMsDelay_New UMETA(DisplayName = "50 ms delay (New)") = 1205U,
	SixtyMsDelay_New UMETA(DisplayName = "60 ms delay (New)") = 1206U,
	OneHundredMsDelay_New UMETA(DisplayName = "100 ms delay (New)") = 1210U,
	OneTwentyFiveMsDelay_New UMETA(DisplayName = "125 ms delay (New)") = 1212U,
	TwoFiftyMsDelay_New UMETA(DisplayName = "250 ms delay (New)") = 1225U,
	FiveMsDelay UMETA(DisplayName = "5 ms delay") = 1250U,
	FifteenMsDelay UMETA(DisplayName = "15 ms delay") = 1251U,
	TwentyFiveMsDelay UMETA(DisplayName = "25 ms delay") = 1252U,
	ThirtyMsDelay UMETA(DisplayName = "30 ms delay") = 1253U,
	ThirtyFiveMsDelay UMETA(DisplayName = "35 ms delay") = 1254U,
	FourtyMsDelay UMETA(DisplayName = "40 ms delay") = 1255U,
	FourtyFiveMsDelay UMETA(DisplayName = "45 ms delay") = 1256U,
	NinetyMsDelay UMETA(DisplayName = "90 ms delay") = 1257U,
	OneTwentyMsDelay UMETA(DisplayName = "120 ms delay") = 1258U,
	OneEightyMsDelay UMETA(DisplayName = "180 ms delay") = 1259U,
	TwoFortyMsDelay UMETA(DisplayName = "240 ms delay") = 1260U,
	Contact_ElectronicObliqueContact UMETA(DisplayName = "Contact, Electronic (Oblique Contact)") = 1300U,
	Contact_Graze UMETA(DisplayName = "Contact, Graze") = 1400U,
	Contact_Crush UMETA(DisplayName = "Contact, Crush") = 1500U,
	Contact_Hydrostatic UMETA(DisplayName = "Contact, Hydrostatic") = 1600U,
	Contact_Mechanical UMETA(DisplayName = "Contact, Mechanical") = 1700U,
	Contact_Chemical UMETA(DisplayName = "Contact, Chemical") = 1800U,
	Contact_Piezoelectric UMETA(DisplayName = "Contact, Piezoelectric") = 1900U,
	Contact_PointInitiating UMETA(DisplayName = "Contact, Point Initiating") = 1910U,
	Contact_PointInitiating_BaseDetonating UMETA(DisplayName = "Contact, Point Initiating, Base Detonating") = 1920U,
	Contact_BaseDetonating UMETA(DisplayName = "Contact, Base Detonating") = 1930U,
	Contact_BallisticCapandBase UMETA(DisplayName = "Contact, Ballistic Cap and Base") = 1940U,
	Contact_Base UMETA(DisplayName = "Contact, Base") = 1950U,
	Contact_Nose UMETA(DisplayName = "Contact, Nose") = 1960U,
	Contact_FittedinStandoffProbe UMETA(DisplayName = "Contact, Fitted in Standoff Probe") = 1970U,
	Contact_Nonaligned UMETA(DisplayName = "Contact, Non-aligned") = 1980U,
	Timed UMETA(DisplayName = "Timed") = 2000U,
	Timed_Programmable UMETA(DisplayName = "Timed, Programmable") = 2100U,
	Timed_Burnout UMETA(DisplayName = "Timed, Burnout") = 2200U,
	Timed_Pyrotechnic UMETA(DisplayName = "Timed, Pyrotechnic") = 2300U,
	Timed_Electronic UMETA(DisplayName = "Timed, Electronic") = 2400U,
	Timed_BaseDelay UMETA(DisplayName = "Timed, Base Delay") = 2500U,
	Timed_ReinforcedNoseImpactDelay UMETA(DisplayName = "Timed, Reinforced Nose Impact Delay") = 2600U,
	Timed_ShortDelayImpact UMETA(DisplayName = "Timed, Short Delay Impact") = 2700U,
	TenMsDelay UMETA(DisplayName = "10 ms delay") = 2701U,
	TwentyMsDelay UMETA(DisplayName = "20 ms delay") = 2702U,
	FiftyMsDelay UMETA(DisplayName = "50 ms delay") = 2705U,
	SixtyMsDelay UMETA(DisplayName = "60 ms delay") = 2706U,
	OneHundredMsDelay UMETA(DisplayName = "100 ms delay") = 2710U,
	OneTwentyFiveMsDelay UMETA(DisplayName = "125 ms delay") = 2712U,
	TwoFiftyMsDelay UMETA(DisplayName = "250 ms delay") = 2725U,
	Timed_NoseMountedVariableDelay UMETA(DisplayName = "Timed, Nose Mounted Variable Delay") = 2800U,
	Timed_LongDelaySide UMETA(DisplayName = "Timed, Long Delay Side") = 2900U,
	Timed_SelectableDelay UMETA(DisplayName = "Timed, Selectable Delay") = 2910U,
	Timed_Impact UMETA(DisplayName = "Timed, Impact") = 2920U,
	Timed_Sequence UMETA(DisplayName = "Timed, Sequence") = 2930U,
	Proximity UMETA(DisplayName = "Proximity") = 3000U,
	Proximity_ActiveLaser UMETA(DisplayName = "Proximity, Active Laser") = 3100U,
	Proximity_MagneticMagpolarity UMETA(DisplayName = "Proximity, Magnetic (Magpolarity)") = 3200U,
	Proximity_ActiveRadarDopplerRadar UMETA(DisplayName = "Proximity, Active Radar (Doppler Radar)") = 3300U,
	Proximity_RadioFrequencyRF UMETA(DisplayName = "Proximity, Radio Frequency (RF)") = 3400U,
	Proximity_Programmable UMETA(DisplayName = "Proximity, Programmable") = 3500U,
	Proximity_Programmable_Prefragmented UMETA(DisplayName = "Proximity, Programmable, Prefragmented") = 3600U,
	Proximity_Infrared UMETA(DisplayName = "Proximity, Infrared") = 3700U,
	Command UMETA(DisplayName = "Command") = 4000U,
	Command_Electronic_RemotelySet UMETA(DisplayName = "Command, Electronic, Remotely Set") = 4100U,
	Altitude UMETA(DisplayName = "Altitude") = 5000U,
	Altitude_RadioAltimeter UMETA(DisplayName = "Altitude, Radio Altimeter") = 5100U,
	Altitude_AirBurst UMETA(DisplayName = "Altitude, Air Burst") = 5200U,
	Depth UMETA(DisplayName = "Depth") = 6000U,
	Acoustic UMETA(DisplayName = "Acoustic") = 7000U,
	Pressure UMETA(DisplayName = "Pressure") = 8000U,
	Pressure_Delay UMETA(DisplayName = "Pressure, Delay") = 8010U,
	Inert UMETA(DisplayName = "Inert") = 8100U,
	Dummy UMETA(DisplayName = "Dummy") = 8110U,
	Practice UMETA(DisplayName = "Practice") = 8120U,
	PlugRepresenting UMETA(DisplayName = "Plug Representing") = 8130U,
	Training UMETA(DisplayName = "Training") = 8150U,
	Pyrotechnic UMETA(DisplayName = "Pyrotechnic") = 9000U,
	Pyrotechnic_Delay UMETA(DisplayName = "Pyrotechnic, Delay") = 9010U,
	Electrooptical UMETA(DisplayName = "Electro-optical") = 9100U,
	Electromechanical UMETA(DisplayName = "Electromechanical") = 9110U,
	Electromechanical_Nose UMETA(DisplayName = "Electromechanical, Nose") = 9120U,
	Strikerless UMETA(DisplayName = "Strikerless") = 9200U,
	Strikerless_NoseImpact UMETA(DisplayName = "Strikerless, Nose Impact") = 9210U,
	Strikerless_CompressionIgnition UMETA(DisplayName = "Strikerless, Compression-Ignition") = 9220U,
	CompressionIgnition UMETA(DisplayName = "Compression-Ignition") = 9300U,
	CompressionIgnition_Strikerless_NoseImpact UMETA(DisplayName = "Compression-Ignition, Strikerless, Nose Impact") = 9310U,
	Percussion UMETA(DisplayName = "Percussion") = 9400U,
	Percussion_Instantaneous UMETA(DisplayName = "Percussion, Instantaneous") = 9410U,
	Electronic UMETA(DisplayName = "Electronic") = 9500U,
	Electronic_InternallyMounted UMETA(DisplayName = "Electronic, Internally Mounted") = 9510U,
	Electronic_RangeSetting UMETA(DisplayName = "Electronic, Range Setting") = 9520U,
	Electronic_Programmed UMETA(DisplayName = "Electronic, Programmed") = 9530U,
	Mechanical UMETA(DisplayName = "Mechanical") = 9600U,
	Mechanical_Nose UMETA(DisplayName = "Mechanical, Nose") = 9610U,
	Mechanical_Tail UMETA(DisplayName = "Mechanical, Tail") = 9620U
};