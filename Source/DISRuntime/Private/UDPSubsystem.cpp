// Fill out your copyright notice in the Description page of Project Settings.

#include "UDPSubsystem.h"
#include "DISRuntimeSettings.h"

DEFINE_LOG_CATEGORY(LogUDPSubsystem);

void UUDPSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//Load settings from the OpenDIS ProjectSettings
	const UDISRuntimeSettings* const ProjectSettings = UDISRuntimeSettings::Get();
	check(ProjectSettings);

	Settings.bShouldAutoOpenSend = ProjectSettings->AutoConnectSend;
	Settings.SendIP = ProjectSettings->AutoSendIPAddress;
	Settings.SendPort = ProjectSettings->AutoSendPort;
	Settings.bShouldAutoOpenReceive = ProjectSettings->AutoConnectReceive;
	Settings.ReceiveIP = ProjectSettings->AutoReceiveIPAddress;
	Settings.ReceivePort = ProjectSettings->AutoReceivePort;

	if (Settings.bShouldAutoOpenSend)
	{
		OpenSendSocket(Settings.SendIP, Settings.SendPort);
	}

	if (Settings.bShouldAutoOpenReceive)
	{
		OpenReceiveSocket(Settings.ReceiveIP, Settings.ReceivePort);
	}
}

void UUDPSubsystem::Deinitialize()
{
	CloseSendSocket();
	CloseReceiveSocket();

	Super::Deinitialize();
}

bool UUDPSubsystem::CloseReceiveSocket()
{
	bool bDidCloseCorrectly = true;
	Settings.bIsReceiveOpen = false;

	if (ReceiverSocket)
	{
		UDPReceiver->Stop();
		delete UDPReceiver;
		UDPReceiver = nullptr;

		bDidCloseCorrectly = ReceiverSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ReceiverSocket);
		ReceiverSocket = nullptr;

		if (OnReceiveSocketClosed.IsBound())
		{
			OnReceiveSocketClosed.Broadcast(Settings.ReceivePort);
		}
	}

	return bDidCloseCorrectly;
}

bool UUDPSubsystem::OpenSendSocket(const FString& InIP /*= TEXT("127.0.0.1")*/, const int32 InPort /*= 3000*/)
{
	Settings.SendIP = InIP;
	Settings.SendPort = InPort;

	RemoteAdress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool bIsValid;
	RemoteAdress->SetIp(*Settings.SendIP, bIsValid);
	RemoteAdress->SetPort(Settings.SendPort);

	if (!bIsValid)
	{
		UE_LOG(LogUDPSubsystem, Error, TEXT("UDP address is invalid <%s:%d>"), *Settings.SendIP, Settings.SendPort);
		return 0;
	}

	SenderSocket = FUdpSocketBuilder(*Settings.SendSocketName).AsReusable().WithBroadcast();

	//Set Send Buffer Size
	SenderSocket->SetSendBufferSize(Settings.BufferSize, Settings.BufferSize);
	SenderSocket->SetReceiveBufferSize(Settings.BufferSize, Settings.BufferSize);

	bool bDidConnect = SenderSocket->Connect(*RemoteAdress);
	Settings.bIsSendOpen = true;
	Settings.SendPort = SenderSocket->GetPortNo();

	if (OnSendSocketOpened.IsBound())
	{
		OnSendSocketOpened.Broadcast(Settings.SendPort);
	}

	return Settings.bIsSendOpen;
}

bool UUDPSubsystem::CloseSendSocket()
{
	Settings.SendPort = 0;

	bool bDidCloseCorrectly = true;
	Settings.bIsSendOpen = false;

	if (SenderSocket)
	{
		bDidCloseCorrectly = SenderSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SenderSocket);
		SenderSocket = nullptr;

		if (OnSendSocketClosed.IsBound())
		{
			OnSendSocketClosed.Broadcast(Settings.SendPort);
		}
	}

	return bDidCloseCorrectly;
}

bool UUDPSubsystem::OpenReceiveSocket(const FString& InListenIp /*= TEXT("0.0.0.0")*/, const int32 InListenPort /*= 3002*/)
{
	Settings.ReceiveIP = InListenIp;
	Settings.ReceivePort = InListenPort;

	bool bDidOpenCorrectly = true;

	if (Settings.bIsReceiveOpen)
	{
		bDidOpenCorrectly = CloseReceiveSocket();
	}

	FIPv4Address Addr;
	FIPv4Address::Parse(Settings.ReceiveIP, Addr);
	//Create Socket
	FIPv4Endpoint Endpoint(Addr, Settings.ReceivePort);

	ReceiverSocket = FUdpSocketBuilder(*Settings.ReceiveSocketName)
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(Settings.BufferSize);

	if (ReceiverSocket == nullptr)
	{
		UE_LOG(LogUDPSubsystem, Error, TEXT("Failed to bind to %s:%s! Setup of receive socket failed."), *Settings.ReceiveIP, *FString::FromInt(Settings.ReceivePort));
		return false;
	}

	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
	FString ThreadName = FString::Printf(TEXT("UDP RECEIVER-FUDPWrapper"));
	UDPReceiver = new FUdpSocketReceiver(ReceiverSocket, ThreadWaitTime, *ThreadName);

	UDPReceiver->OnDataReceived().BindLambda([this](const FArrayReaderPtr& DataPtr, const FIPv4Endpoint& Endpoint)
	{
		if (!OnReceivedBytes.IsBound())
		{
			return;
		}

		TArray<uint8> Data;
		Data.AddUninitialized(DataPtr->TotalSize());
		DataPtr->Serialize(Data.GetData(), DataPtr->TotalSize());

		FString SenderIp = Endpoint.Address.ToString();

		if (Settings.bReceiveDataOnGameThread)
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

	Settings.bIsReceiveOpen = true;

	if (OnReceiveSocketOpened.IsBound())
	{
		OnReceiveSocketOpened.Broadcast(Settings.ReceivePort);
	}

	UDPReceiver->Start();

	return bDidOpenCorrectly;
}

bool UUDPSubsystem::EmitBytes(const TArray<uint8>& Bytes)
{
	bool bDidSendCorrectly = true;

	if (SenderSocket && SenderSocket->GetConnectionState() == SCS_Connected)
	{
		int32 BytesSent = 0;
		bDidSendCorrectly = SenderSocket->Send(Bytes.GetData(), Bytes.Num(), BytesSent);
	}
	else if (Settings.bShouldAutoOpenSend)
	{
		bool bDidOpen = OpenSendSocket(Settings.SendIP, Settings.SendPort);
		return bDidOpen && EmitBytes(Bytes);
	}

	return bDidSendCorrectly;
}

void UUDPSubsystem::GetUDPReceiveSettings(bool& CurrentlyConnected, FString& ReceiveIPAddress, int32& ReceivePort, bool& AutoConnectReceive)
{
	CurrentlyConnected = Settings.bIsReceiveOpen;
	ReceiveIPAddress = Settings.ReceiveIP;
	ReceivePort = Settings.ReceivePort;
	AutoConnectReceive = Settings.bShouldAutoOpenReceive;
}

void UUDPSubsystem::GetUDPSendSettings(bool& CurrentlyConnected, FString& SendIPAddress, int32& SendPort, bool& AutoConnectSend)
{
	CurrentlyConnected = Settings.bIsSendOpen;
	SendIPAddress = Settings.SendIP;
	SendPort = Settings.SendPort;
	AutoConnectSend = Settings.bShouldAutoOpenSend;
}
