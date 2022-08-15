// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DISReceiveComponent.h"
#include "DISSendComponent.h"
#include "DISInterface.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType)
class DISRUNTIME_API UDISInterface : public UInterface
{
	GENERATED_BODY()
};

class DISRUNTIME_API IDISInterface 
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GRILL DIS|DIS Interface")
		UDISReceiveComponent* GetActorDISReceiveComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GRILL DIS|DIS Interface")
		UDISSendComponent* GetActorDISSendComponent();
};