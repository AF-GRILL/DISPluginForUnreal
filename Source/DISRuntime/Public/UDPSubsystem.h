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

UENUM(Blueprintable)
enum class EConnectionType : uint8
{
	Broadcast,
	Multicast,
	Unicast
};

USTRUCT(Blueprintable)
struct FSendSocketSettings
{
	GENERATED_BODY()

	/** Connection type to use for this send socket. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EConnectionType SendSocketConnectionType;

	/** Friendly description of what this socket is to be used for. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SocketDescription;

	/** Byte size the buffer of the socket should have. Defaults to roughly 2MB. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 BufferSize;

	FSendSocketSettings()
	{
		SendSocketConnectionType = EConnectionType::Broadcast;

		SocketDescription = FString(TEXT("UE4-DIS-Send-Socket"));

		BufferSize = 2 * 1024 * 1024;	//default roughly 2mb
	}
};

USTRUCT(Blueprintable)
struct FReceiveSocketSettings
{
	GENERATED_BODY()

	/** Friendly description of what this socket is to be used for. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SocketDescription;

	/** Byte size the buffer of the socket should have. Defaults to roughly 2MB. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 BufferSize;

	/** Whether or not multicast should be used with this receive socket. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bUseMulticast;

	/** Set to true to process packets sent by the local machine. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAllowLoopback;

	/** Whether we should process our data on the gamethread or the udp thread. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bReceiveDataOnGameThread;

	FReceiveSocketSettings()
	{
		SocketDescription = FString(TEXT("UE4-DIS-Receive-Socket"));

		BufferSize = 2 * 1024 * 1024;	//default roughly 2mb

		bUseMulticast = false;
		bAllowLoopback = false;
		bReceiveDataOnGameThread = true;
	}
};

USTRUCT()
struct FReceiveSocketMapValue
{
	GENERATED_BODY()

	FSocket* ReceiveSocket;

	FUdpSocketReceiver* UDPReceiver;

	FReceiveSocketMapValue()
	{
		ReceiveSocket = nullptr;
		UDPReceiver = nullptr;
	}

	FReceiveSocketMapValue(FSocket* NewReceiveSocket, FUdpSocketReceiver* NewUdpReceiver)
	{
		ReceiveSocket = NewReceiveSocket;
		UDPReceiver = NewUdpReceiver;
	}

	bool CloseReceiveSocket()
	{
		bool bDidCloseCorrectly = false;

		UDPReceiver->Stop();
		delete UDPReceiver;
		UDPReceiver = nullptr;

		bDidCloseCorrectly = ReceiveSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ReceiveSocket);
		ReceiveSocket = nullptr;

		return bDidCloseCorrectly;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FUDPReceiveSocketStateSignature, int32, ReceiveSocketID, FString, IpListeningOn, int32, PortListeningOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FUDPSendSocketStateSignature, int32, SendSocketID, FString, LocalIp, int32, LocalPort, FString, PeerIp, int32, PeerPort);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUDPMessageSignature, const TArray<uint8>&, Bytes, const FString&, IPAddress);

DECLARE_STATS_GROUP(TEXT("UDPSubsystem_Game"), STATGROUP_UDPSubsystem, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("ReceiveBytes"), STAT_ReceiveBytes, STATGROUP_UDPSubsystem);
DECLARE_CYCLE_STAT(TEXT("SendBytes"), STAT_SendBytes, STATGROUP_UDPSubsystem);

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
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|UDP Subsystem|Events")
		FUDPMessageSignature OnReceivedBytes;

	/** Called after a new receive UDP socket is opened.
	Passes the bound IP and port as a parameter. */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|UDP Subsystem|Events")
		FUDPReceiveSocketStateSignature OnReceiveSocketOpened;

	/** Called after a receive UDP socket has been closed.
	Passes the bound IP and port as a parameter. */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|UDP Subsystem|Events")
		FUDPReceiveSocketStateSignature OnReceiveSocketClosed;

	/** Called after a sned UDP socket has been opened.
	Passes the bound IP and port as a parameter. */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|UDP Subsystem|Events")
		FUDPSendSocketStateSignature OnSendSocketOpened;

	/** Called after a send UDP socket has been closed.
	Passes the bound IP and port as a parameter. */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|UDP Subsystem|Events")
		FUDPSendSocketStateSignature OnSendSocketClosed;

	/**
	 * Opens a new UDP receive socket at the given IP address and port. Closes an opened connection if there was one prior to creating a new one.
	 * Returns whether or not the opening of the socket was successful and the ID of the receive socket.
	 * @param SocketSettings - The settings this seocket should utilize.
	 * @param ReceiveSocketID - The ID of the opened receive socket.
	 * @param IpToListenOn - The IP address to receive UDP packets from. Pass in '0.0.0.0' if all connections desired.
	 * @param PortToListenOn - The port to receive UDP packets on.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|UDP Subsystem")
		bool OpenReceiveSocket(FReceiveSocketSettings SocketSettings, int32& ReceiveSocketID, const FString& IpToListenOn = TEXT("0.0.0.0"), const int32 PortToListenOn = 3001);
	/**
	 * Closes the opened receive socket.
	 * Returns whether or not the receive socket was closed successfully. If none are opened, returns true.
	 * @param ReceiveSocketIdToClose - The ID of the receive socket to close.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|UDP Subsystem")
		bool CloseReceiveSocket(int32 ReceiveSocketIdToClose);
	/**
	 * Opens a new UDP send socket at the given IP address and port. Closes an opened connection if there was one prior to creating a new one.
	 * Returns whether or not the opening of the socket was successful and the ID of the send socket.
	 * @param SocketSettings - The settings this seocket should utilize.
	 * @param SendSocketID - The ID of the opened send socket.
	 * @param IpToSendOn - The IP address to send UDP packets to.
	 * @param PortToSendOn - The port to send UDP packets on.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|UDP Subsystem")
		bool OpenSendSocket(FSendSocketSettings SocketSettings, int32& SendSocketID, const FString& IpToSendOn = TEXT("127.0.0.1"), const int32 PortToSendOn = 3000);
	/**
	 * Closes the opened send socket.
	 * Returns whether or not the send socket was closed successfully. If none are opened, returns true.
	 * @param SendSocketIdToClose - The ID of the send socket to close.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|UDP Subsystem")
		bool CloseSendSocket(int32 SendSocketIdToClose);
	/**
	 * Sends bytes over the opened send socket.
	 * Returns whether or not the sending was successful for every opened socket.
	 * @param Bytes - The bytes to send over UDP.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|UDP Subsystem")
		bool EmitBytes(const TArray<uint8>& Bytes);
	/**
	 * Closes all opened receive sockets.
	 * Returns whether or not all of the receive sockets were closed successfully. If none are opened, returns true.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|UDP Subsystem")
		bool CloseAllReceiveSockets();
	/**
	 * Closes all opened send sockets.
	 * Returns whether or not all of the send sockets were closed successfully. If none are opened, returns true.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|UDP Subsystem")
		bool CloseAllSendSockets();
	/**
	 * Gets the IDs of the connected receive sockets, if any.
	 * Returns an array containing all of the connected receive socket IDs.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|UDP Subsystem")
		TArray<int32> GetConnectedReceiveSocketIDs();
	/**
	 * Gets the IDs of the connected send sockets, if any.
	 * Returns an array containing all of the connected send socket IDs.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|UDP Subsystem")
		TArray<int32> GetConnectedSendSocketIDs();
	/**
	 * Checks whether there are any connected receive or send sockets.
	 * Returns a boolean containing whether or not any sockets are connected.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|UDP Subsystem")
		bool AnyConnectedSockets();

protected:
	ISocketSubsystem* SocketSubsystem;

	TMap<int32, FSocket*> AllSendSockets;
	TMap<int32, FReceiveSocketMapValue> AllReceiveSockets;

private:
	int TotalSendSocketIterator = 0;
	int TotalReceiveSocketIterator = 0;

	FString LocalIPAddress;
};