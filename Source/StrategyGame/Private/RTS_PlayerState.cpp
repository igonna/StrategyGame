// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_PlayerState.h"

ARTS_PlayerState::ARTS_PlayerState()
{
	bReplicates = true;
}

void ARTS_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARTS_PlayerState, PlayerColor);
	DOREPLIFETIME(ARTS_PlayerState, PlayerName);
	DOREPLIFETIME(ARTS_PlayerState, Capital);
}

void ARTS_PlayerState::GetMoney(float Money)
{
	Capital += Money;
}

bool ARTS_PlayerState::SpentMoney(float Money)
{
	if (Capital - Money >= 0)
	{
		Capital -= Money;
		return true;
	}
	return false;
}

bool ARTS_PlayerState::CanBuy(float Money)
{
	return (Capital - Money >= 0);
}