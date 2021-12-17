// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OpenDISComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DISEntity_Base.generated.h"

UCLASS(Blueprintable)
class UEOPENDIS_API ADISEntity_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADISEntity_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UOpenDISComponent* OpenDISComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
