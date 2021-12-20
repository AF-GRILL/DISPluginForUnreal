// Fill out your copyright notice in the Description page of Project Settings.


#include "DISEntity_Base.h"

// Sets default values
ADISEntity_Base::ADISEntity_Base()
{
	OpenDISComponent = CreateDefaultSubobject<UOpenDISComponent>(TEXT("OpenDIS Component"));

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADISEntity_Base::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADISEntity_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADISEntity_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}