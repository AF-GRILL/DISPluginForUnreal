// Fill out your copyright notice in the Description page of Project Settings.

#include "UDPSubsystem.h"
#include "DISRuntimeSettings.h"

DEFINE_LOG_CATEGORY(LogUDPSubsystem);

void UUDPSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	bool canBindAll = false;
	TSharedRef<FInternetAddr> Sender = SocketSubsystem->GetLocalHostAddr(*GLog, canBindAll);
	LocalIPAddress = Sender->ToString(false);
}

void UUDPSubsystem::Deinitialize()
{
	CloseAllSendSockets();
	CloseAllReceiveSockets();

	Super::Deinitialize();
}

bool UUDPSubsystem::OpenReceiveSocket(FReceiveSocketSettings SocketSettings, int32& ReceiveSocketID, const FString& IpToListenOn /*= TEXT("0.0.0.0")*/, const int32 PortToListenOn /*= 3002*/)
{
	FIPv4Address Addr;
	FIPv4Address::Parse(IpToListenOn, Addr);
	//Create Socket

	FSocket* ReceiverSocket;
	FUdpSocketBuilder SocketBuilder = FUdpSocketBuilder(SocketSettings.SocketDescription)
		.AsNonBlocking()
		//.AsReusable()
		.WithReceiveBufferSize(SocketSettings.BufferSize);
		
	//Handle setting up of socket based on multicast or not
	if (SocketSettings.bUseMulticast)
	{
		FIPv4Endpoint Endpoint(FIPv4Address::Any, PortToListenOn);
		ReceiverSocket = SocketBuilder.BoundToEndpoint(Endpoint).JoinedToGroup(Addr);
	}
	else
	{
		FIPv4Endpoint Endpoint(Addr, PortToListenOn);
		ReceiverSocket = SocketBuilder.BoundToEndpoint(Endpoint);
	}

	if (ReceiverSocket == nullptr)
	{
		UE_LOG(LogUDPSubsystem, Error, TEXT("Failed to bind to address <%s:%d>! Setup of receive socket failed. Verify given IP and Port are in valid ranges and that another socket is not already set up on the given address."), *IpToListenOn, PortToListenOn);
		return false;
	}

	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
	FString ThreadName = FString::Printf(TEXT("UDP RECEIVER-FUDPWrapper"));
	FUdpSocketReceiver* UDPReceiver = new FUdpSocketReceiver(ReceiverSocket, ThreadWaitTime, *ThreadName);

	UDPReceiver->OnDataReceived().BindLambda([this, SocketSettings](const FArrayReaderPtr& DataPtr, const FIPv4Endpoint& Endpoint)
	{
		SCOPE_CYCLE_COUNTER(STAT_ReceiveBytes);
		if (!OnReceivedBytes.IsBound())
		{
			return;
		}

		TArray<uint8> Data;
		Data.AddUninitialized(DataPtr->TotalSize());
		DataPtr->Serialize(Data.GetData(), DataPtr->TotalSize());

		FString SenderIp = Endpoint.Address.ToString();

		if (SocketSettings.bIgnorePacketsFromLocalIP && SenderIp.Equals(LocalIPAddress))
		{
			return;
		}

		if (SocketSettings.bReceiveDataOnGameThread)
		{
			//Copy data to receiving thread via lambda capture
			AsyncTask(ENamedThreads::GameThread, [this, Data, SenderIp]()
			{
				//double check we're still bound on this thread
				if (OnReceivedBytes.IsBound())
				{
					OnReceivedBytes.Broadcast(Data, SenderIp);
				}
			});
		}
		else
		{
			OnReceivedBytes.Broadcast(Data, SenderIp);
		}
	});

	if (OnReceiveSocketOpened.IsBound())
	{
		OnReceiveSocketOpened.Broadcast(TotalReceiveSocketIterator, *IpToListenOn, PortToListenOn);
	}

	UDPReceiver->Start();

	//Add new receive socket info to map and increase iterator
	AllReceiveSockets.Add(TotalReceiveSocketIterator, FReceiveSocketMapValue(ReceiverSocket, UDPReceiver));
	ReceiveSocketID = TotalSendSocketIterator;
	TotalReceiveSocketIterator++;

	return true;
}

bool UUDPSubsystem::CloseReceiveSocket(int32 ReceiveSocketIdToClose)
{
	bool bDidCloseCorrectly = true;

	FReceiveSocketMapValue* MapValue = AllReceiveSockets.Find(ReceiveSocketIdToClose);

	if (MapValue)
	{
		//Get IP and Port the socket was listening on prior to closing it
		TSharedRef<FInternetAddr> Sender = SocketSubsystem->CreateInternetAddr();
		MapValue->ReceiveSocket->GetAddress(*Sender);
		FIPv4Endpoint Endpoint = FIPv4Endpoint(Sender);

		FString Ip = Endpoint.Address.ToString();
		int32 Port = Endpoint.Port;

		//Close the receive socket
		bDidCloseCorrectly = MapValue->CloseReceiveSocket();

		//If bound, broadcast Receive Socket Closed event
		if (OnReceiveSocketClosed.IsBound())
		{
			OnReceiveSocketClosed.Broadcast(ReceiveSocketIdToClose, Ip, Port);
		}

		AllReceiveSockets.Remove(ReceiveSocketIdToClose);
	}

	return bDidCloseCorrectly;
}

bool UUDPSubsystem::OpenSendSocket(FSendSocketSettings SocketSettings, int32& SendSocketID, const FString& IpToSendOn /*= TEXT("127.0.0.1")*/, const int32 PortToSendOn /*= 3000*/)
{
	TSharedPtr<FInternetAddr> RemoteAdress = SocketSubsystem->CreateInternetAddr();

	bool bIsValid;
	RemoteAdress->SetIp(*IpToSendOn, bIsValid);
	RemoteAdress->SetPort(PortToSendOn);

	if (!bIsValid)
	{
		UE_LOG(LogUDPSubsystem, Error, TEXT("UDP address is invalid <%s:%d>"), *IpToSendOn, PortToSendOn);
		return 0;
	}

	FSocket* SenderSocket;
	FUdpSocketBuilder SocketBuilder = FUdpSocketBuilder(SocketSettings.SocketDescription).AsReusable();
	
	//Handle setting up send socket based on different connection types
	switch (SocketSettings.SendSocketConnectionType)
	{
	case EConnectionType::Broadcast:
	{
		SenderSocket = SocketBuilder.WithBroadcast();
		break;
	}
	case EConnectionType::Multicast:
	{
		FIPv4Address Addr;
		FIPv4Address::Parse(IpToSendOn, Addr);
		SenderSocket = SocketBuilder.JoinedToGroup(Addr);
		break;
	}
	case EConnectionType::Unicast:
	{
		SenderSocket = SocketBuilder;
		break;
	}
	default:
		UE_LOG(LogUDPSubsystem, Error, TEXT("Invalid Connection Type passed to Open Send Socket!"));
		return false;
	}

	//Set Send Buffer Size
	SenderSocket->SetSendBufferSize(SocketSettings.BufferSize, SocketSettings.BufferSize);
	SenderSocket->SetReceiveBufferSize(SocketSettings.BufferSize, SocketSettings.BufferSize);

	bool bDidConnect = SenderSocket->Connect(*RemoteAdress);
	int32 LocalPort = SenderSocket->GetPortNo();

	TSharedRef<FInternetAddr> Sender = SocketSubsystem->CreateInternetAddr();
	SenderSocket->GetAddress(*Sender);
	FString LocalIp = FIPv4Endpoint(Sender).Address.ToString();

	if (OnSendSocketOpened.IsBound())
	{
		OnSendSocketOpened.Broadcast(TotalSendSocketIterator, LocalIp, LocalPort, IpToSendOn, PortToSendOn);
	}

	//Add new send socket info to map and increase iterator
	AllSendSockets.Add(TotalSendSocketIterator, SenderSocket);
	SendSocketID = TotalSendSocketIterator;
	TotalSendSocketIterator++;

	return true;
}

bool UUDPSubsystem::CloseSendSocket(int32 SendSocketIdToClose)
{
	bool bDidCloseCorrectly = true;

	FSocket* SocketToClose = *AllSendSockets.Find(SendSocketIdToClose);

	if (SocketToClose)
	{
		//Get the IP and Port the socket was sending to prior to closing it
		TSharedRef<FInternetAddr> Sender = SocketSubsystem->CreateInternetAddr();
		SocketToClose->GetAddress(*Sender);
		FIPv4Endpoint LocalEndpoint = FIPv4Endpoint(Sender);
		SocketToClose->GetPeerAddress(*Sender);
		FIPv4Endpoint PeerEndpoint = FIPv4Endpoint(Sender);

		FString LocalIp = LocalEndpoint.Address.ToString();
		int32 LocalPort = LocalEndpoint.Port;
		FString PeerIp = PeerEndpoint.Address.ToString();
		int32 PeerPort = PeerEndpoint.Port;

		//Close the send socket
		bDidCloseCorrectly = SocketToClose->Close();
		SocketSubsystem->DestroySocket(SocketToClose);
		SocketToClose = nullptr;

		//If bound, broadcast the Send Socket Closed event
		if (OnSendSocketClosed.IsBound())
		{
			OnSendSocketClosed.Broadcast(SendSocketIdToClose, LocalIp, LocalPort, PeerIp, PeerPort);
		}

		AllSendSockets.Remove(SendSocketIdToClose);
	}

	return bDidCloseCorrectly;
}

bool UUDPSubsystem::EmitBytes(const TArray<uint8>& Bytes)
{
	SCOPE_CYCLE_COUNTER(STAT_SendBytes);
	bool bDidSendCorrectly = true;

	for (const TPair<int32, FSocket*>& pair : AllSendSockets) 
	{
		FSocket* SendSocket = pair.Value;

		if (SendSocket && SendSocket->GetConnectionState() == SCS_Connected)
		{
			int32 BytesSent = 0;
			bDidSendCorrectly = bDidSendCorrectly && SendSocket->Send(Bytes.GetData(), Bytes.Num(), BytesSent);
		}
	}
	return bDidSendCorrectly;
}

bool UUDPSubsystem::CloseAllReceiveSockets()
{
	bool allClosedSuccessfully = true;

	//Create a copy of the map to prevent removing entries while iterating
	TMap<int32, FReceiveSocketMapValue> tempMap = AllReceiveSockets;

	for (const TPair<int32, FReceiveSocketMapValue>& pair : tempMap)
	{
		allClosedSuccessfully = allClosedSuccessfully && CloseReceiveSocket(pair.Key);
	}

	return allClosedSuccessfully;
}

bool UUDPSubsystem::CloseAllSendSockets()
{
	bool allClosedSuccessfully = true;

	//Create a copy of the map to prevent removing entries while iterating
	TMap<int32, FSocket*> tempMap = AllSendSockets;

	for (const TPair<int32, FSocket*>& pair : tempMap)
	{
		allClosedSuccessfully = allClosedSuccessfully && CloseSendSocket(pair.Key);
	}

	return allClosedSuccessfully;
}