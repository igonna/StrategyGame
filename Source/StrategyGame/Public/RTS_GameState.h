// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Net/UnrealNetwork.h"
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RTS_GameState.generated.h"

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API ARTS_GameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ARTS_GameState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	bool MatchReady;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	int32 MaxPlayers;
};
