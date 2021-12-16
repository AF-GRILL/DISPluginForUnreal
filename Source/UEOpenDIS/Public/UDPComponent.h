// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sockets/Public/IPAddress.h"
#include "Common/UdpSocketBuilder.h"
#include "Common/UdpSocketReceiver.h"
#include "Common/UdpSocketSender.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UDPComponent.generated.h"

USTRUCT(BlueprintType)
struct FUDPSettings
{
	GENERATED_BODY()

	/** Default sending socket IP string in form e.g. 127.0.0.1. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
		FString SendIP;

	/** Default connection port e.g. 3001*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
		int32 SendPort;

	/** Default receiving socket IP string in form e.g. 0.0.0.0 for all connections, may need 127.0.0.1 for some cases. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
		FString ReceiveIP;

	/** Default connection port e.g. 3002*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
		int32 ReceivePort;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
		FString SendSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
		FString ReceiveSocketName;

	/** in bytes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
		int32 BufferSize;

	/** If true will auto-connect on begin play to IP/port specified for sending udp messages, plus when emit is called */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
		bool bShouldAutoOpenSend;

	/** If true will auto-listen on begin play to port specified for receiving udp messages. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
		bool bShouldAutoOpenReceive;

	/** Whether we should process our data on the gamethread or the udp thread. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP Connection Properties")
		bool bReceiveDataOnGameThread;

	UPROPERTY(BlueprintReadOnly, Category = "UDP Connection Properties")
		bool bIsReceiveOpen;
	UPROPERTY(BlueprintReadOnly, Category = "UDP Connection Properties")
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

/**
 *
 */
class UEOPENDIS_API FUDPWrapper
{
public:
	TFunction<void(const TArray<uint8>&, const FString&)> OnReceivedBytes;
	TFunction<void(int32 Port)> OnReceiveOpened;
	TFunction<void(int32 Port)> OnReceiveClosed;
	TFunction<void(int32 BoundPort)> OnSendOpened;
	TFunction<void(int32 Port)> OnSendClosed;

	//Default settings, on open send/receive they will sync with what was last passed into them
	FUDPSettings CurrentUDPSettings;

	FUDPWrapper();
	~FUDPWrapper();

	/**
	* Open socket for sending, returns true if successful
	*/
	bool OpenSendSocket(const FString& InIP = TEXT("127.0.0.1"), const int32 InPort = 3001);

	/**
	* Close current sending socket, returns true if successful
	*/
	bool CloseSendSocket();

	/**
	* Emit given bytes to send socket. If Settings.bShouldAutoOpenSend is true it will auto-open socket.
	* Returns true if bytes emitted successfully
	*/
	bool EmitBytes(const TArray<uint8>& Bytes);

	//Receive
	/**
	* Open current receiving socket, returns true if successful
	*/
	bool OpenReceiveSocket(const FString& InIP = TEXT("0.0.0.0"), const int32 InListenPort = 3000);
	/**
	* Close current receiving socket, returns true if successful
	*/
	bool CloseReceiveSocket();

	//Callback convenience
	void ClearSendCallbacks();
	void ClearReceiveCallbacks();

protected:
	FSocket* SenderSocket;
	FSocket* ReceiverSocket;
	FUdpSocketReceiver* UDPReceiver;
	FString SocketDescription;
	TSharedPtr<FInternetAddr> RemoteAdress;
	ISocketSubsystem* SocketSubsystem;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDPSocketStateSignature, int32, Port);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDPSocketSendStateSignature, int32, BoundPort);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUDPMessageSignature, const TArray<uint8>&, Bytes, const FString&, IPAddress);

UCLASS(ClassGroup = "Networking")
class UEOPENDIS_API UUDPComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUDPComponent();

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

	bool OpenSendSocket(const FString& InIP = TEXT("127.0.0.1"), const int32 InPort = 3000);
	bool CloseSendSocket();
	bool OpenReceiveSocket(const FString& InListenIP = TEXT("0.0.0.0"), const int32 InListenPort = 3002);
	bool CloseReceiveSocket();
	bool EmitBytes(const TArray<uint8>& Bytes);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	TSharedPtr<FUDPWrapper> UDPWrapper;
	void LinkupCallbacks();
};