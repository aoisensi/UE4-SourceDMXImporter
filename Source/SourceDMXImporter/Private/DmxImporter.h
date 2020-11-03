// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FDmxImporter
{

public:
	bool OpenFile(const FString& Filename);

private:
	bool ReadHeader(const FString& Filename);

	int32 HeaderSize;
	FName EncodingType;
	int32 EncodingVer;
	FName FormatType;
	int32 FormatVer;

	const int32 MaxHeaderSize = 256;

};
