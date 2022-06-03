// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.


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
