// Fill out your copyright notice in the Description page of Project Settings.

#include "PDUProcessor.h"
#include "UDPSubsystem.h"

#include "DISEntity_Base.h"

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
		//For list of enums for PDU type refer to SISO-REF-010-2015, ANNEX A
		switch (static_cast<EPDUType>(pdu->getPduType()))
		{
		case EPDUType::EntityState:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FEntityStatePDU entityStatePDU = ConvertEntityStatePDUtoBPStruct(static_cast<DIS::EntityStatePdu*>(pdu));

			OnEntityStatePDUProcessed.Broadcast(entityStatePDU);

			break;
		}
		case EPDUType::Fire:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FFirePDU firePDU = ConvertFirePDUtoBPStruct(static_cast<DIS::FirePdu*>(pdu));

			OnFirePDUProcessed.Broadcast(firePDU);

			break;
		}
		case EPDUType::Detonation:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FDetonationPDU detPDU = ConvertDetonationPDUtoBPStruct(static_cast<DIS::DetonationPdu*>(pdu));

			OnDetonationPDUProcessed.Broadcast(detPDU);

			break;
		}
		case EPDUType::RemoveEntity:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FRemoveEntityPDU removeEntityPDU = ConvertRemoveEntityPDUtoBPStruct(static_cast<DIS::RemoveEntityPdu*>(pdu));

			OnRemoveEntityPDUProcessed.Broadcast(removeEntityPDU);

			break;
		}
		case EPDUType::Start_Resume:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FStartResumePDU StartResumePDU = ConvertStartResumePDUtoBPStruct(static_cast<DIS::StartResumePdu*>(pdu));

			OnStartResumePDUProcessed.Broadcast(StartResumePDU);

			break;
		}
		case EPDUType::Stop_Freeze:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FStopFreezePDU StopFreezePDU = ConvertStopFreezePDUtoBPStruct(static_cast<DIS::StopFreezePdu*>(pdu));

			OnStopFreezePDUProcessed.Broadcast(StopFreezePDU);

			break;
		}
		case EPDUType::EntityStateUpdate:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FEntityStateUpdatePDU entityStateUpdatePDU = ConvertEntityStateUpdatePDUtoBPStruct(static_cast<DIS::EntityStateUpdatePdu*>(pdu));

			OnEntityStateUpdatePDUProcessed.Broadcast(entityStateUpdatePDU);

			break;
		}
		default:
		{
			break;
		}
		}
	}
}

// TODO: Implement additional PDU conversions to bytes to support sending of more types.
void UPDUProcessor::ConvertEntityStatePDUtoBytes(int Exercise, FEntityStatePDU EntityStatePDUIn, TArray<uint8>& BytesOut)
{
	EntityStatePDUIn.ProtocolVersion = 6;
	EntityStatePDUIn.ExerciseID = Exercise;

	DIS::EntityStatePdu entityStatePDUToSend = EntityStatePDUIn.ToDIS();

	DIS::DataStream buffer(BigEndian);

	//marshal
	entityStatePDUToSend.marshal(buffer);
	TArray<uint8> tempBytes;
	tempBytes.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		tempBytes[i] = buffer[i];
	}
	buffer.clear();

	BytesOut = tempBytes;
}

void UPDUProcessor::ConvertEntityStateUpdatePDUtoBytes(int Exercise, FEntityStateUpdatePDU EntityStateUpdatePDUIn, TArray<uint8>& BytesOut)
{
	EntityStateUpdatePDUIn.ExerciseID = Exercise;

	const DIS::EntityStateUpdatePdu PduToSend = EntityStateUpdatePDUIn.ToDIS();

	DIS::DataStream Buffer(BigEndian);

	PduToSend.marshal(Buffer);
	TArray<uint8> BytesToSend;
	BytesToSend.Init(0, Buffer.size());
	for (int i = 0; i < Buffer.size(); i++)
	{
		BytesToSend[i] = Buffer[i];
	}
	Buffer.clear();

	BytesOut = BytesToSend;
}

void UPDUProcessor::ConvertFirePDUtoBytes(int Exercise, FFirePDU FirePDUIn, TArray<uint8>& BytesOut)
{
	FirePDUIn.ExerciseID = Exercise;

	const DIS::FirePdu PduToSend = FirePDUIn.ToDIS();

	DIS::DataStream Buffer(BigEndian);

	PduToSend.marshal(Buffer);
	TArray<uint8> BytesToSend;
	BytesToSend.Init(0, Buffer.size());
	for (int i = 0; i < Buffer.size(); i++)
	{
		BytesToSend[i] = Buffer[i];
	}
	Buffer.clear();

	BytesOut = BytesToSend;
}

void UPDUProcessor::ConvertRemoveEntityPDUtoBytes(int Exercise, FRemoveEntityPDU RemoveEntityPDUIn, TArray<uint8>& BytesOut)
{
	RemoveEntityPDUIn.ExerciseID = Exercise;

	const DIS::RemoveEntityPdu PduToSend = RemoveEntityPDUIn.ToDIS();

	DIS::DataStream Buffer(BigEndian);

	PduToSend.marshal(Buffer);
	TArray<uint8> BytesToSend;
	BytesToSend.Init(0, Buffer.size());
	for (int i = 0; i < Buffer.size(); i++)
	{
		BytesToSend[i] = Buffer[i];
	}
	Buffer.clear();

	BytesOut = BytesToSend;
}

void UPDUProcessor::ConvertDetonationPDUtoBytes(int Exercise, FDetonationPDU DetonationPDUIn, TArray<uint8>& BytesOut)
{
	DetonationPDUIn.ExerciseID = Exercise;

	const DIS::DetonationPdu PduToSend = DetonationPDUIn.ToDIS();

	DIS::DataStream Buffer(BigEndian);

	PduToSend.marshal(Buffer);
	TArray<uint8> BytesToSend;
	BytesToSend.Init(0, Buffer.size());
	for (int i = 0; i < Buffer.size(); i++)
	{
		BytesToSend[i] = Buffer[i];
	}
	Buffer.clear();

	BytesOut = BytesToSend;
}

void UPDUProcessor::ConvertStartResumePDUtoBytes(int Exercise, FStartResumePDU StartResumePDUIn, TArray<uint8>& BytesOut)
{
	StartResumePDUIn.ExerciseID = Exercise;

	const DIS::StartResumePdu PduToSend = StartResumePDUIn.ToDIS();

	DIS::DataStream Buffer(BigEndian);

	PduToSend.marshal(Buffer);
	TArray<uint8> BytesToSend;
	BytesToSend.Init(0, Buffer.size());
	for (int i = 0; i < Buffer.size(); i++)
	{
		BytesToSend[i] = Buffer[i];
	}
	Buffer.clear();

	BytesOut = BytesToSend;
}

void UPDUProcessor::ConvertStopFreezePDUtoBytes(int Exercise, FStopFreezePDU StopFreezePDUIn, TArray<uint8>& BytesOut)
{
	StopFreezePDUIn.ExerciseID = Exercise;

	const DIS::StopFreezePdu PduToSend = StopFreezePDUIn.ToDIS();

	DIS::DataStream Buffer(BigEndian);

	PduToSend.marshal(Buffer);
	TArray<uint8> BytesToSend;
	BytesToSend.Init(0, Buffer.size());
	for (int i = 0; i < Buffer.size(); i++)
	{
		BytesToSend[i] = Buffer[i];
	}
	Buffer.clear();

	BytesOut = BytesToSend;
}

FEntityStatePDU UPDUProcessor::ConvertEntityStatePDUtoBPStruct(DIS::EntityStatePdu* EntityStatePDUIn)
{
	FEntityStatePDU entityStatePDU;

	//pdu common parameters
	entityStatePDU.ProtocolVersion = EntityStatePDUIn->getProtocolVersion();
	entityStatePDU.ExerciseID = EntityStatePDUIn->getExerciseID();
	entityStatePDU.PduType = static_cast<EPDUType>(EntityStatePDUIn->getPduType());
	entityStatePDU.ProtocolFamily = EntityStatePDUIn->getProtocolFamily();
	entityStatePDU.Timestamp = EntityStatePDUIn->getTimestamp();
	entityStatePDU.Length = EntityStatePDUIn->getLength();
	entityStatePDU.Padding = EntityStatePDUIn->getPadding();

	// Entity State PDU specifics
	//entity id
	entityStatePDU.EntityID = EntityStatePDUIn->getEntityID();

	//pure since unsupported in BP
	DIS::Vector3Double& position = EntityStatePDUIn->getEntityLocation();
	entityStatePDU.EntityLocationDouble[0] = position.getX();
	entityStatePDU.EntityLocationDouble[1] = position.getY();
	entityStatePDU.EntityLocationDouble[2] = position.getZ();

	//location
	entityStatePDU.EntityLocation[0] = position.getX();
	entityStatePDU.EntityLocation[1] = position.getY();
	entityStatePDU.EntityLocation[2] = position.getZ();

	//rotation
	DIS::Orientation& rotation = EntityStatePDUIn->getEntityOrientation();
	entityStatePDU.EntityOrientation.Yaw = rotation.getPsi();
	entityStatePDU.EntityOrientation.Roll = rotation.getPhi();
	entityStatePDU.EntityOrientation.Pitch = rotation.getTheta();

	//velocity (originally in float so this is fine)
	entityStatePDU.EntityLinearVelocity[0] = EntityStatePDUIn->getEntityLinearVelocity().getX();
	entityStatePDU.EntityLinearVelocity[1] = EntityStatePDUIn->getEntityLinearVelocity().getY();
	entityStatePDU.EntityLinearVelocity[2] = EntityStatePDUIn->getEntityLinearVelocity().getZ();

	//Dead reckoning
	entityStatePDU.DeadReckoningParameters.DeadReckoningAlgorithm = EntityStatePDUIn->getDeadReckoningParameters().getDeadReckoningAlgorithm();
	entityStatePDU.DeadReckoningParameters.OtherParameters = TArray<uint8>(reinterpret_cast<uint8*>(EntityStatePDUIn->getDeadReckoningParameters().getOtherParameters()), 15);
	entityStatePDU.DeadReckoningParameters.EntityLinearAcceleration[0] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getX();
	entityStatePDU.DeadReckoningParameters.EntityLinearAcceleration[1] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getY();
	entityStatePDU.DeadReckoningParameters.EntityLinearAcceleration[2] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getZ();
	entityStatePDU.DeadReckoningParameters.EntityAngularVelocity[0] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getX();
	entityStatePDU.DeadReckoningParameters.EntityAngularVelocity[1] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getY();
	entityStatePDU.DeadReckoningParameters.EntityAngularVelocity[2] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getZ();

	//single vars
	entityStatePDU.ForceID = static_cast<EForceID>(EntityStatePDUIn->getForceId());
	entityStatePDU.Marking = FString(EntityStatePDUIn->getMarking().getCharacters());
	entityStatePDU.EntityAppearance = EntityStatePDUIn->getEntityAppearance();
	entityStatePDU.Capabilities = EntityStatePDUIn->getCapabilities();

	//Entity type
	entityStatePDU.EntityType = EntityStatePDUIn->getEntityType();

	//Alternative Entity type
	entityStatePDU.AlternativeEntityType = EntityStatePDUIn->getAlternativeEntityType();

	//Articulation Parameters
	for (int i = 0; i < EntityStatePDUIn->getNumberOfArticulationParameters(); i++) 
	{
		DIS::ArticulationParameter tempArtParam = EntityStatePDUIn->getArticulationParameters()[i];
		FArticulationParameters newArtParam;
		newArtParam.ChangeIndicator = tempArtParam.getChangeIndicator();
		newArtParam.ParameterType = tempArtParam.getParameterType();
		newArtParam.ParameterTypeDesignator = tempArtParam.getParameterTypeDesignator();
		newArtParam.ParameterValue = tempArtParam.getParameterValue();
		newArtParam.PartAttachedTo = tempArtParam.getPartAttachedTo();

		entityStatePDU.ArticulationParameters.Add(newArtParam);
	}

	return entityStatePDU;
}

FEntityStateUpdatePDU UPDUProcessor::ConvertEntityStateUpdatePDUtoBPStruct(DIS::EntityStateUpdatePdu* EntityStateUpdatePDUIn) 
{
	FEntityStateUpdatePDU entityStateUpdatePDU;

	//pdu common parameters
	entityStateUpdatePDU.ProtocolVersion = EntityStateUpdatePDUIn->getProtocolVersion();
	entityStateUpdatePDU.ExerciseID = EntityStateUpdatePDUIn->getExerciseID();
	entityStateUpdatePDU.PduType = static_cast<EPDUType>(EntityStateUpdatePDUIn->getPduType());
	entityStateUpdatePDU.ProtocolFamily = EntityStateUpdatePDUIn->getProtocolFamily();
	entityStateUpdatePDU.Timestamp = EntityStateUpdatePDUIn->getTimestamp();
	entityStateUpdatePDU.Length = EntityStateUpdatePDUIn->getLength();
	entityStateUpdatePDU.Padding = EntityStateUpdatePDUIn->getPadding();

	//Entity State Update specifics
	//entity id
	entityStateUpdatePDU.EntityID = EntityStateUpdatePDUIn->getEntityID();

	//pure since unsupported in BP
	DIS::Vector3Double& position = EntityStateUpdatePDUIn->getEntityLocation();
	entityStateUpdatePDU.EntityLocationDouble[0] = position.getX();
	entityStateUpdatePDU.EntityLocationDouble[1] = position.getY();
	entityStateUpdatePDU.EntityLocationDouble[2] = position.getZ();

	//location
	entityStateUpdatePDU.EntityLocation[0] = position.getX();
	entityStateUpdatePDU.EntityLocation[1] = position.getY();
	entityStateUpdatePDU.EntityLocation[2] = position.getZ();

	//rotation
	DIS::Orientation& rotation = EntityStateUpdatePDUIn->getEntityOrientation();
	entityStateUpdatePDU.EntityOrientation.Yaw = rotation.getPsi();
	entityStateUpdatePDU.EntityOrientation.Roll = rotation.getPhi();
	entityStateUpdatePDU.EntityOrientation.Pitch = rotation.getTheta();

	//velocity (originally in float so this is fine)
	entityStateUpdatePDU.EntityLinearVelocity[0] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getX();
	entityStateUpdatePDU.EntityLinearVelocity[1] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getY();
	entityStateUpdatePDU.EntityLinearVelocity[2] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getZ();

	//Single Vars
	entityStateUpdatePDU.Padding1 = EntityStateUpdatePDUIn->getPadding1();
	entityStateUpdatePDU.EntityAppearance = EntityStateUpdatePDUIn->getEntityAppearance();

	//Articulation Parameters
	for (int i = 0; i < EntityStateUpdatePDUIn->getNumberOfArticulationParameters(); i++)
	{
		DIS::ArticulationParameter tempArtParam = EntityStateUpdatePDUIn->getArticulationParameters()[i];
		FArticulationParameters newArtParam;
		newArtParam.ChangeIndicator = tempArtParam.getChangeIndicator();
		newArtParam.ParameterType = tempArtParam.getParameterType();
		newArtParam.ParameterTypeDesignator = tempArtParam.getParameterTypeDesignator();
		newArtParam.ParameterValue = tempArtParam.getParameterValue();
		newArtParam.PartAttachedTo = tempArtParam.getPartAttachedTo();

		entityStateUpdatePDU.ArticulationParameters.Add(newArtParam);
	}

	return entityStateUpdatePDU;
}

FFirePDU UPDUProcessor::ConvertFirePDUtoBPStruct(DIS::FirePdu* FirePDUIn)
{
	FFirePDU firePDU;

	//pdu common parameters
	firePDU.ProtocolVersion = FirePDUIn->getProtocolVersion();
	firePDU.ExerciseID = FirePDUIn->getExerciseID();
	firePDU.PduType = static_cast<EPDUType>(FirePDUIn->getPduType());
	firePDU.ProtocolFamily = FirePDUIn->getProtocolFamily();
	firePDU.Timestamp = FirePDUIn->getTimestamp();
	firePDU.Length = FirePDUIn->getLength();
	firePDU.Padding = FirePDUIn->getPadding();

	// WarfareFamilyPdu specific parameters
	firePDU.FiringEntityID = FirePDUIn->getFiringEntityID();
	firePDU.TargetEntityID = FirePDUIn->getTargetEntityID();

	// Fire PDU specifics
	//single vars
	firePDU.FireMissionIndex = FirePDUIn->getFireMissionIndex();
	firePDU.Range = FirePDUIn->getRange();

	//MunitionEntityID
	firePDU.MunitionEntityID.Site = FirePDUIn->getMunitionID().getSite();
	firePDU.MunitionEntityID.Application = FirePDUIn->getMunitionID().getApplication();
	firePDU.MunitionEntityID.Entity = FirePDUIn->getMunitionID().getEntity();

	//velocity
	firePDU.Velocity[0] = FirePDUIn->getVelocity().getX();
	firePDU.Velocity[1] = FirePDUIn->getVelocity().getY();
	firePDU.Velocity[2] = FirePDUIn->getVelocity().getZ();

	//location
	firePDU.Location[0] = FirePDUIn->getLocationInWorldCoordinates().getX();
	firePDU.Location[1] = FirePDUIn->getLocationInWorldCoordinates().getY();
	firePDU.Location[2] = FirePDUIn->getLocationInWorldCoordinates().getZ();

	//locationDouble
	firePDU.LocationDouble[0] = FirePDUIn->getLocationInWorldCoordinates().getX();
	firePDU.LocationDouble[1] = FirePDUIn->getLocationInWorldCoordinates().getY();
	firePDU.LocationDouble[2] = FirePDUIn->getLocationInWorldCoordinates().getZ();

	//event id
	firePDU.EventID = FirePDUIn->getEventID();

	//burst descriptor
	firePDU.BurstDescriptor.Warhead = FirePDUIn->getBurstDescriptor().getWarhead();
	firePDU.BurstDescriptor.Fuse = FirePDUIn->getBurstDescriptor().getFuse();
	firePDU.BurstDescriptor.Rate = FirePDUIn->getBurstDescriptor().getRate();
	firePDU.BurstDescriptor.Quantity = FirePDUIn->getBurstDescriptor().getQuantity();
	firePDU.BurstDescriptor.EntityType = FirePDUIn->getBurstDescriptor().getMunition();

	return firePDU;
}

FDetonationPDU UPDUProcessor::ConvertDetonationPDUtoBPStruct(DIS::DetonationPdu* DetPDUIn)
{
	FDetonationPDU detonationPDU;

	//pdu common parameters
	detonationPDU.ProtocolVersion = DetPDUIn->getProtocolVersion();
	detonationPDU.ExerciseID = DetPDUIn->getExerciseID();
	detonationPDU.PduType = static_cast<EPDUType>(DetPDUIn->getPduType());
	detonationPDU.ProtocolFamily = DetPDUIn->getProtocolFamily();
	detonationPDU.Timestamp = DetPDUIn->getTimestamp();
	detonationPDU.Length = DetPDUIn->getLength();
	detonationPDU.Padding = DetPDUIn->getPadding();

	// WarfareFamilyPdu specific parameters
	detonationPDU.FiringEntityID = DetPDUIn->getFiringEntityID();
	detonationPDU.TargetEntityID = DetPDUIn->getTargetEntityID();

	//Detonation PDU specifics
	//MunitionEntityID
	detonationPDU.MunitionEntityID = DetPDUIn->getMunitionID();

	//event id
	detonationPDU.EventID = DetPDUIn->getEventID();

	//velocity
	detonationPDU.Velocity[0] = DetPDUIn->getVelocity().getX();
	detonationPDU.Velocity[1] = DetPDUIn->getVelocity().getY();
	detonationPDU.Velocity[2] = DetPDUIn->getVelocity().getZ();

	//location
	detonationPDU.Location[0] = DetPDUIn->getLocationInWorldCoordinates().getX();
	detonationPDU.Location[1] = DetPDUIn->getLocationInWorldCoordinates().getY();
	detonationPDU.Location[2] = DetPDUIn->getLocationInWorldCoordinates().getZ();

	//locationDouble
	detonationPDU.LocationDouble[0] = DetPDUIn->getLocationInWorldCoordinates().getX();
	detonationPDU.LocationDouble[1] = DetPDUIn->getLocationInWorldCoordinates().getY();
	detonationPDU.LocationDouble[2] = DetPDUIn->getLocationInWorldCoordinates().getZ();

	//location
	detonationPDU.LocationInEntityCoords[0] = DetPDUIn->getLocationInEntityCoordinates().getX();
	detonationPDU.LocationInEntityCoords[1] = DetPDUIn->getLocationInEntityCoordinates().getY();
	detonationPDU.LocationInEntityCoords[2] = DetPDUIn->getLocationInEntityCoordinates().getZ();

	//burst descriptor
	detonationPDU.BurstDescriptor.Warhead = DetPDUIn->getBurstDescriptor().getWarhead();
	detonationPDU.BurstDescriptor.Fuse = DetPDUIn->getBurstDescriptor().getFuse();
	detonationPDU.BurstDescriptor.Rate = DetPDUIn->getBurstDescriptor().getRate();
	detonationPDU.BurstDescriptor.Quantity = DetPDUIn->getBurstDescriptor().getQuantity();
	detonationPDU.BurstDescriptor.EntityType = DetPDUIn->getBurstDescriptor().getMunition();

	detonationPDU.TargetEntityID = DetPDUIn->getTargetEntityID();

	detonationPDU.FiringEntityID.Site = DetPDUIn->getFiringEntityID().getSite();
	detonationPDU.FiringEntityID.Application = DetPDUIn->getFiringEntityID().getApplication();
	detonationPDU.FiringEntityID.Entity = DetPDUIn->getFiringEntityID().getEntity();

	//single vars
	detonationPDU.PduType = static_cast<EPDUType>(DetPDUIn->getPduType());
	detonationPDU.DetonationResult = DetPDUIn->getDetonationResult();
	detonationPDU.Pad = DetPDUIn->getPad();

	//Articulation Parameters
	for (int i = 0; i < DetPDUIn->getNumberOfArticulationParameters(); i++)
	{
		DIS::ArticulationParameter tempArtParam = DetPDUIn->getArticulationParameters()[i];
		FArticulationParameters newArtParam;
		newArtParam.ChangeIndicator = tempArtParam.getChangeIndicator();
		newArtParam.ParameterType = tempArtParam.getParameterType();
		newArtParam.ParameterTypeDesignator = tempArtParam.getParameterTypeDesignator();
		newArtParam.ParameterValue = tempArtParam.getParameterValue();
		newArtParam.PartAttachedTo = tempArtParam.getPartAttachedTo();

		detonationPDU.ArticulationParameters.Add(newArtParam);
	}

	return detonationPDU;
}

FRemoveEntityPDU UPDUProcessor::ConvertRemoveEntityPDUtoBPStruct(DIS::RemoveEntityPdu* RemovePDUIn)
{
	FRemoveEntityPDU removeEntityPDU;

	//pdu common parameters
	removeEntityPDU.ProtocolVersion = RemovePDUIn->getProtocolVersion();
	removeEntityPDU.ExerciseID = RemovePDUIn->getExerciseID();
	removeEntityPDU.PduType = static_cast<EPDUType>(RemovePDUIn->getPduType());
	removeEntityPDU.ProtocolFamily = RemovePDUIn->getProtocolFamily();
	removeEntityPDU.Timestamp = RemovePDUIn->getTimestamp();
	removeEntityPDU.Length = RemovePDUIn->getLength();
	removeEntityPDU.Padding = RemovePDUIn->getPadding();

	//Simulation Management Family Pdu specific
	removeEntityPDU.OriginatingEntityID = RemovePDUIn->getOriginatingEntityID();
	removeEntityPDU.ReceivingEntityID = RemovePDUIn->getReceivingEntityID();

	removeEntityPDU.RequestID = RemovePDUIn->getRequestID();

	return removeEntityPDU;
}

FStartResumePDU UPDUProcessor::ConvertStartResumePDUtoBPStruct(DIS::StartResumePdu* StartResumePDUIn) 
{
	FStartResumePDU startResumePDU;

	//pdu common parameters
	startResumePDU.ProtocolVersion = StartResumePDUIn->getProtocolVersion();
	startResumePDU.ExerciseID = StartResumePDUIn->getExerciseID();
	startResumePDU.PduType = static_cast<EPDUType>(StartResumePDUIn->getPduType());
	startResumePDU.ProtocolFamily = StartResumePDUIn->getProtocolFamily();
	startResumePDU.Timestamp = StartResumePDUIn->getTimestamp();
	startResumePDU.Length = StartResumePDUIn->getLength();
	startResumePDU.Padding = StartResumePDUIn->getPadding();

	//Simulation Management Family Pdu specific
	startResumePDU.OriginatingEntityID = StartResumePDUIn->getOriginatingEntityID();
	startResumePDU.ReceivingEntityID = StartResumePDUIn->getReceivingEntityID();

	// Start/Resume PDU specific
	DIS::ClockTime RealWorldTime = StartResumePDUIn->getRealWorldTime();
	DIS::ClockTime SimulationTime = StartResumePDUIn->getRealWorldTime();

	startResumePDU.RealWorldTime.Hour = RealWorldTime.getHour();
	startResumePDU.RealWorldTime.TimePastHour = RealWorldTime.getTimePastHour();

	startResumePDU.SimulationTime.Hour = SimulationTime.getHour();
	startResumePDU.SimulationTime.TimePastHour = SimulationTime.getTimePastHour();

	startResumePDU.RequestID = StartResumePDUIn->getRequestID();

	return startResumePDU;
}

FStopFreezePDU UPDUProcessor::ConvertStopFreezePDUtoBPStruct(DIS::StopFreezePdu* StopFreezePDUIn)
{
	FStopFreezePDU stopFreezePDU;

	//pdu common parameters
	stopFreezePDU.ProtocolVersion = StopFreezePDUIn->getProtocolVersion();
	stopFreezePDU.ExerciseID = StopFreezePDUIn->getExerciseID();
	stopFreezePDU.PduType = static_cast<EPDUType>(StopFreezePDUIn->getPduType());
	stopFreezePDU.ProtocolFamily = StopFreezePDUIn->getProtocolFamily();
	stopFreezePDU.Timestamp = StopFreezePDUIn->getTimestamp();
	stopFreezePDU.Length = StopFreezePDUIn->getLength();
	stopFreezePDU.Padding = StopFreezePDUIn->getPadding();

	//Simulation Management Family Pdu specific
	stopFreezePDU.OriginatingEntityID = StopFreezePDUIn->getOriginatingEntityID();
	stopFreezePDU.ReceivingEntityID = StopFreezePDUIn->getReceivingEntityID();

	// Stop/Freeze PDU specifics
	DIS::ClockTime RealWorldTime = StopFreezePDUIn->getRealWorldTime();

	stopFreezePDU.RealWorldTime.Hour = RealWorldTime.getHour();
	stopFreezePDU.RealWorldTime.TimePastHour = RealWorldTime.getTimePastHour();

	stopFreezePDU.Reason = static_cast<EReason>(StopFreezePDUIn->getReason());
	stopFreezePDU.FrozenBehavior = StopFreezePDUIn->getFrozenBehavior();
	stopFreezePDU.PaddingOne = StopFreezePDUIn->getPadding1();
	stopFreezePDU.RequestID = StopFreezePDUIn->getRequestID();

	return stopFreezePDU;
}