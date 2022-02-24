// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DISComponent.h"
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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DIS Interface")
		UDISComponent* GetActorDISComponent();
};