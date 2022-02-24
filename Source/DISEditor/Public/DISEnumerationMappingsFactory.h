// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "AssetTypeActions_Base.h"
#include "DISClassEnumMappings.h"
#include "DISEnumerationMappingsFactory.generated.h"

/**
 * 
 */
UCLASS()
class DISEDITOR_API UDISEnumerationMappingsFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UDISEnumerationMappingsFactory();

	/* Creates the asset inside the UE4 Editor */
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

class UDISEnumerationMappingsDatabase : public FAssetTypeActions_Base {
public:
	virtual FText GetName() const override { return FText::FromString("DIS Enumeration Mappings"); }
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Blueprint;; }
	virtual FColor GetTypeColor() const override { return FColor(127, 255, 255); }
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override { return FText::FromString("Mappings between DIS Enumerations and Unreal Engine actors."); }
	virtual UClass* GetSupportedClass() const override { return UDISClassEnumMappings::StaticClass(); }
};