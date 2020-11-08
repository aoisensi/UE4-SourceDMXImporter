// Fill out your copyright notice in the Description page of Project Settings.

#include "DataModelKV2.h"

FDataModelKV2::FDataModelKV2(const FString& InSource, const int32 HeaderSize)
{
	Source = InSource;
	Size = InSource.Len();
	Ptr = HeaderSize + 1;
	if (!Skip())
	{
		FailedParse = true;
		return;
	}
	while (true)
	{
		FElement Element;
		if (!ReadElement(Element))
		{
			FailedParse = true;
			return;
		}
		if (!RootId.IsSet())
		{
			RootId = Element.Id;
		}
		if (!Skip()) break; //success
	}
}

bool FDataModelKV2::ReadElement(FElement& Element)
{
	FString Type;
	if (!ReadString(Type)) return false;
	return ReadElement(Element, Type);
}

bool FDataModelKV2::ReadElement(FElement& Element, const FString& Type)
{
	Element.Type = Type;
	if (!Should(TEXT('{'))) return false;
	if (!Skip()) return false;
	while (true)
	{
		if (Maybe(TEXT('}'))) break; //done

		FString Key;
		if (!ReadString(Key)) return false;
		if (!Skip()) return false;

		FLiteral Literal;
		if (!ReadString(Literal.Type)) return false;
		if (!Skip()) return false;
		if (IsElement(Literal.Type))
		{
			FElement ValueElement;
			if (!ReadElement(ValueElement, Literal.Type)) return false;
			if (!Skip()) return false;
			Literal.Id = ValueElement.Id;
			Literal.Type = TEXT("element");
		}
		else
		{
			if (Literal.Type == TEXT("element_array"))
			{
				if (!Should(TEXT('['))) return false;
				if (!Skip()) return false;
				if (!Maybe(']')) {
					if (!Skip()) return false;
					while (true)
					{
						FLiteral Item;
						if (!ReadString(Item.Type)) return false;
						if (!Skip()) return false;
						if (Item.Type == "element")
						{
							if (!ReadString(Item.Value)) return false;
							if (!Skip()) return false;
							FGuid Id;
							FGuid::Parse(Item.Value, Id);
							Item.Id = Id;
						}
						else
						{
							FElement ItemElement;
							if (!ReadElement(ItemElement, Item.Type)) return false;
							if (!Skip()) return false;
							Item.Type = TEXT("element");
							Item.Id = ItemElement.Id;
						}
						Literal.IdArray.Add(Item.Id.GetValue());
						if (Maybe(TEXT(',')))
						{
							if (!Skip()) return false;
							continue;
						}
						else if (Maybe(TEXT(']')))
						{
							if (!Skip()) return false;
							break;
						}
						else return false;
					}
				}
				if (!Skip()) return false;
			}
			else if (Literal.Type.EndsWith(TEXT("_array")))
			{
				if (!Should(TEXT('['))) return false;
				if (!Skip()) return false;
				if (!Maybe(TEXT(']'))) {
					if (!Skip()) return false;
					while (true)
					{
						FString Value;
						if (!ReadString(Value)) return false;
						if (!Skip()) return false;
						Literal.Array.Add(Value);
						if (Maybe(TEXT(',')))
						{
							if (!Skip()) return false;
							continue;
						}
						else if (Maybe(TEXT(']')))
						{
							if (!Skip()) return false;
							break;
						}
						else return false;
					}
				}
				if (!Skip()) return false;
			}
			else
			{
				if (!ReadString(Literal.Value)) return false;
				if (!Skip()) return false;
				if (Literal.Type == TEXT("element") || Literal.Type == TEXT("elementid"))
				{
					if (Literal.Value != TEXT(""))
					{
						FGuid Id;
						if (!FGuid::Parse(Literal.Value, Id)) return false;
						Literal.Id = Id;
					}
				}
			}
		}
		Element.Map.Add(Key, Literal);
	}
	auto IdLiteral = Element.Map.Find(TEXT("id"));
	if (IdLiteral->Type != TEXT("elementid")) return false;
	if (!FGuid::Parse(IdLiteral->Value, Element.Id)) return false;
	Elements.Add(Element.Id, Element); // add
	return true;
}

bool FDataModelKV2::ReadString(FString& String)
{
	if (!Should(TEXT('"'))) return false;
	auto Start = Ptr;
	for (auto I = 0;; I++)
	{
		auto Char = Next();
		if (!Check()) return false;
		if (Char != TEXT('"')) continue;
		String = Source.Mid(Start, I);
		return Skip();
	}
}

bool FDataModelKV2::IsElement(FString& Type)
{
	return !Type.Equals(Type.ToLower(), ESearchCase::CaseSensitive);
}

bool FDataModelKV2::Maybe(TCHAR Char)
{
	if (Should(Char)) return true;
	Back();
	return false;
}

bool FDataModelKV2::Should(TCHAR Char)
{
	return Char == Next();
}

bool FDataModelKV2::Skip()
{
	while (true)
	{
		auto Char = Next();
		if (!Check()) {
			FlagEOF = true;
			return false;
		}
		if (Char == TEXT(' ') || Char == TEXT('\n') || Char == TEXT('\t') || Char == TEXT('\r')) continue;
		Back();
		return true;
	}
}

bool FDataModelKV2::Check()
{
	return Size > Ptr;
}

void FDataModelKV2::Back()
{
	Ptr--;
}

TCHAR FDataModelKV2::Next()
{
	return Source[Ptr++];
}

TCHAR FDataModelKV2::Get()
{
	return Source[Ptr];
}
