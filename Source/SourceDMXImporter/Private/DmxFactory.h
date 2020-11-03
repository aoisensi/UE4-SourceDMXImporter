// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "DmxFactory.generated.h"

UCLASS()
class SOURCEDMXIMPORTER_API UDmxFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual bool DoesSupportClass(UClass* Class) override;
	virtual UClass* ResolveSupportedClass() override;
	virtual UObject* FactoryCreateFile(
		UClass* InClass,
	    UObject* InParent,
	    FName InName,
	    EObjectFlags Flags,
	    const FString& Filename,
	    const TCHAR* Parms,
	    FFeedbackContext* Warn,
	    bool& bOutOperationCanceled
	) override;

protected:
	bool bDetectImportTypeOnImport;
	
};
