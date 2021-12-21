// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sockets/Public/IPAddress.h"
#include "Common/UdpSocketBuilder.h"
#include "Common/UdpSocketReceiver.h"
#include "Common/UdpSocketSender.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UDPSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUDPSubsystem, Log, All);

USTRUCT()
struct FUDPSettings
{
	GENERATED_BODY()

	/** Default sending socket IP string in form e.g. 127.0.0.1. */
	FString SendIP;

	/** Default connection port e.g. 3001*/
	int32 SendPort;

	/** Default receiving socket IP string in form e.g. 0.0.0.0 for all connections, may need 127.0.0.1 for some cases. */
	FString ReceiveIP;

	/** Default connection port e.g. 3002*/
	int32 ReceivePort;

	FString SendSocketName;
	FString ReceiveSocketName;

	/** in bytes */
	int32 BufferSize;

	/** If true will auto-connect on begin play to IP/port specified for sending udp messages, plus when emit is called */
	bool bShouldAutoOpenSend;

	/** If true will auto-listen on begin play to port specified for receiving udp messages. */
	bool bShouldAutoOpenReceive;

	/** Whether we should process our data on the gamethread or the udp thread. */
	bool bReceiveDataOnGameThread;

	bool bIsReceiveOpen;
	bool bIsSendOpen;

	FUDPSettings()
	{
		bShouldAutoOpenSend = false;
		bShouldAutoOpenReceive = false;
		bReceiveDataOnGameThread = true;
		SendIP = FString(TEXT("127.0.0.1"));
		SendPort = 3001;
		ReceiveIP = FString(TEXT("0.0.0.0"));
		ReceivePort = 3000;
		SendSocketName = FString(TEXT("UE4-OpenDIS-Send"));
		ReceiveSocketName = FString(TEXT("UE4-OpenDIS-Receive"));

		bIsReceiveOpen = false;
		bIsSendOpen = false;

		BufferSize = 2 * 1024 * 1024;	//default roughly 2mb
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDPSocketStateSignature, int32, Port);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDPSocketSendStateSignature, int32, BoundPort);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUDPMessageSignature, const TArray<uint8>&, Bytes, const FString&, IPAddress);

UCLASS(ClassGroup = "Networking", meta = (BlueprintSpawnableComponent))
class UEOPENDIS_API UUDPSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	/** On message received on receive socket from Ip address */
	UPROPERTY(BlueprintAssignable, Category = "UDP Events")
		FUDPMessageSignature OnReceivedBytes;

	/** Callback when we start listening on the udp receive socket*/
	UPROPERTY(BlueprintAssignable, Category = "UDP Events")
		FUDPSocketStateSignature OnReceiveSocketOpened;

	/** Called after receiving socket has been closed. */
	UPROPERTY(BlueprintAssignable, Category = "UDP Events")
		FUDPSocketStateSignature OnReceiveSocketClosed;

	/** Called when the send socket is ready to use; optionally open your receive socket to bound send port here */
	UPROPERTY(BlueprintAssignable, Category = "UDP Events")
		FUDPSocketSendStateSignature OnSendSocketOpened;

	/** Called when the send socket has been closed */
	UPROPERTY(BlueprintAssignable, Category = "UDP Events")
		FUDPSocketStateSignature OnSendSocketClosed;

	/** Defining UDP sending and receiving Ips, ports, and other defaults*/
	FUDPSettings Settings;

	UFUNCTION(BlueprintCallable, Category = "OpenDIS|UDP")
		bool OpenSendSocket(const FString& InIP = TEXT("127.0.0.1"), const int32 InPort = 3000);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|UDP")
		bool CloseSendSocket();
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|UDP")
		bool OpenReceiveSocket(const FString& InListenIP = TEXT("0.0.0.0"), const int32 InListenPort = 3001);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|UDP")
		bool CloseReceiveSocket();
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|UDP")
		bool EmitBytes(const TArray<uint8>& Bytes);

	UFUNCTION(BlueprintPure, Category = "OpenDIS|UDP")
		void GetUDPReceiveSettings(bool& CurrentlyConnected, FString& ReceiveIPAddress, int32& ReceivePort, bool& AutoConnectReceive);
	UFUNCTION(BlueprintPure, Category = "OpenDIS|UDP")
		void GetUDPSendSettings(bool& CurrentlyConnected, FString& SendIPAddress, int32& SendPort, bool& AutoConnectSend);

protected:
	FSocket* SenderSocket;
	FSocket* ReceiverSocket;
	FUdpSocketReceiver* UDPReceiver;
	FString SocketDescription;
	TSharedPtr<FInternetAddr> RemoteAdress;
	ISocketSubsystem* SocketSubsystem;
};