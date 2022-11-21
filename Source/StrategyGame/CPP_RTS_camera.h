// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Building.h"
#include "Templates/SubclassOf.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BlockingRTSCameraVolume.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CPP_RTS_camera.generated.h"

UCLASS()
class STRATEGYGAME_API ACPP_RTS_camera : public APawn
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = Camera, EditAnywhere)
	class UCapsuleComponent* CapsuleCollision;
	UPROPERTY(BlueprintReadWrite, Category = Camera, EditAnywhere)
	class UCameraComponent* Camera;
	UPROPERTY(BlueprintReadWrite, Category = Camera, EditAnywhere)
	class USpringArmComponent* SpringArm;

public:
	// Fog of war
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTextureRenderTarget2D* FogRenderTarget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* AdditiveFogBrush;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* UnrevealedEffect;

	// Hit / Selection
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FHitResult HitResult;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* HitActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> WarUnitsArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> WhileSelectionWarUnitsArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> WhileSelectionBuildingsArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* ActiveBuilding;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bSelection;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bSelectedWarUnit;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TickDeltaTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DistanceBetweenUnits;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bRotateUnits;
	//object components
	//USpringArmComponetnt spring_arm;

	// Sets default values for this pawn's properties
	ACPP_RTS_camera();
	//camera options
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float up_down_speed_c;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SpringArmLengthLow;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SpringArmLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float rotation_speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float default_arm_length_c;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float scroll_speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float const_scroll_speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float scroll_scale_percent_c;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector PreviousLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool ForbiddenPlace;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float ForbiddenAxisX;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float ForbiddenAxisY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpringArmAngle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector PrevLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float LastCameraMoveUpdate;

	
	


	// LMB RMB
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bDefaultControl;

	//LMB
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool LMB_click;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float LMB_click_pos_x;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float LMB_click_pos_y;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool LMB_click_held_c;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float LMB_total_angle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector LMB_world_location;
	

	//RMB
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RMB_total_angle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RMB_click_pos_x;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RMB_click_pos_y;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool RMB_click;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RMB_held_time;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bScrolling;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RMB_last_click_time;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RMB_break_timeout;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//bool RMB_click_held;

	//MMB
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool MMB_click_held;
	bool MMB_locked;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TotalRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MMB_held_time;


	//ESC
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool esc_pressed_c;

	//Viewport
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 viewport_size_x;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 viewport_size_y;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool menu_visible;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//UUserWidget* menu_widget;

	//mouse position
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float mouse_pos_x;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float mouse_pos_y;

	//build
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool build_mode;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ABuilding* SelectedBuilding;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bGoodBuildPosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bSpawnConstruction;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bRotateConstruction;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector MouseWorldPosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* ClickedActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DefaultBuildingRotation;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	static float CGetMouseAngle(float MouseClickPosX, float MouseClickPosY, float MousePosX, float MousePosY);
	
	UFUNCTION(BlueprintCallable)
	static float CGetRadiusVector(float MouseClickPosX, float MouseClickPosY, float MousePosX, float MousePosY);

	UFUNCTION(BlueprintCallable)
	void CUpdateMousePosition();

	UFUNCTION(BlueprintCallable)
	void CSetScrollingLocation(float coef);

	UFUNCTION(BlueprintCallable)
	void CZoom(float Difference);

	UFUNCTION(BlueprintCallable)
	void CBreakRotation();
	UFUNCTION(BlueprintCallable)
	AActor* GetNearestActor(TArray<AActor*> AActorArray, FVector Destination);

	/*UFUNCTION(BlueprintCallable)
	void CSpawnActorByClick();*/

	UFUNCTION(BlueprintCallable)
	float CGetKeyHeldTime(FName KeyName);
	//GetWorld()->GetFirstPlayerController()->GetInputKeyTimeDown(FKey("Spacebar"))

	UFUNCTION(BlueprintCallable)
	void BorderMove(float ScrollSpeed);
	UFUNCTION(BlueprintCallable)
	void CMoveMouseXY();

	UFUNCTION(BlueprintCallable)
	FVector CropLocationVector(FVector Location, float x, float y);
	UFUNCTION(BlueprintCallable)
	void GetForbiddenAxis();
	

	// Inputs
	void IA_ESC();

	void IA_PRE_LMB_click();
	void IA_REL_LMB_click();

	void IA_PRE_MMB_click();
	void IA_REL_MMB_click();

	void IA_PRE_RMB_click();
	void IA_REL_RMB_click();

	void IA_Move_X(float rate);
	void IA_Move_Y(float rate);
	void IA_Turn_X(float rate);




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void CancelBuiding();
	UFUNCTION(BlueprintCallable)
	void SetUnitWidgetsVisibility(bool Visible);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CameraMoved();

	UFUNCTION(BlueprintCallable)
	void SetFogRevealRadius(float radius);


	UFUNCTION(BlueprintCallable)
	float FindUnitRotationAngle(FVector Destination, TArray<AActor*> Units, int32 LeaderIndex);

	UFUNCTION(BlueprintCallable)
	void UnitsMovementSquare(FVector Destination, float _distanceBetweenUnits, bool _rotateUnits, UPARAM(ref) TArray<AActor*>& Units);

	UFUNCTION(BlueprintCallable)
	void RotateUnits(float Angle, int32 LeaderIndex, UPARAM(ref) TArray<FVector>& TargetDisplacements);
};
