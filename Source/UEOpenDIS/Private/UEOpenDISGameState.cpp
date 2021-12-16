// Fill out your copyright notice in the Description page of Project Settings.

#include "UEOpenDISGameState.h"
#include "UEOpenDISRuntimeSettings.h"
#include "OpenDISComponent.h"

// Called when the game starts
void AUEOpenDISGameState::BeginPlay()
{
	Super::BeginPlay();

	//Load settings from the OpenDIS ProjectSettings
	const UUEOpenDISRuntimeSettings* const Settings = UUEOpenDISRuntimeSettings::Get();
	check(Settings);
	//Initialize DISClassMappings from the loaded settings
	for (FOpenDISEnumerationMappings DISMapping : Settings->DISClassMappings)
	{
		for (FEntityType EntityType : DISMapping.AssociatedDISEnumerations)
		{
			DISClassMappings.Add(EntityType, DISMapping.DISEntity);
		}
	}

	ExerciseID = Settings->ExerciseID;
	SiteID = Settings->SiteID;
	ApplicationID = Settings->ApplicationID;
	IPAddress = Settings->IPAddress;
	Port = Settings->Port;
}

bool AUEOpenDISGameState::OpenReceiveSocket(FString InListenIP, int32 InListenPort)
{
	// TODO: Implement opening receive socket for UDP
	// May want to handle actual UDP related stuff in a separate class

	return false;
}

bool AUEOpenDISGameState::OpenSendSocket(FString InIP, int32 InPort)
{
	// TODO: Implement opening send socket for UDP
	// May want to handle actual UDP related stuff in a separate class

	return false;
}

bool AUEOpenDISGameState::CloseReceiveSocket()
{
	// TODO: Implement closing receive socket for UDP
	// May want to handle actual UDP related stuff in a separate class

	return false;
}

bool AUEOpenDISGameState::CloseSendSocket()
{
	// TODO: Implement closing send socket for UDP
	// May want to handle actual UDP related stuff in a separate class

	return false;
}

void AUEOpenDISGameState::ProcessDISPacket(int ByteArrayLength, TArray<uint8> InData, int& OutType)
{
	//UE_LOG(LogTemp, Log, TEXT("trying to process"));
	if (ByteArrayLength < 1)
	{
		return;
	}

	DIS::Pdu* pdu = NULL;

	pdu = DIS::PduBank::GetStaticPDU(static_cast<DIS::PDUType>(InData[PDU_TYPE_POSITION]));
	if (pdu)
	{
		//for list of enums for pdu type refer to siso ref 010 2015, ANNEX A
		switch (pdu->getPduType())
		{
			//entity state
		case 1:
		{
			DIS::DataStream ds((char*)&InData[0], ByteArrayLength, BigEndian);
			pdu->unmarshal(ds);
			// TODO: Verify that EntityStatePDU is being handled correctly.
			FEntityStatePDU entityStatePDU = ConvertESPDUtoBPStruct(static_cast<DIS::EntityStatePdu&>(*pdu));

			//Find associated actor in the DISActorMappings map
			AActor* associatedActor = *DISActorMappings.Find(entityStatePDU.EntityID);
			if (associatedActor != nullptr)
			{
				//If an actor was found, relay information to the associated component
				UOpenDISComponent* DISComponent = (UOpenDISComponent*)associatedActor->FindComponentByClass(UOpenDISComponent::StaticClass());

				if (DISComponent != nullptr)
				{
					DISComponent->HandleEntityStatePDU(entityStatePDU);
				}
			}
			else
			{
				//If an actor was not found -- check to see if there is an associated actor for the entity type
				TAssetSubclassOf<AActor>* associatedSoftClassReference = DISClassMappings.Find(entityStatePDU.EntityType);
				if (associatedSoftClassReference != nullptr)
				{
					//If so, spawn one and relay information to the associated component
					UClass* associatedClass = associatedSoftClassReference->LoadSynchronous();
					if (associatedClass != nullptr)
					{
						AActor* spawnedActor = GetWorld()->SpawnActor(associatedClass);
						UOpenDISComponent* DISComponent = (UOpenDISComponent*)spawnedActor->FindComponentByClass(UOpenDISComponent::StaticClass());
						//Add actor to the map
						AddDISEntityToMap(entityStatePDU.EntityID, spawnedActor);

						if (DISComponent != nullptr)
						{
							DISComponent->HandleEntityStatePDU(entityStatePDU);
						}
					}
				}
				else
				{
					//Otherwise notify the user that no such mapping exists
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("No mapping between an actor and the DIS enumeration %s exists!"), *entityStatePDU.EntityType.ToString()));
				}
			}

			break;
		}
		//fire
		case 2:
		{
			DIS::DataStream ds((char*)&InData[0], ByteArrayLength, BigEndian);
			pdu->unmarshal(ds);
			// TODO: Verify that FirePDU is being handled correctly.
			FFirePDU firePDU = ConvertFirePDUtoBPStruct(static_cast<DIS::FirePdu&>(*pdu));

			//Get associated OpenDISComponent and relay information
			UOpenDISComponent* DISComponent = GetAssociatedOpenDISComponent(firePDU.MunitionEntityID);

			if (DISComponent != nullptr)
			{
				DISComponent->HandleFirePDU(firePDU);
			}

			break;
		}
		//detonation
		case 3:
		{
			DIS::DataStream ds((char*)&InData[0], ByteArrayLength, BigEndian);
			pdu->unmarshal(ds);
			// TODO: Verify that DetonationPDU is being handled correctly.
			FDetonationPDU detPDU = ConvertDetonationPDUtoBPStruct(static_cast<DIS::DetonationPdu&>(*pdu));

			//Get associated OpenDISComponent and relay information
			UOpenDISComponent* DISComponent = GetAssociatedOpenDISComponent(detPDU.MunitionEntityID);

			if (DISComponent != nullptr)
			{
				DISComponent->HandleDetonationPDU(detPDU);
			}

			break;
		}
		//remove entity
		case 12:
		{
			DIS::DataStream ds((char*)&InData[0], ByteArrayLength, BigEndian);
			pdu->unmarshal(ds);
			// TODO: Verify that RemoveEntityPDU is being handled correctly.
			// TODO: Ignore RemoveEntityPDU if it is not meant for our sim. Only the sim dictated in the pdu should handle the remove request.
			FRemoveEntityPDU removeEntityPDU = ConvertRemoveEntityPDUtoBPStruct(static_cast<DIS::RemoveEntityPdu&>(*pdu));

			//Verify that we are the appropriate sim to handle the RemoveEntityPDU
			if (removeEntityPDU.ReceivingEntityID.Site == SiteID && removeEntityPDU.ReceivingEntityID.Application == ApplicationID)
			{
				//Get associated OpenDISComponent and relay information
				UOpenDISComponent* DISComponent = GetAssociatedOpenDISComponent(removeEntityPDU.ReceivingEntityID);

				if (DISComponent != nullptr)
				{
					DISComponent->HandleRemoveEntityPDU(removeEntityPDU);
				}
			}
			break;
		}
		//start/resume
		case 13:
		{
			// TODO: Handle start/resume PDUs accordingly
			break;
		}
		//stop/freeze
		case 14:
		{
			// TODO: Handle stop/freeze PDUs accordingly
			break;
		}
		//entity state update
		case 67:
		{
			// TODO: Handle EntityStateUpdate PDUs accordingly
			break;
		}
		default:
		{
			break;
		}
		}
	}
}

UOpenDISComponent* AUEOpenDISGameState::GetAssociatedOpenDISComponent(FEntityID EntityIDIn)
{
	UOpenDISComponent* DISComponent = nullptr;

	//Find associated actor in the DISActorMappings map
	AActor* associatedActor = *DISActorMappings.Find(EntityIDIn);
	if (associatedActor != nullptr)
	{
		//If an actor was found, relay information to the associated component
		DISComponent = (UOpenDISComponent*)associatedActor->FindComponentByClass(UOpenDISComponent::StaticClass());
	}

	return DISComponent;
}

void AUEOpenDISGameState::AddDISEntityToMap(FEntityID EntityIDToAdd, AActor* EntityToAdd)
{
	DISActorMappings.Add(EntityIDToAdd, EntityToAdd);
}

bool AUEOpenDISGameState::RemoveDISEntityFromMap(FEntityID EntityIDToRemove)
{
	int amountRemoved = DISActorMappings.Remove(EntityIDToRemove);

	return (amountRemoved > 0);
}

void AUEOpenDISGameState::ConvertESPDU2Bytes(int EntityID, int Site, int Application, int Exercise, FEntityStatePDU EntityStatePDUIn, TArray<uint8>& BytesOut)
{
	DIS::DataStream buffer(BigEndian);
	//protocol and exercise
	DIS::EntityStatePdu tempEntity;
	tempEntity.setProtocolVersion(6);
	tempEntity.setExerciseID(Exercise);
	//entity id
	DIS::EntityID tempID;
	tempID.setSite(Site);
	tempID.setApplication(Application);
	tempID.setEntity(EntityID);
	tempEntity.setEntityID(tempID);
	//entity type
	DIS::EntityType tempType;
	tempType.setCategory(EntityStatePDUIn.EntityType.Category);
	tempType.setCountry(EntityStatePDUIn.EntityType.Country);
	tempType.setDomain(EntityStatePDUIn.EntityType.Domain);
	tempType.setEntityKind(EntityStatePDUIn.EntityType.EntityKind);
	tempType.setExtra(EntityStatePDUIn.EntityType.Extra);
	tempType.setSpecific(EntityStatePDUIn.EntityType.Specific);
	tempType.setSubcategory(EntityStatePDUIn.EntityType.Subcategory);
	tempEntity.setEntityType(tempType);
	//dead reckoning
	DIS::DeadReckoningParameter drp;
	drp.setDeadReckoningAlgorithm(4);
	DIS::Vector3Float tempFloat;
	tempFloat.setX(EntityStatePDUIn.DeadReckoningParameters.EntityAngularVelocity.X);
	tempFloat.setY(EntityStatePDUIn.DeadReckoningParameters.EntityAngularVelocity.Y);
	tempFloat.setZ(EntityStatePDUIn.DeadReckoningParameters.EntityAngularVelocity.Z);
	drp.setEntityAngularVelocity(tempFloat);
	tempFloat.setX(EntityStatePDUIn.DeadReckoningParameters.EntityLinearAcceleration.X);
	tempFloat.setY(EntityStatePDUIn.DeadReckoningParameters.EntityLinearAcceleration.Y);
	tempFloat.setZ(EntityStatePDUIn.DeadReckoningParameters.EntityLinearAcceleration.Z);
	drp.setEntityLinearAcceleration(tempFloat);
	tempEntity.setDeadReckoningParameters(drp);
	//location FIX TO USE DOUBLE LATER, USING FLOAT FOR SIMPLE TESTING
	DIS::Vector3Double tempDouble;
	tempDouble.setX(EntityStatePDUIn.EntityLocation[0]);
	tempDouble.setY(EntityStatePDUIn.EntityLocation[1]);
	tempDouble.setZ(EntityStatePDUIn.EntityLocation[2]);
	tempEntity.setEntityLocation(tempDouble);
	//rotation
	DIS::Orientation tempOri;
	tempOri.setPhi(0.0);
	tempOri.setPsi(0.0);
	tempOri.setTheta(0.0);
	tempEntity.setEntityOrientation(tempOri);
	//marking
	DIS::Marking tempMarking;
	tempMarking.setCharacterSet(1);
	tempMarking.setByStringCharacters(TCHAR_TO_ANSI(*EntityStatePDUIn.Marking));
	tempEntity.setMarking(tempMarking);
	//marshal
	tempEntity.marshal(buffer);
	TArray<uint8> tempBytes;
	tempBytes.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		tempBytes[i] = buffer[i];
	}
	buffer.clear();
	BytesOut = tempBytes;
}

FEntityStatePDU AUEOpenDISGameState::ConvertESPDUtoBPStruct(DIS::EntityStatePdu& EntityStatePDUOut)
{
	FEntityStatePDU entityStatePDU;
	DIS::Vector3Double& position = EntityStatePDUOut.getEntityLocation();
	DIS::Orientation& rotation = EntityStatePDUOut.getEntityOrientation();
	const DIS::EntityID EntityID = EntityStatePDUOut.getEntityID();
	const DIS::EntityType EntityType = EntityStatePDUOut.getEntityType();

	//pure since unsupported in BP
	entityStatePDU.EntityLocationDouble[0] = position.getX();
	entityStatePDU.EntityLocationDouble[1] = position.getY();
	entityStatePDU.EntityLocationDouble[2] = position.getZ();

	//checked

	//entity id
	entityStatePDU.EntityID.Site = EntityID.getSite();
	entityStatePDU.EntityID.Application = EntityID.getApplication();
	entityStatePDU.EntityID.Entity = EntityID.getEntity();

	//location
	entityStatePDU.EntityLocation[0] = position.getX();
	entityStatePDU.EntityLocation[1] = position.getY();
	entityStatePDU.EntityLocation[2] = position.getZ();

	//rotation
	entityStatePDU.EntityOrientation.Yaw = rotation.getPhi();
	entityStatePDU.EntityOrientation.Roll = rotation.getPsi();
	entityStatePDU.EntityOrientation.Pitch = rotation.getTheta();

	//velocity (originally in float so this is fine)
	entityStatePDU.EntityLinearVelocity[0] = EntityStatePDUOut.getEntityLinearVelocity().getX();
	entityStatePDU.EntityLinearVelocity[1] = EntityStatePDUOut.getEntityLinearVelocity().getY();
	entityStatePDU.EntityLinearVelocity[2] = EntityStatePDUOut.getEntityLinearVelocity().getZ();

	//Dead reckoning
	entityStatePDU.DeadReckoningParameters.DeadReckoningAlgorithm = EntityStatePDUOut.getDeadReckoningParameters().getDeadReckoningAlgorithm();
	//figure out how to get the character buffer of 15 8bits and put it into tarray of 15 elements each with 8bits
	//returnStruct.DeadReckoningParameters.OtherParameters = espdu.getDeadReckoningParameters().getOtherParameters();
	entityStatePDU.DeadReckoningParameters.EntityLinearAcceleration[0] = EntityStatePDUOut.getDeadReckoningParameters().getEntityLinearAcceleration().getX();
	entityStatePDU.DeadReckoningParameters.EntityLinearAcceleration[1] = EntityStatePDUOut.getDeadReckoningParameters().getEntityLinearAcceleration().getY();
	entityStatePDU.DeadReckoningParameters.EntityLinearAcceleration[2] = EntityStatePDUOut.getDeadReckoningParameters().getEntityLinearAcceleration().getZ();
	entityStatePDU.DeadReckoningParameters.EntityAngularVelocity[0] = EntityStatePDUOut.getDeadReckoningParameters().getEntityAngularVelocity().getX();
	entityStatePDU.DeadReckoningParameters.EntityAngularVelocity[1] = EntityStatePDUOut.getDeadReckoningParameters().getEntityAngularVelocity().getY();
	entityStatePDU.DeadReckoningParameters.EntityAngularVelocity[2] = EntityStatePDUOut.getDeadReckoningParameters().getEntityAngularVelocity().getZ();

	//single vars
	entityStatePDU.ForceID = static_cast<EForceID>(EntityStatePDUOut.getForceId());
	entityStatePDU.Marking = FString(EntityStatePDUOut.getMarking().getCharacters());
	entityStatePDU.PduType = EntityStatePDUOut.getPduType();
	entityStatePDU.EntityAppearance = EntityStatePDUOut.getEntityAppearance();
	entityStatePDU.NumberOfArticulationParameters = EntityStatePDUOut.getNumberOfArticulationParameters();
	entityStatePDU.Capabilities = EntityStatePDUOut.getCapabilities();

	//Entity type
	entityStatePDU.EntityType.EntityKind = EntityType.getEntityKind();
	entityStatePDU.EntityType.Domain = EntityType.getDomain();
	entityStatePDU.EntityType.Country = EntityType.getCountry();
	entityStatePDU.EntityType.Category = EntityType.getCategory();
	entityStatePDU.EntityType.Subcategory = EntityType.getSubcategory();
	entityStatePDU.EntityType.Specific = EntityType.getSpecific();
	entityStatePDU.EntityType.Extra = EntityType.getExtra();

	return entityStatePDU;
}

FFirePDU AUEOpenDISGameState::ConvertFirePDUtoBPStruct(DIS::FirePdu& FirePDUOut)
{
	FFirePDU firePDU;

	//single vars
	firePDU.fireMissionIndex = FirePDUOut.getFireMissionIndex();
	firePDU.range = FirePDUOut.getRange();

	//MunitionEntityID
	firePDU.MunitionEntityID.Site = FirePDUOut.getMunitionID().getSite();
	firePDU.MunitionEntityID.Application = FirePDUOut.getMunitionID().getApplication();
	firePDU.MunitionEntityID.Entity = FirePDUOut.getMunitionID().getEntity();

	//velocity
	firePDU.velocity[0] = FirePDUOut.getVelocity().getX();
	firePDU.velocity[1] = FirePDUOut.getVelocity().getY();
	firePDU.velocity[2] = FirePDUOut.getVelocity().getZ();

	//location
	firePDU.location[0] = FirePDUOut.getLocationInWorldCoordinates().getX();
	firePDU.location[1] = FirePDUOut.getLocationInWorldCoordinates().getY();
	firePDU.location[2] = FirePDUOut.getLocationInWorldCoordinates().getZ();

	//locationDouble
	firePDU.locationDouble[0] = FirePDUOut.getLocationInWorldCoordinates().getX();
	firePDU.locationDouble[1] = FirePDUOut.getLocationInWorldCoordinates().getY();
	firePDU.locationDouble[2] = FirePDUOut.getLocationInWorldCoordinates().getZ();

	//event id
	firePDU.eventID.Site = FirePDUOut.getEventID().getSite();
	firePDU.eventID.Application = FirePDUOut.getEventID().getApplication();
	firePDU.eventID.EventID = FirePDUOut.getEventID().getEventNumber();

	//burst descriptor
	firePDU.burstDescriptor.warhead = FirePDUOut.getBurstDescriptor().getWarhead();
	firePDU.burstDescriptor.fuse = FirePDUOut.getBurstDescriptor().getFuse();
	firePDU.burstDescriptor.rate = FirePDUOut.getBurstDescriptor().getRate();
	firePDU.burstDescriptor.quantity = FirePDUOut.getBurstDescriptor().getQuantity();
	firePDU.burstDescriptor.EntityType.EntityKind = FirePDUOut.getBurstDescriptor().getMunition().getEntityKind();
	firePDU.burstDescriptor.EntityType.Domain = FirePDUOut.getBurstDescriptor().getMunition().getDomain();
	firePDU.burstDescriptor.EntityType.Country = FirePDUOut.getBurstDescriptor().getMunition().getCountry();
	firePDU.burstDescriptor.EntityType.Category = FirePDUOut.getBurstDescriptor().getMunition().getCategory();
	firePDU.burstDescriptor.EntityType.Subcategory = FirePDUOut.getBurstDescriptor().getMunition().getSubcategory();
	firePDU.burstDescriptor.EntityType.Specific = FirePDUOut.getBurstDescriptor().getMunition().getSpecific();
	firePDU.burstDescriptor.EntityType.Extra = FirePDUOut.getBurstDescriptor().getMunition().getExtra();

	return firePDU;
}

FDetonationPDU AUEOpenDISGameState::ConvertDetonationPDUtoBPStruct(DIS::DetonationPdu& DetPDUOut)
{
	FDetonationPDU detonationPDU;

	//MunitionEntityID
	detonationPDU.MunitionEntityID.Site = DetPDUOut.getMunitionID().getSite();
	detonationPDU.MunitionEntityID.Application = DetPDUOut.getMunitionID().getApplication();
	detonationPDU.MunitionEntityID.Entity = DetPDUOut.getMunitionID().getEntity();

	//event id
	detonationPDU.eventID.Site = DetPDUOut.getEventID().getSite();
	detonationPDU.eventID.Application = DetPDUOut.getEventID().getApplication();
	detonationPDU.eventID.EventID = DetPDUOut.getEventID().getEventNumber();

	//velocity
	detonationPDU.velocity[0] = DetPDUOut.getVelocity().getX();
	detonationPDU.velocity[1] = DetPDUOut.getVelocity().getY();
	detonationPDU.velocity[2] = DetPDUOut.getVelocity().getZ();

	//location
	detonationPDU.location[0] = DetPDUOut.getLocationInWorldCoordinates().getX();
	detonationPDU.location[1] = DetPDUOut.getLocationInWorldCoordinates().getY();
	detonationPDU.location[2] = DetPDUOut.getLocationInWorldCoordinates().getZ();

	//locationDouble
	detonationPDU.locationDouble[0] = DetPDUOut.getLocationInWorldCoordinates().getX();
	detonationPDU.locationDouble[1] = DetPDUOut.getLocationInWorldCoordinates().getY();
	detonationPDU.locationDouble[2] = DetPDUOut.getLocationInWorldCoordinates().getZ();

	//location
	detonationPDU.locationInEntityCoords[0] = DetPDUOut.getLocationInEntityCoordinates().getX();
	detonationPDU.locationInEntityCoords[1] = DetPDUOut.getLocationInEntityCoordinates().getY();
	detonationPDU.locationInEntityCoords[2] = DetPDUOut.getLocationInEntityCoordinates().getZ();

	//burst descriptor
	detonationPDU.burstDescriptor.warhead = DetPDUOut.getBurstDescriptor().getWarhead();
	detonationPDU.burstDescriptor.fuse = DetPDUOut.getBurstDescriptor().getFuse();
	detonationPDU.burstDescriptor.rate = DetPDUOut.getBurstDescriptor().getRate();
	detonationPDU.burstDescriptor.quantity = DetPDUOut.getBurstDescriptor().getQuantity();
	detonationPDU.burstDescriptor.EntityType.EntityKind = DetPDUOut.getBurstDescriptor().getMunition().getEntityKind();
	detonationPDU.burstDescriptor.EntityType.Domain = DetPDUOut.getBurstDescriptor().getMunition().getDomain();
	detonationPDU.burstDescriptor.EntityType.Country = DetPDUOut.getBurstDescriptor().getMunition().getCountry();
	detonationPDU.burstDescriptor.EntityType.Category = DetPDUOut.getBurstDescriptor().getMunition().getCategory();
	detonationPDU.burstDescriptor.EntityType.Subcategory = DetPDUOut.getBurstDescriptor().getMunition().getSubcategory();
	detonationPDU.burstDescriptor.EntityType.Specific = DetPDUOut.getBurstDescriptor().getMunition().getSpecific();
	detonationPDU.burstDescriptor.EntityType.Extra = DetPDUOut.getBurstDescriptor().getMunition().getExtra();

	//single vars
	detonationPDU.detonationResult = DetPDUOut.getDetonationResult();
	detonationPDU.NumberOfArticulationParameters = DetPDUOut.getNumberOfArticulationParameters();
	detonationPDU.pad = DetPDUOut.getPad();

	return detonationPDU;
}

FRemoveEntityPDU AUEOpenDISGameState::ConvertRemoveEntityPDUtoBPStruct(DIS::RemoveEntityPdu& RemovePDUOut)
{
	FRemoveEntityPDU removeEntityPDU;

	removeEntityPDU.OriginatingEntityID.Site = RemovePDUOut.getOriginatingEntityID().getSite();
	removeEntityPDU.OriginatingEntityID.Application = RemovePDUOut.getOriginatingEntityID().getApplication();
	removeEntityPDU.OriginatingEntityID.Entity = RemovePDUOut.getOriginatingEntityID().getEntity();
	removeEntityPDU.ReceivingEntityID.Site = RemovePDUOut.getReceivingEntityID().getSite();
	removeEntityPDU.ReceivingEntityID.Application = RemovePDUOut.getReceivingEntityID().getApplication();
	removeEntityPDU.ReceivingEntityID.Entity = RemovePDUOut.getReceivingEntityID().getEntity();
	removeEntityPDU.RequestID = RemovePDUOut.getRequestID();

	return removeEntityPDU;
}