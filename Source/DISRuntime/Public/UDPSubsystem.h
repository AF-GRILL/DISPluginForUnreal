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
		SendSocketName = FString(TEXT("UE4-DIS-Send"));
		ReceiveSocketName = FString(TEXT("UE4-DIS-Receive"));

		bIsReceiveOpen = false;
		bIsSendOpen = false;

		BufferSize = 2 * 1024 * 1024;	//default roughly 2mb
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDPSocketStateSignature, int32, Port);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUDPMessageSignature, const TArray<uint8>&, Bytes, const FString&, IPAddress);

UCLASS(ClassGroup = "Networking", meta = (BlueprintSpawnableComponent))
class DISRUNTIME_API UUDPSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	/** Called after bytes are received by a bound UDP socket.
	Passes the received message in bytes and the IP address that received the message as parameters. */
	UPROPERTY(BlueprintAssignable, Category = "UDP Subsystem|Events")
		FUDPMessageSignature OnReceivedBytes;

	/** Called after a new receive UDP socket is opened.
	Passes the bound port as a parameter. */
	UPROPERTY(BlueprintAssignable, Category = "UDP Subsystem|Events")
		FUDPSocketStateSignature OnReceiveSocketOpened;

	/** Called after a receive UDP socket has been closed.
	Passes the bound port as a parameter. */
	UPROPERTY(BlueprintAssignable, Category = "UDP Subsystem|Events")
		FUDPSocketStateSignature OnReceiveSocketClosed;

	/** Called after a sned UDP socket has been opened.
	Passes the bound port as a parameter. */
	UPROPERTY(BlueprintAssignable, Category = "UDP Subsystem|Events")
		FUDPSocketStateSignature OnSendSocketOpened;

	/** Called after a send UDP socket has been closed.
	Passes the bound port as a parameter. */
	UPROPERTY(BlueprintAssignable, Category = "UDP Subsystem|Events")
		FUDPSocketStateSignature OnSendSocketClosed;

	/** Defining UDP sending and receiving Ips, ports, and other defaults */
	FUDPSettings Settings;

	/**
	 * Opens a new UDP send socket at the given IP address and port. Closes an opened connection if there was one prior to creating a new one.
	 * Returns whether or not the opening of the socket was successful.
	 * @param InIP - The IP address to send UDP packets to.
	 * @param InPort - The port to send UDP packets on.
	 */
	UFUNCTION(BlueprintCallable, Category = "UDP Subsystem")
		bool OpenSendSocket(const FString& InIP = TEXT("127.0.0.1"), const int32 InPort = 3000);
	/**
	 * Closes the opened send socket.
	 * Returns whether or not the send socket was closed successfully. If none are opened, returns true.
	 */
	UFUNCTION(BlueprintCallable, Category = "UDP Subsystem")
		bool CloseSendSocket();
	/**
	 * Opens a new UDP receive socket at the given IP address and port. Closes an opened connection if there was one prior to creating a new one.
	 * Returns whether or not the opening of the socket was successful.
	 * @param InIP - The IP address to receive UDP packets from. Pass in '0.0.0.0' if all connections desired.
	 * @param InPort - The port to receive UDP packets on.
	 */
	UFUNCTION(BlueprintCallable, Category = "UDP Subsystem")
		bool OpenReceiveSocket(const FString& InListenIP = TEXT("0.0.0.0"), const int32 InListenPort = 3001);
	/**
	 * Closes the opened receive socket.
	 * Returns whether or not the receive socket was closed successfully. If none are opened, returns true.
	 */
	UFUNCTION(BlueprintCallable, Category = "UDP Subsystem")
		bool CloseReceiveSocket();
	/**
	 * Sends bytes over the opened send socket.
	 * Returns whether or not the sending was successful.
	 * @param Bytes - The bytes to send over UDP.
	 */
	UFUNCTION(BlueprintCallable, Category = "UDP Subsystem")
		bool EmitBytes(const TArray<uint8>& Bytes);

	/**
	 * Gets all information relating to the current receive settings.
	 */
	UFUNCTION(BlueprintPure, Category = "UDP Subsystem")
		void GetUDPReceiveSettings(bool& CurrentlyConnected, FString& ReceiveIPAddress, int32& ReceivePort, bool& AutoConnectReceive);
	/**
	 * Gets all information relating to the current send settings.
	 */
	UFUNCTION(BlueprintPure, Category = "UDP Subsystem")
		void GetUDPSendSettings(bool& CurrentlyConnected, FString& SendIPAddress, int32& SendPort, bool& AutoConnectSend);

protected:
	FSocket* SenderSocket;
	FSocket* ReceiverSocket;
	FUdpSocketReceiver* UDPReceiver;
	FString SocketDescription;
	TSharedPtr<FInternetAddr> RemoteAdress;
	ISocketSubsystem* SocketSubsystem;
};