// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CPP_RTS_camera.h"
#include "Engine/Texture2D.h"
#include "Math/Vector2D.h"
#include "Math/Color.h"
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RTS_HUD.generated.h"

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API ARTS_HUD : public AHUD
{
	GENERATED_BODY()
public:
	//Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float UIBorderWidth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MinimapCoordXAndUi;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MinimapCoordY;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MinimapW;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MinimapH;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MinimapCoordX;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FLinearColor SelectionColor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FLinearColor SelectionColorTransparent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SelectionBorderWidth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MinimapSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int ViewportSizeX;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int ViewportSizeY;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTexture2D* MinimapTexture;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FLinearColor UIBorderColor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector2D MinimapCoordTL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector2D MinimapCoordRB;



	//Functions
	virtual void BeginPlay() override;
	virtual void ReceiveDrawHUD(int32 SizeX, int32 SizeY);
	virtual void DrawHUD() override;
	UFUNCTION(BlueprintCallable)
	void DrawSelection(float screen_x, float screen_y, float screen_w, float screen_h);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SelectionEvent();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BreakSelectionEvent();
	UFUNCTION(BlueprintCallable)
	void DrawMinimapPoint(float minimap_coord_x, float minimap_coord_y, float minimap_w, float minimap_h, FLinearColor PointColor, FVector2D MapCoordTL, FVector2D MapCoordRB, FVector ObjectLocation, float Size);
	UFUNCTION(BlueprintPure, Category = HUD)
	void CGetActorsInSelectionRectangle(TSubclassOf<AActor> ClassFilter, const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<AActor*>& OutActors, bool bIncludeNonCollidingComponents = true, bool bActorMustBeFullyEnclosed = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DrawMinimapFogOfWar();
};
