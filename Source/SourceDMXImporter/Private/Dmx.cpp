// Fill out your copyright notice in the Description page of Project Settings.

#include "Dmx.h"
#include "DataModel.h"

FDmx::FDmx(const FDataModel& Data)
{
	auto Root = Data.Elements[Data.RootId.GetValue()];
	auto DataModel = Data.Elements[Root.Find(TEXT("model")).Id.GetValue()];
	Model = ReadModel(DataModel, Data);
}

TSharedPtr<FDmx::FDmeTransform> FDmx::ReadTransform(const FDataModel::FElement& Element)
{
	if (Element.Type != TEXT("DmeTransform")) return nullptr;
	if (MapTransform.Contains(Element.Id))
	{
		return MapTransform[Element.Id];
	}
	TSharedPtr<FDmx::FDmeTransform> Out(new FDmx::FDmeTransform);
	if (!Element.Find(TEXT("name")).GetString(Out->Name)) return nullptr;
	if (!Element.Find(TEXT("position")).GetVector(Out->Position)) return nullptr;
	if (!Element.Find(TEXT("orientation")).GetQuat(Out->Orientation)) return nullptr;
	MapTransform.Add(Element.Id, Out);
	return Out;
}

TSharedPtr<FDmx::FDmeTransformList> FDmx::ReadTransformList(const FDataModel::FElement& Element, const FDataModel& Data)
{
	if (Element.Type != TEXT("DmeTransformList")) return nullptr;
	if (MapTransformList.Contains(Element.Id))
	{
		return MapTransformList[Element.Id];
	}
	TSharedPtr<FDmx::FDmeTransformList> Out(new FDmx::FDmeTransformList);
	if (!Element.Find(TEXT("name")).GetString(Out->Name)) return nullptr;
	TArray<FGuid> Transforms;
	if (!Element.Find(TEXT("transforms")).GetElementArray(Transforms)) return nullptr;
	auto Size = Transforms.Num();
	Out->Transforms.SetNum(Size);
	for (int32 I = 0; I < Size; I++)
	{
		auto Transform = Data.Elements.Find(Transforms[I]);
		auto Ptr = ReadTransform(*Transform);
		Out->Transforms[I] = Ptr;
	}
	MapTransformList.Add(Element.Id, Out);
	return Out;
}

TSharedPtr<FDmx::FDmeMaterial> FDmx::ReadMaterial(const FDataModel::FElement& Element)
{
	if (Element.Type != TEXT("DmeMaterial")) return nullptr;
	if (MapMaterial.Contains(Element.Id))
	{
		return MapMaterial[Element.Id];
	}
	TSharedPtr<FDmx::FDmeMaterial> Out(new FDmx::FDmeMaterial);
	if (!Element.Find(TEXT("name")).GetString(Out->Name)) return nullptr;
	if (!Element.Find(TEXT("mtlname")).GetString(Out->MtlName)) return nullptr;
	MapMaterial.Add(Element.Id, Out);
	return Out;
}

TSharedPtr<FDmx::FDmeFaceSet> FDmx::ReadFaceSet(const FDataModel::FElement& Element, const FDataModel& Data)
{
	if (Element.Type != TEXT("DmeFaceSet")) return nullptr;
	if (MapFaceSet.Contains(Element.Id))
	{
		return MapFaceSet[Element.Id];
	}
	TSharedPtr<FDmx::FDmeFaceSet> Out(new FDmx::FDmeFaceSet);
	if (!Element.Find(TEXT("name")).GetString(Out->Name)) return nullptr;
	auto MaterialId = Element.Find(TEXT("material")).Id;
	if (MaterialId.IsSet())
	{
		Out->Material = ReadMaterial(*Data.Elements.Find(MaterialId.GetValue()));
	}
	if (!Element.Find(TEXT("faces")).GetIntArray(Out->Faces)) return nullptr;
	MapFaceSet.Add(Element.Id, Out);
	return Out;
}

TSharedPtr<FDmx::FDmeVertexData> FDmx::ReadVertexData(const FDataModel::FElement& Element)
{
	if (Element.Type != TEXT("DmeVertexData")) return nullptr;
	if (MapVertexData.Contains(Element.Id))
	{
		return MapVertexData[Element.Id];
	}
	TSharedPtr <FDmx::FDmeVertexData> Out(new FDmx::FDmeVertexData);
	if (!Element.Find(TEXT("name")).GetString(Out->Name)) return nullptr;
	if (!Element.Find(TEXT("vertexFormat")).GetStringArray(Out->VertexFormat)) return nullptr;
	if (!Element.Find(TEXT("jointCount")).GetInt(Out->JointCount)) return nullptr;
	if (!Element.Find(TEXT("flipVCoordinates")).GetBool(Out->FlipVCoordinates)) return nullptr;
	if (!Element.Find(TEXT("positions")).GetVectorArray(Out->Positions)) return nullptr;
	if (!Element.Find(TEXT("positionsIndices")).GetIntArray(Out->PositionsIndices)) return nullptr;
	if (!Element.Find(TEXT("normals")).GetVectorArray(Out->Normals)) return nullptr;
	if (!Element.Find(TEXT("normalsIndices")).GetIntArray(Out->NormalsIndices)) return nullptr;
	if (!Element.Find(TEXT("textureCoordinates")).GetVector2DArray(Out->TextureCoordinates)) return nullptr;
	if (!Element.Find(TEXT("textureCoordinatesIndices")).GetIntArray(Out->TextureCoordinatesIndices)) return nullptr;
	MapVertexData.Add(Element.Id, Out);
	return Out;
}

TSharedPtr<FDmx::FDmeDag> FDmx::ReadDag(const FDataModel::FElement& Element, const FDataModel& Data)
{
	if (Element.Type != TEXT("DmeDag"))
	{
		if (Element.Type == TEXT("DmeMesh"))
		{
			return (TSharedPtr<FDmx::FDmeDag>)ReadMesh(Element, Data);
		}
		return nullptr;
	}
	if (MapDag.Contains(Element.Id))
	{
		return MapDag[Element.Id];
	}
	TSharedPtr <FDmx::FDmeDag> Out(new FDmx::FDmeDag);
	if (!Element.Find(TEXT("name")).GetString(Out->Name)) return nullptr;

	auto TransformId = Element.Find(TEXT("transform")).Id;
	if (TransformId.IsSet())
	{
		Out->Transform = ReadTransform(*Data.Elements.Find(TransformId.GetValue()));
	}

	Out->Visible = true; //default
	Element.Find(TEXT("visible")).GetBool(Out->Visible);

	auto ShapeId = Element.Find(TEXT("shape")).Id;
	if (ShapeId.IsSet())
	{
		Out->ShapeMesh = ReadMesh(*Data.Elements.Find(ShapeId.GetValue()), Data);
	}

	MapDag.Add(Element.Id, Out);
	return Out;
}

TSharedPtr<FDmx::FDmeMesh> FDmx::ReadMesh(const FDataModel::FElement& Element, const FDataModel& Data)
{
	if (Element.Type != TEXT("DmeMesh")) return nullptr;
	if (MapMesh.Contains(Element.Id))
	{
		return MapMesh[Element.Id];
	}
	TSharedPtr<FDmx::FDmeMesh> Out(new FDmx::FDmeMesh);
	if (!Element.Find(TEXT("name")).GetString(Out->Name)) return nullptr;
	if (!Element.Find(TEXT("visible")).GetBool(Out->Visible)) return nullptr;

	auto CurrentStateId = Element.Find(TEXT("currentState")).Id;
	if (CurrentStateId.IsSet())
	{
		Out->CurrentState = ReadVertexData(*Data.Elements.Find(CurrentStateId.GetValue()));
	}

	TArray<FGuid> FaceSets;
	if (!Element.Find(TEXT("faceSets")).GetElementArray(FaceSets)) return nullptr;
	auto FaceSetsSize = FaceSets.Num();
	Out->FaceSets.SetNum(FaceSetsSize);
	for (int32 I = 0; I < FaceSetsSize; I++)
	{
		auto FaceSet = Data.Elements.Find(FaceSets[I]);
		auto Ptr = ReadFaceSet(*FaceSet, Data);
		Out->FaceSets[I] = Ptr;
	}

	MapMesh.Add(Element.Id, Out);
	return Out;
}

TSharedPtr<FDmx::FDmeModel> FDmx::ReadModel(const FDataModel::FElement& Element, const FDataModel& Data)
{
	if (Element.Type != TEXT("DmeModel")) return nullptr;
	if (MapModel.Contains(Element.Id))
	{
		return MapModel[Element.Id];
	}

	TSharedPtr<FDmx::FDmeModel> Out(new FDmx::FDmeModel);
	if (!Element.Find(TEXT("name")).GetString(Out->Name)) return nullptr;

	TArray<FGuid> Children;
	if (!Element.Find(TEXT("children")).GetElementArray(Children)) return nullptr;
	auto ChildrenSize = Children.Num();
	Out->Children.SetNum(ChildrenSize);
	for (int32 I = 0; I < ChildrenSize; I++)
	{
		auto Child = Data.Elements.Find(Children[I]);
		auto Ptr = ReadDag(*Child, Data);
		Out->Children[I] = Ptr;
	}

	TArray<FGuid> BaseStates;
	if (!Element.Find(TEXT("baseStates")).GetElementArray(BaseStates)) return nullptr;
	auto BaseStatesSize = BaseStates.Num();
	Out->BaseStates.SetNum(BaseStatesSize);
	for (int32 I = 0; I < BaseStatesSize; I++)
	{
		auto BaseState = Data.Elements.Find(BaseStates[I]);
		auto Ptr = ReadTransformList(*BaseState, Data);
		Out->BaseStates[I] = Ptr;
	}

	MapModel.Add(Element.Id, Out);
	return Out;
}
