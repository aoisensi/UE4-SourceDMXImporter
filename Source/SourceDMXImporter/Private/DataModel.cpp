// Fill out your copyright notice in the Description page of Project Settings.

#include "DataModel.h"

bool FDataModel::FLiteral::GetString(FString& Out)
{
	if (Type != TEXT("string")) return false;
	Out = Value;
	return true;
}

bool FDataModel::FLiteral::GetBool(bool& Out)
{
	if (Type != TEXT("bool")) return false;
	Out = Value == TEXT("1");
	return true;
}

bool FDataModel::FLiteral::GetInt(int32& Out)
{
	if (Type != TEXT("int")) return false;
	Out = FCString::Atoi(*Value);
	return true;
}

bool FDataModel::FLiteral::GetVector(FVector& Out)
{
	if (Type != TEXT("vector3")) return false;
	TArray<FString> Strings;
	if (Value.ParseIntoArray(Strings, TEXT(" ")) != 3) return false;
	Out = FVector(
		FCString::Atof(*Strings[0]),
		FCString::Atof(*Strings[1]),
		FCString::Atof(*Strings[2])
	);
	return true;
}

bool FDataModel::FLiteral::GetQuat(FQuat& Out)
{
	if (Type != TEXT("quaternion")) return false;
	TArray<FString> Strings;
	if (Value.ParseIntoArray(Strings, TEXT(" ")) != 4) return false;
	Out = FQuat(
		FCString::Atof(*Strings[0]),
		FCString::Atof(*Strings[1]),
		FCString::Atof(*Strings[2]),
		FCString::Atof(*Strings[3])
	);
	return true;
}


bool FDataModel::FLiteral::GetElementArray(TArray<FGuid>& Out)
{
	if (Type != TEXT("element_array")) return false;
	Out = IdArray;
	return true;
}

bool FDataModel::FLiteral::GetStringArray(TArray<FString>& Out)
{
	if (Type != TEXT("string_array")) return false;
	Out = Array;
	return true;
}

bool FDataModel::FLiteral::GetIntArray(TArray<int32>& Out)
{
	if (Type != TEXT("int_array")) return false;
	auto Size = Array.Num();
	Out.SetNum(Size);
	for (auto I = 0; I < Size; I++)
	{
		Out[I] = FCString::Atoi(*Array[I]);
	}
	return true;
}

bool FDataModel::FLiteral::GetVector2DArray(TArray<FVector2D>& Out)
{
	if (Type != TEXT("vector2_array")) return false;
	auto Size = Array.Num();
	Out.SetNum(Size);
	for (auto I = 0; I < Size; I++)
	{
		TArray<FString> Strings;
		if (Array[I].ParseIntoArray(Strings, TEXT(" ")) != 2) return false;
		Out[I] = FVector2D(
			FCString::Atof(*Strings[0]),
			FCString::Atof(*Strings[1])
		);
	}
	return true;
}

bool FDataModel::FLiteral::GetVectorArray(TArray<FVector>& Out)
{
	if (Type != TEXT("vector3_array")) return false;
	auto Size = Array.Num();
	Out.SetNum(Size);
	for (auto I = 0; I < Size; I++)
	{
		TArray<FString> Strings;
		if (Array[I].ParseIntoArray(Strings, TEXT(" ")) != 3) return false;
		Out[I] = FVector(
			FCString::Atof(*Strings[0]),
			FCString::Atof(*Strings[1]),
			FCString::Atof(*Strings[2])
		);
	}
	return true;
}

FDataModel::FLiteral FDataModel::FElement::Find(const FString& Key) const
{
	if (Map.Contains(Key))
	{
		return *Map.Find(Key);
	}
	else
	{
		FDataModel::FLiteral Literal;
		Literal.Type = TEXT("undefined");
		return Literal;
	}
}
