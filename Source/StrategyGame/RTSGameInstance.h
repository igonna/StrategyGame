// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RTSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API URTSGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FLinearColor PlayerColor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString PlayerName;
};
