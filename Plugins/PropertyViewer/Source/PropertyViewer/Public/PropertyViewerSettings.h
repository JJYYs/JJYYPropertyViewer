// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PropertyViewerSettings.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPropertyViewerUIConfigData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Property;

	UPROPERTY(EditAnywhere)
	FString MapName;

	UPROPERTY(EditAnywhere)
	FColor Color;
};

UENUM()
enum class EPropertyViewerLogType : uint8
{
	StringLog,
	VectorLog,
	VisualLog,
};


UENUM()
enum class EPropertyViewerDataType : uint8
{
	Bool,
	Float,
	Int,
	String,
	Vector,
	Rotator,
	Enum,
	Struct,
	Max
};

USTRUCT(BlueprintType)
struct FPropertyViewerDrawConfigData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta=(editcondition = "false"))
	FString DataType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPropertyViewerLogType LogType = EPropertyViewerLogType::StringLog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Offset = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
	meta=(EditConditionHides, EditCondition ="LogType == EPropertyViewerLogType::StringLog"))
	bool PrintToLog = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta=(EditConditionHides, EditCondition = "LogType != EPropertyViewerLogType::StringLog"))
	bool NormalizeVector = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta=(EditConditionHides, EditCondition ="LogType != EPropertyViewerLogType::StringLog"))
	float Radius = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
	meta=(EditConditionHides, EditCondition ="LogType == EPropertyViewerLogType::VectorLog"))
	float Thickness = 2;
	
};

UCLASS(config=PropertyViewer)
class PROPERTYVIEWER_API UPropertyViewerSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	virtual FName GetCategoryName() const;

public:
	UPROPERTY(config, EditAnywhere, Category=DrawTokensConfig)
	TMap<FString, FPropertyViewerDrawConfigData> DrawTokensConfig;

	UPROPERTY(config, EditAnywhere, Category=UIConfig)
	TArray<FPropertyViewerUIConfigData> UIConfig;
};
