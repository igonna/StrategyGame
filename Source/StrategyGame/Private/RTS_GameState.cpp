// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_GameState.h"

ARTS_GameState::ARTS_GameState()
{
	bReplicates = true;
}

void ARTS_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARTS_GameState, MatchReady);
	DOREPLIFETIME(ARTS_GameState, MaxPlayers);
}