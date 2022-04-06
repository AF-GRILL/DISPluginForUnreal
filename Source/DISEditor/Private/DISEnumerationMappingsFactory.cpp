// Fill out your copyright notice in the Description page of Project Settings.


#include "DISEnumerationMappingsFactory.h"


UDISEnumerationMappingsFactory::UDISEnumerationMappingsFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	//Configure the class that this factory creates
	SupportedClass = UDISClassEnumMappings::StaticClass();
}

UObject* UDISEnumerationMappingsFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	//Create the editor asset 
	UDISClassEnumMappings* DISEnumerationMappingsEditorAsset = NewObject<UDISClassEnumMappings>(InParent, InClass, InName, Flags);
	return DISEnumerationMappingsEditorAsset;
}
