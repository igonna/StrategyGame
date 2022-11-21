// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_Controller.h"
#include "WarUnit.h"
#include "Building.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "CPP_RTS_Camera.h"

ARTS_Controller::ARTS_Controller()
{
	bReplicates = true;
	this->bShowMouseCursor = true;
}

void ARTS_Controller::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARTS_Controller, SpawnedActor);
}

void ARTS_Controller::CRPC_SetDestination_Implementation(AActor* WarUnit, FVector Destination, AActor* ClickedActor)
{
	if (WarUnit)
	{
		if (Cast<IIRTS_Interface>(WarUnit))
		{ 
			Cast<AWarUnit>(WarUnit)->INF_SetDestination(Destination, ClickedActor);
		}
	}
}

void ARTS_Controller::CRPC_SpawnUnitFromClass_Implementation(UClass* Class, FVector Location, FRotator Rotation)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnedActor = GetWorld()->SpawnActor(Class, &Location, &Rotation, SpawnInfo);
	if (Cast<IIRTS_Interface>(SpawnedActor))
	{
		if (Cast<ABuilding>(SpawnedActor))
		{
			Cast<ABuilding>(SpawnedActor)->INF_SetPlayerInfo(Cast<ARTS_PlayerState>(PlayerState)->GetPlayerId(), Cast<ARTS_PlayerState>(PlayerState)->PlayerColor);
		}
		if (Cast<AWarUnit>(SpawnedActor))
		{
			Cast<AWarUnit>(SpawnedActor)->INF_SetPlayerInfo(Cast<ARTS_PlayerState>(PlayerState)->GetPlayerId(), Cast<ARTS_PlayerState>(PlayerState)->PlayerColor);
		}
	}
}

void ARTS_Controller::CRPC_SetUnitInfo_Implementation(AActor* Unit, int32 PlayerId, FLinearColor Color)
{
	if (Unit)
	{
		if (Cast<IIRTS_Interface>(Unit))
		{
			if (Cast<AWarUnit>(Unit))
				Cast<AWarUnit>(Unit)->INF_SetPlayerInfo(PlayerId, Color);
			if (Cast<ABuilding>(Unit))
				Cast<ABuilding>(Unit)->INF_SetPlayerInfo(PlayerId, Color);
		}
	}
}
