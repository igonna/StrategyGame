// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTS_PlayerState.h"
#include "UObject/Class.h"
#include "IRTS_Interface.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTS_Controller.generated.h"

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API ARTS_Controller : public APlayerController, public IIRTS_Interface
{
	GENERATED_BODY()
public:
	ARTS_Controller();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(Server, reliable, BlueprintCallable)
	void CRPC_SetDestination(AActor* WarUnit, FVector Destination, AActor* ClickedActor);
	UFUNCTION(Server, reliable, BlueprintCallable)
	void CRPC_SpawnUnitFromClass(UClass* Class, FVector Location, FRotator Rotation);

	UFUNCTION(Server, reliable, BlueprintCallable)
	void CRPC_SetUnitInfo(AActor* Unit, int32 PlayerId, FLinearColor Color);

	//UFUNCTION(Server, reliable, BlueprintCallable)
	//void CRPC_CreateBaseBuildingForest();

	//UFUNCTION(Server, reliable, BlueprintCallable)
	//void CRPC_SetUnitInfo(AActor* Unit, int32 PlayerId, FLinearColor Color);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	AActor* SpawnedActor;
protected:

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement")
	//void INF_MoveToLocation(FVector Destination);
};
