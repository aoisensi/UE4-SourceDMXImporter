// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataModel.h"

class FDataModelKV2 : public FDataModel
{
public:
	FDataModelKV2(const FString& InSource, const int32 HeaderSize);

private:
	FString Source;
	int32 Size;
	int32 Ptr;
	bool FlagEOF;

	bool ReadElement(FElement& Element);
	bool ReadElement(FElement& Element, const FString& Type);
	bool ReadString(FString& String);

	static bool IsElement(FString& Type);

	bool Maybe(TCHAR Char);
	bool Should(TCHAR Char);
	bool Skip();
	bool Check();
	void Back();
	TCHAR Next();
	TCHAR Get();
};