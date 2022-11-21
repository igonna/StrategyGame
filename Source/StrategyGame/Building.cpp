// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

ABuilding::ABuilding()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    RootComponent = BuildingMesh;
    SphereVision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereVision"));
    SphereVision->SetupAttachment(RootComponent);
    CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
    CollisionCapsule->SetupAttachment(RootComponent);

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
    HealthWidgetComponent->SetupAttachment(RootComponent);
    HealthWidgetComponent->SetVisibility(false);
}

void ABuilding::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ABuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABuilding, BuildingColor);
    DOREPLIFETIME(ABuilding, PlayerId);
    DOREPLIFETIME(ABuilding, health);
}


void ABuilding::CreateConstruction_Implementation(AActor* WorkerRef)
{

}

void ABuilding::DestroyConstruction_Implementation()
{

}

void ABuilding::INF_SetPlayerInfo_Implementation(int32 PlayerNum, FLinearColor UUnitColor)
{
    BuildingColor = UUnitColor;
    PlayerId = PlayerNum;
    CRPC_SetUnitInfoAll(PlayerId, BuildingColor);
}

void ABuilding::CRPC_SetUnitInfoAll_Implementation(int32 PlayerNum, FLinearColor Color)
{
    BuildingColor = Color;
    PlayerId = PlayerNum;
}

void ABuilding::OnClickedEvent_Implementation()
{

}