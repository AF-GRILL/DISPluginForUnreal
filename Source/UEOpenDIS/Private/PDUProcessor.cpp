// Fill out your copyright notice in the Description page of Project Settings.

#include "PDUProcessor.h"
#include "UDPSubsystem.h"

void UPDUProcessor::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency(UUDPSubsystem::StaticClass());
	Super::Initialize(Collection);

	//Get the UDP Subsystem and bind to receiving UDP Bytes
	GetGameInstance()->GetSubsystem<UUDPSubsystem>()->OnReceivedBytes.AddDynamic(this, &UPDUProcessor::HandleOnReceivedUDPBytes);
}

void UPDUProcessor::Deinitialize()
{
	Super::Deinitialize();
}

void UPDUProcessor::HandleOnReceivedUDPBytes(const TArray<uint8>& Bytes, const FString& IPAddress)
{
	ProcessDISPacket(Bytes);
}

void UPDUProcessor::ProcessDISPacket(TArray<uint8> InData)
{
	int bytesArrayLength = InData.Num();

	if (bytesArrayLength < 1)
	{
		return;
	}

	DIS::Pdu* pdu = NULL;

	pdu = DIS::PduBank::GetStaticPDU(static_cast<DIS::PDUType>(InData[PDU_TYPE_POSITION]));
	if (pdu)
	{
		//for list of enums for pdu type refer to siso ref 010 2015, ANNEX A
		switch (static_cast<EPDUType>(pdu->getPduType()))
		{
			//entity state
		case EPDUType::EntityState:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FEntityStatePDU entityStatePDU = ConvertESPDUtoBPStruct(static_cast<DIS::EntityStatePdu&>(*pdu));

			OnEntityStatePDUProcessed.Broadcast(entityStatePDU);

			break;
		}
		//fire
		case EPDUType::Fire:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FFirePDU firePDU = ConvertFirePDUtoBPStruct(static_cast<DIS::FirePdu&>(*pdu));

			OnFirePDUProcessed.Broadcast(firePDU);

			break;
		}
		//detonation
		case EPDUType::Detonation:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FDetonationPDU detPDU = ConvertDetonationPDUtoBPStruct(static_cast<DIS::DetonationPdu&>(*pdu));

			OnDetonationPDUProcessed.Broadcast(detPDU);

			break;
		}
		//remove entity
		case EPDUType::RemoveEntity:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FRemoveEntityPDU removeEntityPDU = ConvertRemoveEntityPDUtoBPStruct(static_cast<DIS::RemoveEntityPdu&>(*pdu));

			OnRemoveEntityPDUProcessed.Broadcast(removeEntityPDU);

			break;
		}
		//start/resume
		case EPDUType::Start_Resume:
		{
			// TODO: Handle start/resume PDUs accordingly
			break;
		}
		//stop/freeze
		case EPDUType::Stop_Freeze:
		{
			// TODO: Handle stop/freeze PDUs accordingly
			break;
		}
		//entity state update
		case EPDUType::EntityStateUpdate:
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

void UPDUProcessor::ConvertESPDU2Bytes(int EntityID, int Site, int Application, int Exercise, FEntityStatePDU EntityStatePDUIn, TArray<uint8>& BytesOut)
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

	// TODO: location FIX TO USE DOUBLE LATER, USING FLOAT FOR SIMPLE TESTING
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

FEntityStatePDU UPDUProcessor::ConvertESPDUtoBPStruct(DIS::EntityStatePdu& EntityStatePDUOut)
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
	// TODO: figure out how to get the character buffer of 15 8bits and put it into tarray of 15 elements each with 8bits
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
	//entityStatePDU.PduType = EntityStatePDUOut.getPduType();
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

FFirePDU UPDUProcessor::ConvertFirePDUtoBPStruct(DIS::FirePdu& FirePDUOut)
{
	FFirePDU firePDU;

	//single vars
	firePDU.FireMissionIndex = FirePDUOut.getFireMissionIndex();
	firePDU.Range = FirePDUOut.getRange();

	//MunitionEntityID
	firePDU.MunitionEntityID.Site = FirePDUOut.getMunitionID().getSite();
	firePDU.MunitionEntityID.Application = FirePDUOut.getMunitionID().getApplication();
	firePDU.MunitionEntityID.Entity = FirePDUOut.getMunitionID().getEntity();

	//velocity
	firePDU.Velocity[0] = FirePDUOut.getVelocity().getX();
	firePDU.Velocity[1] = FirePDUOut.getVelocity().getY();
	firePDU.Velocity[2] = FirePDUOut.getVelocity().getZ();

	//location
	firePDU.Location[0] = FirePDUOut.getLocationInWorldCoordinates().getX();
	firePDU.Location[1] = FirePDUOut.getLocationInWorldCoordinates().getY();
	firePDU.Location[2] = FirePDUOut.getLocationInWorldCoordinates().getZ();

	//locationDouble
	firePDU.LocationDouble[0] = FirePDUOut.getLocationInWorldCoordinates().getX();
	firePDU.LocationDouble[1] = FirePDUOut.getLocationInWorldCoordinates().getY();
	firePDU.LocationDouble[2] = FirePDUOut.getLocationInWorldCoordinates().getZ();

	//event id
	firePDU.EventID.Site = FirePDUOut.getEventID().getSite();
	firePDU.EventID.Application = FirePDUOut.getEventID().getApplication();
	firePDU.EventID.EventID = FirePDUOut.getEventID().getEventNumber();

	//burst descriptor
	firePDU.BurstDescriptor.Warhead = FirePDUOut.getBurstDescriptor().getWarhead();
	firePDU.BurstDescriptor.Fuse = FirePDUOut.getBurstDescriptor().getFuse();
	firePDU.BurstDescriptor.Rate = FirePDUOut.getBurstDescriptor().getRate();
	firePDU.BurstDescriptor.Quantity = FirePDUOut.getBurstDescriptor().getQuantity();
	firePDU.BurstDescriptor.EntityType.EntityKind = FirePDUOut.getBurstDescriptor().getMunition().getEntityKind();
	firePDU.BurstDescriptor.EntityType.Domain = FirePDUOut.getBurstDescriptor().getMunition().getDomain();
	firePDU.BurstDescriptor.EntityType.Country = FirePDUOut.getBurstDescriptor().getMunition().getCountry();
	firePDU.BurstDescriptor.EntityType.Category = FirePDUOut.getBurstDescriptor().getMunition().getCategory();
	firePDU.BurstDescriptor.EntityType.Subcategory = FirePDUOut.getBurstDescriptor().getMunition().getSubcategory();
	firePDU.BurstDescriptor.EntityType.Specific = FirePDUOut.getBurstDescriptor().getMunition().getSpecific();
	firePDU.BurstDescriptor.EntityType.Extra = FirePDUOut.getBurstDescriptor().getMunition().getExtra();

	return firePDU;
}

FDetonationPDU UPDUProcessor::ConvertDetonationPDUtoBPStruct(DIS::DetonationPdu& DetPDUOut)
{
	FDetonationPDU detonationPDU;

	//MunitionEntityID
	detonationPDU.MunitionEntityID.Site = DetPDUOut.getMunitionID().getSite();
	detonationPDU.MunitionEntityID.Application = DetPDUOut.getMunitionID().getApplication();
	detonationPDU.MunitionEntityID.Entity = DetPDUOut.getMunitionID().getEntity();

	//event id
	detonationPDU.EventID.Site = DetPDUOut.getEventID().getSite();
	detonationPDU.EventID.Application = DetPDUOut.getEventID().getApplication();
	detonationPDU.EventID.EventID = DetPDUOut.getEventID().getEventNumber();

	//velocity
	detonationPDU.Velocity[0] = DetPDUOut.getVelocity().getX();
	detonationPDU.Velocity[1] = DetPDUOut.getVelocity().getY();
	detonationPDU.Velocity[2] = DetPDUOut.getVelocity().getZ();

	//location
	detonationPDU.Location[0] = DetPDUOut.getLocationInWorldCoordinates().getX();
	detonationPDU.Location[1] = DetPDUOut.getLocationInWorldCoordinates().getY();
	detonationPDU.Location[2] = DetPDUOut.getLocationInWorldCoordinates().getZ();

	//locationDouble
	detonationPDU.LocationDouble[0] = DetPDUOut.getLocationInWorldCoordinates().getX();
	detonationPDU.LocationDouble[1] = DetPDUOut.getLocationInWorldCoordinates().getY();
	detonationPDU.LocationDouble[2] = DetPDUOut.getLocationInWorldCoordinates().getZ();

	//location
	detonationPDU.LocationInEntityCoords[0] = DetPDUOut.getLocationInEntityCoordinates().getX();
	detonationPDU.LocationInEntityCoords[1] = DetPDUOut.getLocationInEntityCoordinates().getY();
	detonationPDU.LocationInEntityCoords[2] = DetPDUOut.getLocationInEntityCoordinates().getZ();

	//burst descriptor
	detonationPDU.BurstDescriptor.Warhead = DetPDUOut.getBurstDescriptor().getWarhead();
	detonationPDU.BurstDescriptor.Fuse = DetPDUOut.getBurstDescriptor().getFuse();
	detonationPDU.BurstDescriptor.Rate = DetPDUOut.getBurstDescriptor().getRate();
	detonationPDU.BurstDescriptor.Quantity = DetPDUOut.getBurstDescriptor().getQuantity();
	detonationPDU.BurstDescriptor.EntityType.EntityKind = DetPDUOut.getBurstDescriptor().getMunition().getEntityKind();
	detonationPDU.BurstDescriptor.EntityType.Domain = DetPDUOut.getBurstDescriptor().getMunition().getDomain();
	detonationPDU.BurstDescriptor.EntityType.Country = DetPDUOut.getBurstDescriptor().getMunition().getCountry();
	detonationPDU.BurstDescriptor.EntityType.Category = DetPDUOut.getBurstDescriptor().getMunition().getCategory();
	detonationPDU.BurstDescriptor.EntityType.Subcategory = DetPDUOut.getBurstDescriptor().getMunition().getSubcategory();
	detonationPDU.BurstDescriptor.EntityType.Specific = DetPDUOut.getBurstDescriptor().getMunition().getSpecific();
	detonationPDU.BurstDescriptor.EntityType.Extra = DetPDUOut.getBurstDescriptor().getMunition().getExtra();

	//single vars
	detonationPDU.DetonationResult = DetPDUOut.getDetonationResult();
	detonationPDU.NumberOfArticulationParameters = DetPDUOut.getNumberOfArticulationParameters();
	detonationPDU.Pad = DetPDUOut.getPad();

	return detonationPDU;
}

FRemoveEntityPDU UPDUProcessor::ConvertRemoveEntityPDUtoBPStruct(DIS::RemoveEntityPdu& RemovePDUOut)
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