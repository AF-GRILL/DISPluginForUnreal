// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>

#include "CoreMinimal.h"
#include "DISEnumsAndStructs.h"
#include "PDUMasterInclude.h"
#include "DISClassEnumMappings.h"
#include "UDPSubsystem.h"
#include "GameFramework/Info.h"
#include "DISGameManager.generated.h"

//Forward declarations
class UDISComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogDISGameManager, Log, All);

DECLARE_STATS_GROUP(TEXT("DISGameManager_Game"), STATGROUP_DISGameManager, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("GetAssociatedDISComponent"), STAT_GetAssociatedDISComponent, STATGROUP_DISGameManager);

USTRUCT(Blueprintable)
struct FSendSocketInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere,
		Meta = (Tooltip = "The IP Address to either send UDP packets to or receive UDP packets from."))
		FString IpAddress = "0.0.0.0";

	UPROPERTY(BlueprintReadWrite, EditAnywhere,	
		Meta = (Tooltip = "The Port to either send UDP packets to or receive UDP packets from. Valid Port ranges are from 1024 to 65535.", UIMin = 1024, UIMax = 65535, ClampMin = 1024, ClampMax = 65535))
		int32 Port = 3000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FSendSocketSettings SocketSettings;
};

USTRUCT(Blueprintable)
struct FReceiveSocketInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere,
		Meta = (Tooltip = "The IP Address to either send UDP packets to or receive UDP packets from."))
		FString IpAddress = "0.0.0.0";

	UPROPERTY(BlueprintReadWrite, EditAnywhere,
		Meta = (Tooltip = "The Port to either send UDP packets to or receive UDP packets from. Valid Port ranges are from 1024 to 65535.", UIMin = 1024, UIMax = 65535, ClampMin = 1024, ClampMax = 65535))
		int32 Port = 3000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FReceiveSocketSettings SocketSettings;
};

UCLASS(Blueprintable)
class DISRUNTIME_API ADISGameManager : public AInfo
{
	GENERATED_BODY()


public:
	ADISGameManager();

	/**
	* Gets a reference to the DIS Game Manager actor in the current open level.
	* Returns the reference to the Game Manager if one is found. Returns null if nothing is found or if multiple exist in the level.
	* @param WorldContextObject Object to get a world object pointer from.
	*/
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Game Manager", meta = (WorldContext = "WorldContextObject"))
		static ADISGameManager* GetDISGameManager(UObject* WorldContextObject);

	/**
	 * Delegates the given Entity State PDU to the appropriate DIS Entity actor.
	 * @param EntityStatePDUIn - The Entity State PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|Game Manager")
		void HandleEntityStatePDU(FEntityStatePDU EntityStatePDUIn);
	/**
	 * Delegates the given Entity State Update PDU to the appropriate DIS Entity actor.
	 * @param EntityStateUpdatePDUIn - The Entity State Update PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|Game Manager")
		void HandleEntityStateUpdatePDU(FEntityStateUpdatePDU EntityStateUpdatePDUIn);
	/**
	 * Delegates the given Fire PDU to the appropriate DIS Entity actor.
	 * @param FirePDUIn - The Fire PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|Game Manager")
		void HandleFirePDU(FFirePDU FirePDUIn);
	/**
	 * Delegates the given Detonation PDU to the appropriate DIS Entity actor.
	 * @param DetonationPDUIn - The Detonation PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|Game Manager")
		void HandleDetonationPDU(FDetonationPDU DetonationPDUIn);
	/**
	 * Delegates the given Remove Entity PDU to the appropriate DIS Entity actor.
	 * @param RemoveEntityPDUIn - The Remove Entity PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|Game Manager")
		void HandleRemoveEntityPDU(FRemoveEntityPDU RemoveEntityPDUIn);

	/**
	 * Adds a new entry to the DIS Entity map.
	 * @param EntityIDToAdd - The Entity ID to key the given entity under in the map.
	 * @param EntityToAdd - The entity to add to the entity map.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|Game Manager")
		void AddDISEntityToMap(FEntityID EntityIDToAdd, AActor* EntityToAdd);
	/**
	 * Removes the entry correlating to the given Entity ID from the DIS Entity map.
	 * Returns whether or not an entry was removed.
	 * @param EntityIDToRemove - The Entity ID of the DIS entity that needs removed from the entity map.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|Game Manager")
		bool RemoveDISEntityFromMap(FEntityID EntityIDToRemove);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GRILL DIS|Game Manager",
		Meta = (DisplayName = "DIS Enumeration Mapping", Tooltip = "The DIS Enumeration Mapping to use for this manager. This dictates the entity enumerations that will be recognized and managed by this DIS Game Manager."))
		UDISClassEnumMappings* DISClassEnum;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void HandleOnDISEntityDestroyed(AActor* DestroyedActor);
	
	/**
	 * The mapping between DIS Enumerations and corresponding entity classes.
	 */
	UPROPERTY(BlueprintReadOnly)
		TMap<FEntityType, TSoftClassPtr<AActor>> DISClassMappings;
	std::map<FEntityType, TSoftClassPtr<AActor>> RawDISClassMappings;
	/**
	 * The mapping between DIS Entity IDs and corresponding entity actors.
	 */
	UPROPERTY(BlueprintReadOnly)
		TMap<FEntityID, AActor*> DISActorMappings;
	std::map<FEntityID, AActor*> RawDISActorMappings;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GRILL DIS|Game Manager",
		Meta = (DisplayName = "Exercise ID", Tooltip = "The Exercise ID of the DIS sim. Valid Exercise IDs range from 0 to 255.", UIMin = 0, UIMax = 255, ClampMin = 0, ClampMax = 255))
		int32 ExerciseID = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GRILL DIS|Game Manager",
		Meta = (DisplayName = "Site ID", Tooltip = "The Site ID of this application instance. Valid Site IDs range from 0 to 65535.", UIMin = 0, UIMax = 65535, ClampMin = 0, ClampMax = 65535))
		int32 SiteID = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GRILL DIS|Game Manager",
		Meta = (DisplayName = "Application ID", Tooltip = "The Application ID of this application instance. Valid Application IDs range from 0 to 65535.", UIMin = 0, UIMax = 65535, ClampMin = 0, ClampMax = 65535))
		int32 ApplicationID = 0;

	//Whether or not to auto connect receive sockets
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GRILL DIS|Game Manager|Networking")
		bool AutoConnectReceiveAddresses;
	//Receive sockets to auto setup
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GRILL DIS|Game Manager|Networking",
		meta = (DisplayName = "Auto Connect Receive Sockets", EditCondition = "AutoConnectReceiveAddresses"))
		TArray<FReceiveSocketInfo> ReceiveSocketsToSetup;
	//Whether or not to auto connect send sockets
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GRILL DIS|Game Manager|Networking")
		bool AutoConnectSendAddresses;
	//Send sockets to auto setup
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GRILL DIS|Game Manager|Networking",
		meta = (DisplayName = "Auto Connect Send Sockets", EditCondition = "AutoConnectSendAddresses"))
		TArray<FSendSocketInfo> SendSocketsToSetup;


private:
	void SpawnNewEntityFromEntityState(FEntityStatePDU EntityStatePDUIn);
	UDISComponent* GetAssociatedDISComponent(FEntityID EntityIDIn);
};