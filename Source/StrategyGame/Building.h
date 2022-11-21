// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTS_FogUnit.h"
#include "Net/UnrealNetwork.h"
#include "IRTS_Interface.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Building.generated.h"

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API ABuilding : public APawn, public IIRTS_Interface, public IRTS_FogUnit
{
	GENERATED_BODY()

public:
	
	// Components
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USphereComponent* SphereVision;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStaticMeshComponent* BuildingMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCapsuleComponent* CollisionCapsule;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UWidgetComponent* HealthWidgetComponent;
	// Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	FLinearColor BuildingColor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	int32 PlayerId;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	float health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player values")
	int32 Cost;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bForest;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bNotCreating;

	// Functions
	ABuilding();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CreateConstruction(AActor* WorkerRef);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DestroyConstruction();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnClickedEvent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UnitInfo")
	void INF_SetPlayerInfo(int32 PlayerNum, FLinearColor UUnitColor);
	void INF_SetPlayerInfo_Implementation(int32 PlayerNum, FLinearColor UUnitColor) override;
	UFUNCTION(NetMulticast, reliable, BlueprintCallable)
	void CRPC_SetUnitInfoAll(int32 PlayerNum, FLinearColor Color);
};
