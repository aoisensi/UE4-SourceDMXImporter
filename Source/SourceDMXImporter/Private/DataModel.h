// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Guid.h"

class FDataModel
{
public:
	bool FailedParse = false;

	struct FLiteral
	{
	public:
		FString Type;
		FString Value;
		TArray<FString> Array;
		TOptional<FGuid> Id;
		TArray<FGuid> IdArray;

		bool GetString(FString& Out);
		bool GetBool(bool& Out);
		bool GetInt(int32& Out);
		bool GetVector(FVector& Out);
		bool GetQuat(FQuat& Out);
		bool GetElementArray(TArray<FGuid>& Out);
		bool GetStringArray(TArray<FString>& Out);
		bool GetIntArray(TArray<int32>& Out);
		bool GetVector2DArray(TArray<FVector2D>& Out);
		bool GetVectorArray(TArray<FVector>& Out);
	};

	struct FElement
	{
	public:
		FGuid Id;
		FString Type;
		TMap<FString, FLiteral> Map;
		FLiteral Find(const FString& Key) const;
	};

	TMap<FGuid, FElement> Elements;
	TOptional<FGuid> RootId;
};