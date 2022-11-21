// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IRTS_Interface.h"
#include "RTS_FogUnit.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "WarUnit.generated.h"
//#include "Components/SphereComponent.h"

UCLASS()
class STRATEGYGAME_API AWarUnit : public ACharacter, public IIRTS_Interface, public IRTS_FogUnit
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWarUnit();
	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/** The player's current health. When reduced to 0, they are considered dead.*/
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//class USphereComponent* SphereCollision;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USphereComponent* SphereVision;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AttackDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	float health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool GoingAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* PressureUnit;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Player values")
	int32 Cost;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AttackTimeOut;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float LastAttackTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	bool Attacing;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector2D ScreenPosition;
	/** RepNotify for changes made to current health.*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	FLinearColor UnitColor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	int32 PlayerId;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnClickedEvent();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetDestination(FVector Destination, AActor* ClickedActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Attack(AActor* ActorForAttack);

	//IRTS_Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Movement")
	void INF_SetDestination(FVector Destination, AActor* ClickedActor);  // This is the prototype declared in the interface
	virtual void INF_SetDestination_Implementation(FVector Destination, AActor* ClickedActor) override; // This is the declaration of the implementation

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UnitInfo")
	void INF_SetPlayerInfo(int32 PlayerNum, FLinearColor UUnitColor);
	void INF_SetPlayerInfo_Implementation(int32 PlayerNum, FLinearColor UUnitColor) override;
	UFUNCTION(NetMulticast, reliable, BlueprintCallable)
	void CRPC_SetUnitInfoAll(int32 PlayerNum, FLinearColor Color);

	// IRTS_FofUnit
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UnitFogOfWar")
	float INF_GetFogRevealRadius();

	// Optimization
	//UFUNCTION(BlueprintCallable)
	bool GetActorPosInScreenSpace(APlayerController& Player, FVector2D& OutScreenPos);
	UFUNCTION(BlueprintCallable)
	FVector2D GetScreenLocation();
};
