// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Guid.h"

class FDataModel
{
public:
	bool FailedParse;

	struct FLiteral
	{
		FString Type;
		FString Value;
		TArray<FString> Array;
		TOptional<FGuid> Id;
		TArray<FGuid> IdArray;
	};

	struct FElement
	{
		FGuid Id;
		FString Type;
		TMap<FString, FLiteral> Map;
	};

	TMap<FGuid, FElement> Elements;
};