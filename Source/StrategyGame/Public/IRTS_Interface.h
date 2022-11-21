// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IRTS_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIRTS_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STRATEGYGAME_API IIRTS_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement")
	void INF_SetDestination(FVector Destination, AActor* ClickedActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UnitInfo")
	void INF_SetPlayerInfo(int32 PlayerNum, FLinearColor UUnitColor);
};
