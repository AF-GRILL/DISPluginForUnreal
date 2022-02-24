// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DISEnumsAndStructs.h"
#include "DISRuntimeSettings.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = "DISRuntimeSettings", config = Game, defaultconfig)
class DISRUNTIME_API UDISRuntimeSettings : public UObject
{
public:
	GENERATED_BODY()
		
	UDISRuntimeSettings(const FObjectInitializer& ObjectInitializer);

	static UDISRuntimeSettings* Get();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "DIS Networking",
		Meta = (DisplayName = "Auto Connect Send", Tooltip = "Whether or not the UDP socket for sending DIS packets should be auto connected."))
		bool AutoConnectSend = false;
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "DIS Networking",
		Meta = (DisplayName = "Auto Send IP Address", Tooltip = "The IP address to send UDP packets from if Auto Connect Send is enabled."))
		FString AutoSendIPAddress = "127.0.0.1";
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "DIS Networking",
		Meta = (DisplayName = "Auto Send Port", Tooltip = "The Port to use for sending UDP packets if Auto Connect Send is enabled. Valid Port ranges are from 1024 to 65535.", UIMin = 1024, UIMax = 65535, ClampMin = 1024, ClampMax = 65535))
		int32 AutoSendPort = 3001;
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "DIS Networking",
		Meta = (DisplayName = "Auto Connect Receive", Tooltip = "Whether or not the UDP socket for receiving DIS packets should be auto connected."))
		bool AutoConnectReceive = false;
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "DIS Networking",
		Meta = (DisplayName = "Auto Receive IP Address", Tooltip = "The IP address to receive UDP packets from if Auto Connect Receive is enabled. Set the IP to '0.0.0.0' if all connections are desired."))
		FString AutoReceiveIPAddress = "0.0.0.0";
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "DIS Networking",
		Meta = (DisplayName = "Auto Receive Port", Tooltip = "The Port to receive UDP packets on if Auto Connect Receive is enabled. Valid Port ranges are from 1024 to 65535.", UIMin = 1024, UIMax = 65535, ClampMin = 1024, ClampMax = 65535))
		int32 AutoReceivePort = 3000;

private:
	static class UDISRuntimeSettings* DISSimSettingsSingleton;
};
