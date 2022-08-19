// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#include "DISReceiveComponent.h"

#include "DeadReckoning_BPFL.h"
#include "DISGameManager.h"
#include "CollisionQueryParams.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogDISReceiveComponent);

// Sets default values for this component's properties
UDISReceiveComponent::UDISReceiveComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UDISReceiveComponent::InitializeComponent()
{
	Super::InitializeComponent();

	//Check if the owning actor was tagged as being spawned from the network
	if (GetOwner()->Tags.Contains(ADISGameManager::SPAWNED_FROM_NETWORK_TAG))
	{
		SpawnedFromNetwork = true;
		GetOwner()->Tags.Remove(ADISGameManager::SPAWNED_FROM_NETWORK_TAG);
	}
	else
	{
		SpawnedFromNetwork = false;
	}
}

// Called when the game starts
void UDISReceiveComponent::BeginPlay()
{
	Super::BeginPlay();

	GeoReferencingSystem = AGeoReferencingSystem::GetGeoReferencingSystem(Cast<UObject>(GetWorld()));
}

// Called every frame
void UDISReceiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDISReceiveComponent::HandleEntityStatePDU(FEntityStatePDU NewEntityStatePDU)
{
	//Check if the entity has been deactivated -- Entity is deactivated if the 23rd bit of the Entity Appearance value is set
	if (NewEntityStatePDU.EntityAppearance & (1 << 23))
	{
		UE_LOG(LogDISReceiveComponent, Log, TEXT("%s Entity Appearance is set to deactivated, deleting entity..."), *NewEntityStatePDU.Marking);
		GetOwner()->Destroy();
		return;
	}

	UpdateCommonEntityStateInfo(NewEntityStatePDU);

	EntityType = NewEntityStatePDU.EntityType;
	EntityForceID = NewEntityStatePDU.ForceID;
	EntityMarking = NewEntityStatePDU.Marking;

	OnReceivedEntityStatePDU.Broadcast(NewEntityStatePDU);

	if (!PerformDeadReckoning)
	{
		GroundClamping_Implementation();
	}
}

void UDISReceiveComponent::HandleEntityStateUpdatePDU(FEntityStateUpdatePDU NewEntityStateUpdatePDU)
{
	//Check if the entity has been deactivated -- Entity is deactivated if the 23rd bit of the Entity Appearance value is set
	if (NewEntityStateUpdatePDU.EntityAppearance & (1 << 23))
	{
		UE_LOG(LogDISReceiveComponent, Log, TEXT("%s Entity Appearance is set to deactivated, deleting entity..."), *NewEntityStateUpdatePDU.EntityID.ToString());
		GetOwner()->Destroy();
		return;
	}

	MostRecentEntityStatePDU = NewEntityStateUpdatePDU;
	UpdateCommonEntityStateInfo(MostRecentEntityStatePDU);

	OnReceivedEntityStateUpdatePDU.Broadcast(NewEntityStateUpdatePDU);

	if (!PerformDeadReckoning)
	{
		GroundClamping_Implementation();
	}
}

void UDISReceiveComponent::UpdateCommonEntityStateInfo(FEntityStatePDU NewEntityStatePDU)
{
	LatestEntityStatePDUTimestamp = FDateTime::Now();
	MostRecentEntityStatePDU = NewEntityStatePDU;
	PreviousDeadReckonedPDU = MostRecentDeadReckonedEntityStatePDU;
	MostRecentDeadReckonedEntityStatePDU = MostRecentEntityStatePDU;
	DeltaTimeSinceLastPDU = 0;

	//Get difference in ECEF between most recent dead reckoning location and last known Dead Reckoning location
	EntityECEFLocationDifference = MostRecentEntityStatePDU.EcefLocation - PreviousDeadReckonedPDU.EcefLocation;

	//Get the rotation difference between the last known dead reckoning rotation and the current rotation. This will be used for internal smoothing.
	FRotator prevRotDegrees = FMath::RadiansToDegrees(PreviousDeadReckonedPDU.EntityOrientation);
	FRotator curRotDegrees = FMath::RadiansToDegrees(MostRecentEntityStatePDU.EntityOrientation);
	EntityRotationDifference = FMath::DegreesToRadians((curRotDegrees - prevRotDegrees).GetNormalized());

	EntityID = NewEntityStatePDU.EntityID;

	GetOwner()->SetLifeSpan(DISTimeoutSeconds);

	NumberEntityStatePDUsReceived++;
}

void UDISReceiveComponent::HandleFirePDU(FFirePDU FirePDUIn)
{
	OnReceivedFirePDU.Broadcast(FirePDUIn);
}

void UDISReceiveComponent::HandleDetonationPDU(FDetonationPDU DetonationPDUIn)
{
	OnReceivedDetonationPDU.Broadcast(DetonationPDUIn);
}

void UDISReceiveComponent::HandleRemoveEntityPDU(FRemoveEntityPDU RemoveEntityPDUIn)
{
	OnReceivedRemoveEntityPDU.Broadcast(RemoveEntityPDUIn);
}

void UDISReceiveComponent::HandleStopFreezePDU(FStopFreezePDU StopFreezePDUIn)
{
	OnReceivedStopFreezePDU.Broadcast(StopFreezePDUIn);
}

void UDISReceiveComponent::HandleStartResumePDU(FStartResumePDU StartResumePDUIn)
{
	OnReceivedStartResumePDU.Broadcast(StartResumePDUIn);
}

void UDISReceiveComponent::DoDeadReckoning(float DeltaTime)
{
	DeltaTimeSinceLastPDU += DeltaTime;

	if (PerformDeadReckoning && SpawnedFromNetwork)
	{
		SCOPE_CYCLE_COUNTER(STAT_DoDeadReckoning);
		//Check if Dead Reckoning updates should be culled or not.
		if (DISCullingMode == EDISCullingMode::CullDeadReckoning || DISCullingMode == EDISCullingMode::CullAll)
		{
			//If so, get the player camera and cull beyond specified distance.
			APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
			if (camManager)
			{
				FVector userCameraLocation = camManager->GetCameraLocation();
				float distanceToUser = UKismetMathLibrary::Vector_Distance(GetOwner()->GetActorLocation(), userCameraLocation);

				if (distanceToUser > DISCullingDistance)
				{
					//In case users are relying on Dead Reckoning for their entity movement, just send them the most recent Dead Reckoned PDU again
					OnDeadReckoningUpdate.Broadcast(MostRecentDeadReckonedEntityStatePDU);
					return;
				}
			}
		}

		if (UDeadReckoning_BPFL::DeadReckoning(MostRecentEntityStatePDU, DeltaTimeSinceLastPDU, MostRecentDeadReckonedEntityStatePDU))
		{
			//If more than one PDU has been received and we're still in the smoothing period, then smooth
			if (PerformDeadReckoningSmoothing && NumberEntityStatePDUsReceived > 1 && DeltaTimeSinceLastPDU <= DeadReckoningSmoothingPeriodSeconds)
			{
				MostRecentDeadReckonedEntityStatePDU = SmoothDeadReckoning(MostRecentDeadReckonedEntityStatePDU);
			}

			OnDeadReckoningUpdate.Broadcast(MostRecentDeadReckonedEntityStatePDU);
		}

		//Perform ground clamping last
		GroundClamping_Implementation();
	}
}

void UDISReceiveComponent::GroundClamping_Implementation()
{
	//Verify that ground clamping is enabled, the entity is owned by another sim, is of the ground domain, and that it is not a munition
	if (SpawnedFromNetwork && (PerformGroundClamping == EGroundClampingMode::AlwaysGroundClamp || (PerformGroundClamping == EGroundClampingMode::GroundClampWithDISOptions && EntityType.Domain == 1 && EntityType.EntityKind != 2)))
	{
		SCOPE_CYCLE_COUNTER(STAT_GroundClamping);

		//Set clamp direction using the North East Down down vector
		FVector clampDirection = FVector::DownVector;

		FVector eastVector = FVector::RightVector;
		FVector northVector = FVector::ForwardVector;

		if (IsValid(GeoReferencingSystem))
		{
			GeoReferencingSystem->GetENUVectorsAtECEFLocation(MostRecentDeadReckonedEntityStatePDU.EcefLocation, eastVector, northVector, clampDirection);
		}
		else
		{
			UE_LOG(LogDISReceiveComponent, Warning, TEXT("Invalid GeoReferencing variable in DISComponent. Error in calculating East, North, Down vectors for Ground Clamp location. Utilizing default East, North, Down vectors for calculation."));
		}

		clampDirection *= -1;

		//Get the location the object is supposed to be at according to the most recent dead reckoning update.
		FVector actorLocation;
		UDIS_BPFL::GetUnrealLocationFromEntityStatePdu(MostRecentDeadReckonedEntityStatePDU, GeoReferencingSystem, actorLocation);

		FHitResult lineTraceHitResult;
		FVector endLocation = (clampDirection * 100000) + actorLocation;
		FVector aboveActorStartLocation = (clampDirection * -100000) + actorLocation;

		FCollisionQueryParams queryParams = FCollisionQueryParams(FName("Ground Clamping"), false, GetOwner());
		//Find colliding point above/below the actor
		if (GetWorld()->LineTraceSingleByChannel(lineTraceHitResult, aboveActorStartLocation, endLocation, UEngineTypes::ConvertToCollisionChannel(GoundClampingCollisionChannel), queryParams))
		{
			FVector clampLocation = lineTraceHitResult.Location;
			//Calculate what the new forward and right vectors should be based on the impact normal
			FVector newForward = FVector::CrossProduct(GetOwner()->GetActorRightVector(), lineTraceHitResult.ImpactNormal);
			FVector newRight = FVector::CrossProduct(lineTraceHitResult.ImpactNormal, newForward);

			FRotator clampRotation = UKismetMathLibrary::MakeRotationFromAxes(newForward, newRight, lineTraceHitResult.ImpactNormal);

			//Create clamp transform and broadcast
			FTransform clampTransform = FTransform(clampRotation, clampLocation);
			TArray<FTransform> allClampTransforms;
			allClampTransforms.Add(clampTransform);

			OnGroundClampingUpdate.Broadcast(allClampTransforms);
		}
	}
}

FEntityStatePDU UDISReceiveComponent::SmoothDeadReckoning(FEntityStatePDU DeadReckonPDUToSmooth)
{
	FEntityStatePDU SmoothedDeadReckonPDU = DeadReckonPDUToSmooth;

	float alpha = UKismetMathLibrary::MapRangeClamped(DeltaTimeSinceLastPDU, 0.0f, DeadReckoningSmoothingPeriodSeconds, 0.0f, 1.0f);

	//Lerp location and rotation for smoothing
	SmoothedDeadReckonPDU.EcefLocation -= FMath::Lerp(EntityECEFLocationDifference, FVector(0, 0, 0), alpha);
	SmoothedDeadReckonPDU.EntityOrientation -= FMath::Lerp(EntityRotationDifference, FRotator(0, 0, 0), alpha);

	return SmoothedDeadReckonPDU;
}