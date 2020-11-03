// Fill out your copyright notice in the Description page of Project Settings.


#include "DmxImporter.h"
#include "Misc/FileHelper.h"
#include "DataModelKV2.h"

bool FDmxImporter::OpenFile(const FString& Filename)
{
	if (!ReadHeader(Filename))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed parse header. Filename: %s"), *Filename);
		return false;
	}
	FDataModel Data;
	if (EncodingType == TEXT("keyvalues2") && EncodingVer == 1)
	{
		TUniquePtr<FArchive> Reader(IFileManager::Get().CreateFileReader(*Filename, 0));
		if (!Reader)
		{
			return false;
		}
		FString Source;
		if (!FFileHelper::LoadFileToString(Source, *Reader))
		{
			return false;
		}
		Data = FDataModelKV2(Source, HeaderSize);
	}
	// Break Point Here And Watch Data Variable!!
	return false;
}

bool FDmxImporter::ReadHeader(const FString& Filename)
{
	TUniquePtr<FArchive> Reader(IFileManager::Get().CreateFileReader(*Filename, 0));
	if (!Reader)
	{
		return false;
	}
	uint8* Ch = (uint8*)FMemory::Malloc(MaxHeaderSize);
	Reader->Serialize(Ch, MaxHeaderSize);
	for (int32 I = 0; I < MaxHeaderSize; I++)
	{
		if (Ch[I] == '\n' || Ch[I] == '\r')
		{
			HeaderSize = I;
			break;
		}
	}
	if (HeaderSize == 0)
	{
		FMemory::Free(Ch);
		return false;
	}
	FString Text;
	FFileHelper::BufferToString(Text, Ch, HeaderSize);
	FMemory::Free(Ch);
	TArray<FString> Splited;
	Text.ParseIntoArray(Splited, TEXT(" "));
	if (Splited.Num() != 9) return false;
	if (Splited[0] != TEXT("<!--")) return false;
	if (Splited[1] != TEXT("dmx")) return false;
	if (Splited[2] != TEXT("encoding")) return false;
	if (Splited[5] != TEXT("format")) return false;
	if (Splited[8] != TEXT("-->")) return false;
	EncodingType = FName(*Splited[3]);
	EncodingVer = FCString::Atoi(*Splited[4]);
	if (EncodingVer == 0) return false;
	FormatType = FName(*Splited[6]);
	FormatVer = FCString::Atoi(*Splited[7]);
	if (FormatVer == 0) return false;
	return true;
};