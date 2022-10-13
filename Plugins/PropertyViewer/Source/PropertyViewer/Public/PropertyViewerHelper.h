// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "PropertyViewerSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PropertyViewerHelper.generated.h"

USTRUCT(BlueprintType)
struct FPropertyViewerReturnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EPropertyViewerDataType Type = EPropertyViewerDataType::Max;

	UPROPERTY(EditAnywhere)
	bool Val_Bool;
	
	UPROPERTY(EditAnywhere)
	int32 Val_Int;

	UPROPERTY(EditAnywhere)
	float Val_Float;

	UPROPERTY(EditAnywhere)
	FString Val_String;

	UPROPERTY(EditAnywhere)
	FVector Val_Vector;

	UPROPERTY(EditAnywhere)
	FRotator Val_Rotator;

	UPROPERTY(EditAnywhere)
	FLinearColor Val_Color;

	UPROPERTY(EditAnywhere)
	FString Val_Byte;

	UPROPERTY(EditAnywhere)
	FString Val_Struct;
};
/**
 * 
 */
UCLASS()
class PROPERTYVIEWER_API UPropertyViewerHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category = "PropertyViewer")
	static void GetTipsArray(FString InTokens,TMap<FString,FString>& OutNames,FString& OutPrevToken,FString& OutLastToken);

	UFUNCTION(BlueprintCallable, Category = "PropertyViewer")
	static bool CheckIsValidToken(FString InTokens,FString& RetType);

	//UFUNCTION(BlueprintCallable, Category = "PropertyViewer")
	static bool GetValueByTokens(AActor* InTarget,FString InTokens,FPropertyViewerReturnData& RetData);
	
	UFUNCTION(BlueprintCallable, Category = "PropertyViewer")
	static TArray<FPropertyViewerUIConfigData> GetUIConfig();

	UFUNCTION(BlueprintCallable, Category = "PropertyViewer")
	static void SetUIConfig(TArray<FPropertyViewerUIConfigData> UIConfig);

	UFUNCTION(BlueprintCallable, Category = "PropertyViewer")
	static TMap<FString,FPropertyViewerDrawConfigData> GetTokensList();

	UFUNCTION(BlueprintCallable, Category = "PropertyViewer")
	static void SetTokensList(TMap<FString,FPropertyViewerDrawConfigData> TokensList);

	UFUNCTION(BlueprintCallable, Category = "PropertyViewer")
	static UPropertyViewerSettings* GetDrawTokensConfigObject();
	
	UFUNCTION(BlueprintCallable, Category = "PropertyViewer")
	static void SaveDrawTokensConfig();
	
};
