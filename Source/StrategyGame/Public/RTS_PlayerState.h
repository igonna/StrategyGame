// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net/UnrealNetwork.h"
#include "Math/Color.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RTS_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API ARTS_PlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ARTS_PlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	FLinearColor PlayerColor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	FString PlayerName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	float Capital;
	UFUNCTION(BlueprintCallable)
	void GetMoney(float Money);
	UFUNCTION(BlueprintCallable)
	bool SpentMoney(float Money);
	UFUNCTION(BlueprintCallable)
	bool CanBuy(float Money);
};
