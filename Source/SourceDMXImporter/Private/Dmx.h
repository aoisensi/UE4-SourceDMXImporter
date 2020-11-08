// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataModel.h"

class FDmx
{
public:
	FDmx(const FDataModel& Data);

	struct FDmeTransform
	{
		FString Name;
		FVector Position;
		FQuat Orientation;
	};

	struct FDmeTransformList
	{
		FString Name;
		TArray<TSharedPtr<FDmeTransform>> Transforms;
	};

	struct FDmeMaterial
	{
		FString Name;
		FString MtlName;
	};

	struct FDmeFaceSet
	{
		FString Name;
		TSharedPtr<FDmeMaterial> Material;
		TArray<int32> Faces;
	};

	struct FDmeVertexData
	{
		FString Name;
		TArray<FString> VertexFormat;
		int32 JointCount;
		bool FlipVCoordinates;
		TArray<FVector> Positions;
		TArray<int32> PositionsIndices;
		TArray<FVector> Normals;
		TArray<int32> NormalsIndices;
		TArray<FVector2D> TextureCoordinates;
		TArray<int32> TextureCoordinatesIndices;
		// Balance, BalanceIndices, JointWeights, JointIndices
	};

	enum class EDagType : uint8 { Dag, Mesh };

	struct FDmeMesh;
	struct FDmeDag
	{
		FString Name;
		EDagType Type;
		TSharedPtr<FDmeTransform> Transform;
		bool Visible;
		TArray<TSharedPtr<FDmeDag>> Children;
		TSharedPtr<FDmeMesh> ShapeMesh;
	};

	struct FDmeMesh : FDmeDag
	{
		// BindState
		TSharedPtr<FDmeVertexData> CurrentState;
		// BaseStates, DeltaStates
		TArray<TSharedPtr<FDmeFaceSet>> FaceSets;
		// DeltaStateWeights, DeltaStateWeightsLagged
	};


	struct FDmeModel
	{
		FString Name;
		TArray<TSharedPtr<FDmeDag>> Children;
		// JointList
		TArray<TSharedPtr<FDmeTransformList>> BaseStates;
	};

	TSharedPtr<FDmeModel> Model;

private:
	TMap<FGuid, TSharedPtr<FDmeTransform>> MapTransform;
	TMap<FGuid, TSharedPtr<FDmeTransformList>> MapTransformList;
	TMap<FGuid, TSharedPtr<FDmeMaterial>> MapMaterial;
	TMap<FGuid, TSharedPtr<FDmeFaceSet>> MapFaceSet;
	TMap<FGuid, TSharedPtr<FDmeVertexData>> MapVertexData;
	TMap<FGuid, TSharedPtr<FDmeDag>> MapDag;
	TMap<FGuid, TSharedPtr<FDmeMesh>> MapMesh;
	TMap<FGuid, TSharedPtr<FDmeModel>> MapModel;

	TSharedPtr<FDmeTransform> ReadTransform(const FDataModel::FElement& Element);
	TSharedPtr<FDmeTransformList> ReadTransformList(const FDataModel::FElement& Element, const FDataModel& Data);
	TSharedPtr<FDmeMaterial> ReadMaterial(const FDataModel::FElement& Element);
	TSharedPtr<FDmeFaceSet> ReadFaceSet(const FDataModel::FElement& Element, const FDataModel& Data);
	TSharedPtr<FDmeVertexData> ReadVertexData(const FDataModel::FElement& Element);
	TSharedPtr<FDmeDag> ReadDag(const FDataModel::FElement& Element, const FDataModel& Data);
	TSharedPtr<FDmeMesh> ReadMesh(const FDataModel::FElement& Element, const FDataModel& Data);
	TSharedPtr<FDmeModel> ReadModel(const FDataModel::FElement& Element, const FDataModel& Data);
};