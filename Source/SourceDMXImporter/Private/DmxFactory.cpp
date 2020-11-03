// Fill out your copyright notice in the Description page of Project Settings.

#include "DmxFactory.h"
#include "Engine/StaticMesh.h"
#include "DmxImporter.h"

UDmxFactory::UDmxFactory(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	SupportedClass = nullptr;
	Formats.Add(TEXT("dmx;Datamodel meshes and animations"));
	bCreateNew = false;
	bText = false;
	bEditorImport = true;
	bDetectImportTypeOnImport = true;
}

bool UDmxFactory::DoesSupportClass(UClass* Class)
{
	return (Class == UStaticMesh::StaticClass());
}

UClass* UDmxFactory::ResolveSupportedClass()
{
	return UStaticMesh::StaticClass();
}

UObject* UDmxFactory::FactoryCreateFile(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	const FString& Filename,
	const TCHAR* Parms,
	FFeedbackContext* Warn,
	bool& bOutOperationCanceled
)
{
	FDmxImporter Importer;
	Importer.OpenFile(Filename);
	bOutOperationCanceled = true;
	return nullptr;
};