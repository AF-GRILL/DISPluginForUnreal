// Fill out your copyright notice in the Description page of Project Settings.


#include "UDPComponent.h"

UUDPComponent::UUDPComponent()
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;

	UDPWrapper = MakeShareable(new FUDPWrapper);

	LinkupCallbacks();
}

void UUDPComponent::BeginPlay()
{
	Super::BeginPlay();

	//Sync all settings to UDPWrapper. These are duplicated for dev convenience in bp
	UDPWrapper->CurrentUDPSettings = Settings;

	if (Settings.bShouldAutoOpenSend)
	{
		OpenSendSocket(Settings.SendIP, Settings.SendPort);
	}

	if (Settings.bShouldAutoOpenReceive)
	{
		OpenReceiveSocket(Settings.ReceiveIP, Settings.ReceivePort);
	}
}

void UUDPComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CloseSendSocket();
	CloseReceiveSocket();

	UDPWrapper->ClearSendCallbacks();
	UDPWrapper->ClearReceiveCallbacks();

	Super::EndPlay(EndPlayReason);
}

void UUDPComponent::LinkupCallbacks()
{
	UDPWrapper->OnSendOpened = [this](int32 BoundPort)
	{
		Settings.bIsSendOpen = true;
		Settings.SendPort = BoundPort;	//ensure sync on opened bound port

		Settings.SendIP = UDPWrapper->CurrentUDPSettings.SendIP;
		Settings.SendPort = UDPWrapper->CurrentUDPSettings.SendPort;

		OnSendSocketOpened.Broadcast(Settings.SendPort);
	};
	UDPWrapper->OnSendClosed = [this](int32 Port)
	{
		Settings.bIsSendOpen = false;
		OnSendSocketClosed.Broadcast(Port);
	};
	UDPWrapper->OnReceiveOpened = [this](int32 Port)
	{
		Settings.ReceiveIP = UDPWrapper->CurrentUDPSettings.ReceiveIP;
		Settings.ReceivePort = UDPWrapper->CurrentUDPSettings.ReceivePort;

		Settings.bIsReceiveOpen = true;
		OnReceiveSocketOpened.Broadcast(Port);
	};
	UDPWrapper->OnReceiveClosed = [this](int32 Port)
	{
		Settings.bIsReceiveOpen = false;
		OnReceiveSocketClosed.Broadcast(Port);
	};
	UDPWrapper->OnReceivedBytes = [this](const TArray<uint8>& Data, const FString& Endpoint)
	{
		OnReceivedBytes.Broadcast(Data, Endpoint);
	};
}

bool UUDPComponent::CloseReceiveSocket()
{
	return UDPWrapper->CloseReceiveSocket();
}

bool UUDPComponent::OpenSendSocket(const FString& InIP /*= TEXT("127.0.0.1")*/, const int32 InPort /*= 3000*/)
{
	//Sync side effect sampled settings
	UDPWrapper->CurrentUDPSettings.SendSocketName = Settings.SendSocketName;
	UDPWrapper->CurrentUDPSettings.BufferSize = Settings.BufferSize;

	return UDPWrapper->OpenSendSocket(InIP, InPort);
}

bool UUDPComponent::CloseSendSocket()
{
	Settings.SendPort = 0;
	return UDPWrapper->CloseSendSocket();
}

bool UUDPComponent::OpenReceiveSocket(const FString& InListenIp /*= TEXT("0.0.0.0")*/, const int32 InListenPort /*= 3002*/)
{
	//Sync side effect sampled settings
	UDPWrapper->CurrentUDPSettings.bShouldAutoOpenReceive = Settings.bShouldAutoOpenReceive;
	UDPWrapper->CurrentUDPSettings.ReceiveSocketName = Settings.ReceiveSocketName;
	UDPWrapper->CurrentUDPSettings.BufferSize = Settings.BufferSize;

	return UDPWrapper->OpenReceiveSocket(InListenIp, InListenPort);
}

bool UUDPComponent::EmitBytes(const TArray<uint8>& Bytes)
{
	return UDPWrapper->EmitBytes(Bytes);
}

FUDPWrapper::FUDPWrapper()
{
	SenderSocket = nullptr;
	ReceiverSocket = nullptr;

	ClearReceiveCallbacks();
	ClearSendCallbacks();
}

FUDPWrapper::~FUDPWrapper()
{
	if (CurrentUDPSettings.bIsReceiveOpen)
	{
		CloseReceiveSocket();
		ClearReceiveCallbacks();
	}
	if (CurrentUDPSettings.bIsSendOpen)
	{
		CloseSendSocket();
		ClearSendCallbacks();
	}
}

bool FUDPWrapper::OpenSendSocket(const FString& InIP, const int32 InPort)
{
	CurrentUDPSettings.SendIP = InIP;
	CurrentUDPSettings.SendPort = InPort;

	RemoteAdress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool bIsValid;
	RemoteAdress->SetIp(*CurrentUDPSettings.SendIP, bIsValid);
	RemoteAdress->SetPort(CurrentUDPSettings.SendPort);

	if (!bIsValid)
	{
		UE_LOG(LogTemp, Error, TEXT("UDP address is invalid <%s:%d>"), *CurrentUDPSettings.SendIP, CurrentUDPSettings.SendPort);
		return 0;
	}

	SenderSocket = FUdpSocketBuilder(*CurrentUDPSettings.SendSocketName).AsReusable().WithBroadcast();

	//Set Send Buffer Size
	SenderSocket->SetSendBufferSize(CurrentUDPSettings.BufferSize, CurrentUDPSettings.BufferSize);
	SenderSocket->SetReceiveBufferSize(CurrentUDPSettings.BufferSize, CurrentUDPSettings.BufferSize);

	bool bDidConnect = SenderSocket->Connect(*RemoteAdress);
	CurrentUDPSettings.bIsSendOpen = true;
	CurrentUDPSettings.SendPort = SenderSocket->GetPortNo();

	if (OnSendOpened)
	{
		OnSendOpened(CurrentUDPSettings.SendPort);
	}

	return CurrentUDPSettings.bIsSendOpen;
}

bool FUDPWrapper::CloseSendSocket()
{
	bool bDidCloseCorrectly = true;
	CurrentUDPSettings.bIsSendOpen = false;

	if (SenderSocket)
	{
		bDidCloseCorrectly = SenderSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SenderSocket);
		SenderSocket = nullptr;

		if (OnSendClosed)
		{
			OnSendClosed(CurrentUDPSettings.SendPort);
		}
	}

	return bDidCloseCorrectly;
}

bool FUDPWrapper::EmitBytes(const TArray<uint8>& Bytes)
{
	bool bDidSendCorrectly = true;

	if (SenderSocket && SenderSocket->GetConnectionState() == SCS_Connected)
	{
		int32 BytesSent = 0;
		bDidSendCorrectly = SenderSocket->Send(Bytes.GetData(), Bytes.Num(), BytesSent);
	}
	else if (CurrentUDPSettings.bShouldAutoOpenSend)
	{
		bool bDidOpen = OpenSendSocket(CurrentUDPSettings.SendIP, CurrentUDPSettings.SendPort);
		return bDidOpen && EmitBytes(Bytes);
	}

	return bDidSendCorrectly;
}

bool FUDPWrapper::OpenReceiveSocket(const FString& InIP, const int32 InListenPort)
{
	CurrentUDPSettings.ReceiveIP = InIP;
	CurrentUDPSettings.ReceivePort = InListenPort;

	bool bDidOpenCorrectly = true;

	if (CurrentUDPSettings.bIsReceiveOpen)
	{
		bDidOpenCorrectly = CloseReceiveSocket();
	}

	FIPv4Address Addr;
	FIPv4Address::Parse(CurrentUDPSettings.ReceiveIP, Addr);
	//Create Socket
	FIPv4Endpoint Endpoint(Addr, CurrentUDPSettings.ReceivePort);

	ReceiverSocket = FUdpSocketBuilder(*CurrentUDPSettings.ReceiveSocketName)
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(CurrentUDPSettings.BufferSize);

	if (ReceiverSocket == nullptr) 
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to bind to %s:%s! Setup of receive socket failed."), *CurrentUDPSettings.ReceiveIP, *FString::FromInt(CurrentUDPSettings.ReceivePort));
		return false;
	}

	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
	FString ThreadName = FString::Printf(TEXT("UDP RECEIVER-FUDPWrapper"));
	UDPReceiver = new FUdpSocketReceiver(ReceiverSocket, ThreadWaitTime, *ThreadName);

	UDPReceiver->OnDataReceived().BindLambda([this](const FArrayReaderPtr& DataPtr, const FIPv4Endpoint& Endpoint)
	{
		if (!OnReceivedBytes)
		{
			return;
		}

		TArray<uint8> Data;
		Data.AddUninitialized(DataPtr->TotalSize());
		DataPtr->Serialize(Data.GetData(), DataPtr->TotalSize());

		FString SenderIp = Endpoint.Address.ToString();

		if (CurrentUDPSettings.bReceiveDataOnGameThread)
		{
			//Copy data to receiving thread via lambda capture
			AsyncTask(ENamedThreads::GameThread, [this, Data, SenderIp]()
			{
				//double check we're still bound on this thread
				if (OnReceivedBytes)
				{
					OnReceivedBytes(Data, SenderIp);
				}
			});
		}
		else
		{
			OnReceivedBytes(Data, SenderIp);
		}
	});

	CurrentUDPSettings.bIsReceiveOpen = true;

	if (OnReceiveOpened)
	{
		OnReceiveOpened(CurrentUDPSettings.ReceivePort);
	}

	UDPReceiver->Start();

	return bDidOpenCorrectly;
}

bool FUDPWrapper::CloseReceiveSocket()
{
	bool bDidCloseCorrectly = true;
	CurrentUDPSettings.bIsReceiveOpen = false;

	if (ReceiverSocket)
	{
		UDPReceiver->Stop();
		delete UDPReceiver;
		UDPReceiver = nullptr;

		bDidCloseCorrectly = ReceiverSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ReceiverSocket);
		ReceiverSocket = nullptr;

		if (OnReceiveClosed)
		{
			OnReceiveClosed(CurrentUDPSettings.ReceivePort);
		}
	}

	return bDidCloseCorrectly;
}

void FUDPWrapper::ClearSendCallbacks()
{
	OnSendOpened = nullptr;
	OnSendClosed = nullptr;
}

void FUDPWrapper::ClearReceiveCallbacks()
{
	OnReceivedBytes = nullptr;
	OnReceiveOpened = nullptr;
	OnReceiveClosed = nullptr;
}