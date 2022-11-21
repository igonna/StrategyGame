// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RTS_FogUnit.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URTS_FogUnit : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STRATEGYGAME_API IRTS_FogUnit
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UnitFogOfWar")
	float INF_GetFogRevealRadius();
};
