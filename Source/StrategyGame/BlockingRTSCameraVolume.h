// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BlockingRTSCameraVolume.generated.h"

UCLASS()
class STRATEGYGAME_API ABlockingRTSCameraVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlockingRTSCameraVolume();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ForbiddenX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ForbiddenY;

protected:

public:	

};
