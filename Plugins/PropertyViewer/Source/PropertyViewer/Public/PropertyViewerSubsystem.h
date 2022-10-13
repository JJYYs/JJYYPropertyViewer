// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyViewerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROPERTYVIEWER_API UPropertyViewerSubsystem : public UTickableWorldSubsystem
{

private:
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickableInEditor() const override;
	virtual TStatId GetStatId() const override;
};
