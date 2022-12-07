// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_RTS_camera.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "GameFramework/PlayerController.h"
#include "RTS_Controller.h"
#include "Building.h"
#include "WarUnit.h"
#include "RTS_HUD.h"
//#include "Landscape.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Sets default values
ACPP_RTS_camera::ACPP_RTS_camera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initializaion
	mouse_pos_x = 1.0f;
	mouse_pos_y = 1.0f;
	SpringArmAngle = -45.0f;
	SpringArmLength = 3800.0f;
	bDefaultControl = false;
	SpringArmLengthLow = 2000.0f;
	rotation_speed = 5.0f;
	scroll_speed = 150.0f;
	const_scroll_speed = scroll_speed;
	DefaultBuildingRotation = 35.0f;
	bScrolling = false;
	bSelectedWarUnit = false;
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	RootComponent = CapsuleCollision;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = SpringArmLength;
	SpringArm->bDoCollisionTest = false;

	// Camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	
}

// Called when the game starts or when spawned
void ACPP_RTS_camera::BeginPlay()
{
	Super::BeginPlay();
	SpringArm->SetRelativeRotation(FRotator(SpringArmAngle, 0.0f, 0.0f));
}

// Called every frame
void ACPP_RTS_camera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetGameTimeSinceCreation() - LastCameraMoveUpdate > 0.2f)
	{
		if (PrevLocation != GetActorLocation())
		{
			CameraMoved();
			PrevLocation = GetActorLocation();
			LastCameraMoveUpdate = GetGameTimeSinceCreation();
		}
	}

	if (!(ForbiddenAxisX * ForbiddenAxisY))
		GetForbiddenAxis();
	if (GetController())
	{
		TickDeltaTime = DeltaTime;
		Cast<ARTS_Controller>(GetController())->GetViewportSize(viewport_size_x, viewport_size_y);
		Cast<ARTS_Controller>(GetController())->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, HitResult);
		if (HitResult.Location.Size())
			MouseWorldPosition = HitResult.Location;
		ClickedActor = HitResult.GetActor();
		if (build_mode)
		{
			if (HitResult.Location != FVector(0.0f, 0.0f, 0.0f))
			{
				if (HitResult.GetActor()->GetClass()->GetName() == FString("Landscape"))
					MouseWorldPosition = HitResult.Location;
				else
				{
					MouseWorldPosition = FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.GetActor()->GetActorLocation().Z);
				}
			}
		}
		if (RMB_click)
		{
			RMB_held_time = CGetKeyHeldTime(FName("RightMouseButton"));
			if (bScrolling)
			{
				Cast<ARTS_Controller>(GetController())->bShowMouseCursor = false;
				RMB_total_angle = CGetMouseAngle(RMB_click_pos_x, RMB_click_pos_y, mouse_pos_x, mouse_pos_y);
				CSetScrollingLocation(DeltaTime);
			}
		}
		if (LMB_click)
		{
			LMB_total_angle = CGetMouseAngle(LMB_click_pos_x, LMB_click_pos_y, mouse_pos_x, mouse_pos_y);
		}
		if (MMB_click_held)
		{
			MMB_held_time = CGetKeyHeldTime(FName("MiddleMouseButton"));
		}
		if (build_mode)
		{
			// Getting vector
			if (bSpawnConstruction)
				SelectedBuilding->SetActorLocation(MouseWorldPosition);
			if (bRotateConstruction)
				if (!((LMB_click_pos_x == mouse_pos_x) && (LMB_click_pos_y == mouse_pos_y)))
					SelectedBuilding->SetActorRotation(FRotator(0.0f, TotalRotation + (-1.0f * LMB_total_angle - 90.0f), 0.0f));
		}
	}
}

// Called to bind functionality to input
void ACPP_RTS_camera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(InputComponent);
	
	InputComponent->BindAction("IA_ESC", IE_Pressed, this, &ACPP_RTS_camera::IA_ESC);

	InputComponent->BindAction("IA_LMB_click", IE_Pressed, this, &ACPP_RTS_camera::IA_PRE_LMB_click);
	InputComponent->BindAction("IA_LMB_click", IE_Released, this, &ACPP_RTS_camera::IA_REL_LMB_click);
	
	InputComponent->BindAction("IA_MMB_click", IE_Pressed, this, &ACPP_RTS_camera::IA_PRE_MMB_click);
	InputComponent->BindAction("IA_MMB_click", IE_Released, this, &ACPP_RTS_camera::IA_REL_MMB_click);

	InputComponent->BindAction("IA_RMB_click", IE_Pressed, this, &ACPP_RTS_camera::IA_PRE_RMB_click);
	InputComponent->BindAction("IA_RMB_click", IE_Released, this, &ACPP_RTS_camera::IA_REL_RMB_click);


	InputComponent->BindAxis("IA_Move_X", this, &ACPP_RTS_camera::IA_Move_X);
	InputComponent->BindAxis("IA_Move_Y", this, &ACPP_RTS_camera::IA_Move_Y);
	InputComponent->BindAxis("IA_Turn_X", this, &ACPP_RTS_camera::IA_Turn_X);

}

//zoom in and out
void ACPP_RTS_camera::CZoom(float Difference)
{
	if (Difference > 0.0f)
	{
		if (SpringArm->TargetArmLength + Difference >= SpringArmLength)
		{
			SpringArm->TargetArmLength = SpringArmLength;
		}
		else
		{
			SpringArm->TargetArmLength = Difference + SpringArm->TargetArmLength;
		}
	}
	else
	{
		if (SpringArm->TargetArmLength + Difference <= SpringArmLengthLow)
		{
			SpringArm->TargetArmLength = SpringArmLengthLow;
		}
		else
		{
			SpringArm->TargetArmLength = Difference + SpringArm->TargetArmLength;
		}
	}
}

void ACPP_RTS_camera::CBreakRotation()
{
	AddActorWorldRotation(FRotator(0.0f, -1.0f * TotalRotation, 0.0f));
	TotalRotation = 0.0f;
	scroll_speed = const_scroll_speed;

	// Set spring arm length
}

float ACPP_RTS_camera::CGetMouseAngle(float MouseClickPosX, float MouseClickPosY, float MousePosX, float MousePosY)
{
	float MousePosXDiff = MousePosX - MouseClickPosX;

	float MousePosYDiff = MousePosY - MouseClickPosY;

	float AngleSin = UKismetMathLibrary::Asin(UKismetMathLibrary::Abs(MousePosY - MouseClickPosY) / UKismetMathLibrary::Sqrt((MousePosXDiff * MousePosXDiff) + (MousePosYDiff * MousePosYDiff))) * 57.2957f;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CSinus: %f"), AngleSin));

	if (MousePosYDiff > 0.0f)
	{
		if (MousePosXDiff > 0.0f)
		{
			return 90.0f - AngleSin + 270.0f;
		}
		else
		{
			return AngleSin + 180.0f;
		}
	}
	else
	{
		if (MousePosXDiff < 0.0f)
		{
			return 90.0f - AngleSin + 90.0f;
		}
		else
		{
			return AngleSin;
		}
	}
}

float ACPP_RTS_camera::CGetRadiusVector(float MouseClickPosX, float MouseClickPosY, float MousePosX, float MousePosY)
{
	float XRoot = MousePosX - MouseClickPosX;
	XRoot *= XRoot;

	float YRoot = MousePosY - MouseClickPosY;
	YRoot *= YRoot;

	return UKismetMathLibrary::Sqrt(XRoot + YRoot);
}

void ACPP_RTS_camera::CUpdateMousePosition()
{
	Cast<ARTS_Controller>(GetController())->GetMousePosition(mouse_pos_x, mouse_pos_y);
}

void ACPP_RTS_camera::CSetScrollingLocation(float coef)
{
	float RotAngle = -360;
	if (TotalRotation > 0.0f)
		RotAngle += TotalRotation;
	else
		RotAngle = TotalRotation;



	FVector NewLocation = GetActorLocation() + FVector(0.0f, CGetRadiusVector(RMB_click_pos_x, RMB_click_pos_y, mouse_pos_x, mouse_pos_y) * (scroll_speed / 100.0f) * (coef / 0.016f), 0.0f).RotateAngleAxis((RMB_total_angle + UKismetMathLibrary::Abs(RotAngle)) * (-1.0f), FVector(0.0f, 0.0f, 1.0f));
	
	// check forbidden location
	NewLocation = CropLocationVector(NewLocation, ForbiddenAxisX, ForbiddenAxisY);
	SetActorLocation(NewLocation);

	FHitResult LineTraceHit;
	FVector Start = CapsuleCollision->GetComponentToWorld().GetLocation();
	Start = FVector(Start.X, Start.Y, Start.Z + 1000.0f);
	FVector End = CapsuleCollision->GetComponentToWorld().GetLocation();
	End = FVector(End.X, End.Y, End.Z - 1000.0f);
	GetWorld()->LineTraceSingleByChannel(LineTraceHit, Start, End, ECC_Visibility, FCollisionQueryParams());
	if (IsValid(LineTraceHit.GetActor()))
	{
		if (LineTraceHit.GetActor()->GetClass()->GetName() == FString("Landscape"))
		{
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, LineTraceHit.Location.Z));
		}
	}
}

float ACPP_RTS_camera::CGetKeyHeldTime(FName KeyName)
{
	return GetWorld()->GetFirstPlayerController()->GetInputKeyTimeDown(FKey(FName(KeyName)));
}

void ACPP_RTS_camera::BorderMove(float ScrollSpeed)
{
	FVector NewLocation;
	if (mouse_pos_x == 0.0f)
	{
		NewLocation = GetActorLocation() + FVector(0.0f, ScrollSpeed * (-1.0f), 0.0f).RotateAngleAxis(TotalRotation, FVector(0.0f, 0.0f, 1.0f));
		NewLocation = CropLocationVector(NewLocation, ForbiddenAxisX, ForbiddenAxisY);
		SetActorLocation(NewLocation);
	}
		
	if (mouse_pos_x == static_cast<float>(viewport_size_x - 1))
	{
		NewLocation = GetActorLocation() + FVector(0.0f, ScrollSpeed, 0.0f).RotateAngleAxis(TotalRotation, FVector(0.0f, 0.0f, 1.0f));
		NewLocation = CropLocationVector(NewLocation, ForbiddenAxisX, ForbiddenAxisY);
		SetActorLocation(NewLocation);
	}
		
	if (mouse_pos_y == 0.0f)
	{
		NewLocation = GetActorLocation() + FVector(ScrollSpeed, 0.0f, 0.0f).RotateAngleAxis(TotalRotation, FVector(0.0f, 0.0f, 1.0f));
		NewLocation = CropLocationVector(NewLocation, ForbiddenAxisX, ForbiddenAxisY);
		SetActorLocation(NewLocation);
	}
		
	if (mouse_pos_y == static_cast<float>(viewport_size_y - 1))
	{
		NewLocation = GetActorLocation() + FVector(ScrollSpeed * (-1.0f), 0.0f, 0.0f).RotateAngleAxis(TotalRotation, FVector(0.0f, 0.0f, 1.0f));
		NewLocation = CropLocationVector(NewLocation, ForbiddenAxisX, ForbiddenAxisY);
		SetActorLocation(NewLocation);
	}
		
}

// Inputs

void ACPP_RTS_camera::IA_ESC()
{
	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// rmb lmb start
void ACPP_RTS_camera::IA_PRE_LMB_click()
{
	LMB_click = true;
	//HitResult = GetHitResult
	HitActor = HitResult.GetActor();
	LMB_click_pos_x = mouse_pos_x;
	LMB_click_pos_y = mouse_pos_y;
	LMB_world_location = HitResult.Location;
	//MouseWorldPosition = HitResult.Location;

	if (build_mode)
	{
		bSpawnConstruction = false;
		bRotateConstruction = true;
	}
}
void ACPP_RTS_camera::IA_REL_LMB_click()
{
	if (build_mode)
	{
		if (bGoodBuildPosition)
		{
			build_mode = false;
			bSpawnConstruction = false;
			bRotateConstruction = false;
			if (IsValid(WarUnitsArray[0]))
				SelectedBuilding->CreateConstruction(WarUnitsArray[0]);
			SelectedBuilding = nullptr;
		}
		else
		{
			bSpawnConstruction = true;
			bRotateConstruction = false;
		}
	}
	else
	{
		if (bDefaultControl)
		{
			/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("DefaultControl"));
			if ((CGetRadiusVector(LMB_click_pos_x, LMB_click_pos_y, mouse_pos_x, mouse_pos_y) / (CGetRadiusVector(0.0f, 0.0f, viewport_size_x, viewport_size_y) / 100.0f)) < 1.5f)
			{
				if(HitResult.Location != FVector(0.0f, 0.0f, 0.0f))
				{
				if (HitActor->GetClass()->IsChildOf<ABuilding>())
				{
					if (WarUnitsArray.Num() >= 1 && Cast<ABuilding>(HitActor)->bForest && Cast<ABuilding>(HitActor)->bNotCreating)
					{
					}
					else
					{
						if (Cast<ABuilding>(HitActor))
						{
							if (Cast<ABuilding>(HitActor)->PlayerId == GetController()->PlayerState->PlayerId)
							{
								Cast<ARTS_HUD>(Cast<ARTS_Controller>(GetController())->GetHUD())->BreakSelectionEvent();
								Cast<ABuilding>(HitActor)->OnClickedEvent();
							}
						}
					}
				}
				else if (HitActor->GetClass()->IsChildOf<AWarUnit>())
				{
					if (Cast<AWarUnit>(HitActor))
					{
						if (Cast<AWarUnit>(HitActor)->PlayerId == GetController()->PlayerState->PlayerId)
						{
							Cast<ARTS_HUD>(Cast<ARTS_Controller>(GetController())->GetHUD())->BreakSelectionEvent();
							bSelectedWarUnit = true;
							Cast<AWarUnit>(HitActor)->OnClickedEvent();
						}
					}
				}
				else
				{
					Cast<ARTS_HUD>(Cast<ARTS_Controller>(GetController())->GetHUD())->BreakSelectionEvent();
				}
				}
			}
			else
			{
				Cast<ARTS_HUD>(Cast<ARTS_Controller>(GetController())->GetHUD())->BreakSelectionEvent();
				Cast<ARTS_HUD>(Cast<ARTS_Controller>(GetController())->GetHUD())->SelectionEvent();
			}*/
		}
		else
		{
			if ((CGetRadiusVector(LMB_click_pos_x, LMB_click_pos_y, mouse_pos_x, mouse_pos_y) / (CGetRadiusVector(0.0f, 0.0f, viewport_size_x, viewport_size_y) / 100.0f)) < 1.5f)
			{
				if(HitResult.Location != FVector(0.0f, 0.0f, 0.0f))
				{
					if (HitActor->GetClass()->IsChildOf<ABuilding>())
					{
						if (WarUnitsArray.Num() >= 1 && Cast<ABuilding>(HitActor)->bForest && Cast<ABuilding>(HitActor)->bNotCreating)
						{
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CPPMove")));
							//Cast<AWarUnit>(WarUnitsArray[0])->BasicMoveToLocation(MouseWorldPosition);
							//Cast<ARTS_Controller>(GetController())->CRPC_MoveToLocation(WarUnitsArray[0], HitActor->GetActorLocation());
						}
						else
						{
							if (Cast<ABuilding>(HitActor))
							{
								if (Cast<ABuilding>(HitActor)->PlayerId == GetController()->PlayerState->GetPlayerId())
								{
									//Cast<ARTS_HUD>(Cast<ARTS_Controller>(GetController())->GetHUD())->BreakSelectionEvent();
									//Cast<ABuilding>(HitActor)->OnClickedEvent();
								}
							}
						}
					}
					if (HitActor->GetClass()->IsChildOf<AWarUnit>())
					{
						if (Cast<AWarUnit>(HitActor))
						{
							if (Cast<AWarUnit>(HitActor)->PlayerId == GetController()->PlayerState->GetPlayerId())
							{
								Cast<ARTS_HUD>(Cast<ARTS_Controller>(GetController())->GetHUD())->BreakSelectionEvent();
								WarUnitsArray.Push(HitActor);
								bSelectedWarUnit = true;
								Cast<AWarUnit>(HitActor)->OnClickedEvent();
							}
						}
					}
				}
				
				if (bSelectedWarUnit)
				{
					UnitsMovementSquare(MouseWorldPosition, DistanceBetweenUnits, bRotateUnits, WarUnitsArray);
					//for(int32 i = 0; i < WarUnitsArray.Num(); ++i)
					//{
					//	// units destination
					//	Cast<ARTS_Controller>(GetController())->CRPC_SetDestination(WarUnitsArray[i], MouseWorldPosition, ClickedActor);
					//}
				}
				
			}
			else
			{
				Cast<ARTS_HUD>(Cast<ARTS_Controller>(GetController())->GetHUD())->SelectionEvent();
			}
		}
	}
	bSelection = false;
	LMB_click = false;
}

void ACPP_RTS_camera::IA_PRE_MMB_click()
{
	MMB_click_held = true;
}
void ACPP_RTS_camera::IA_REL_MMB_click()
{
	MMB_click_held = false;
	if (MMB_held_time < 0.2f)
		CBreakRotation();
}

void ACPP_RTS_camera::IA_PRE_RMB_click()
{
	RMB_last_click_time = GetGameTimeSinceCreation();
	if (bDefaultControl)
	{
		bScrolling = false;
	}
	else
	{
		bScrolling = true;
	}
		RMB_click = true;
		RMB_click_pos_x = mouse_pos_x;
		RMB_click_pos_y = mouse_pos_y;
}
void ACPP_RTS_camera::IA_REL_RMB_click()
{
	if (bDefaultControl)
	{
		/*if (bSelectedWarUnit)
		{
			for(int32 i = 0; i < WarUnitsArray.Num(); ++i)
			{
				Cast<AWarUnit>(WarUnitsArray[i])->BasicMoveToLocation(MouseWorldPosition);
			}
		}*/
	}
	else
	{
		if ((RMB_last_click_time + RMB_break_timeout) > GetGameTimeSinceCreation())
		//if ((RMB_click_pos_x == mouse_pos_x) && (RMB_click_pos_y == mouse_pos_y))
		{
			if (sqrt(FMath::Square(mouse_pos_x - RMB_click_pos_x) + FMath::Square(mouse_pos_y - RMB_click_pos_y)) < 30.0f)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), (sqrt(FMath::Square(mouse_pos_x - RMB_click_pos_x) + FMath::Square(mouse_pos_y - RMB_click_pos_y)))));
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), 3.0f));
				Cast<ARTS_HUD>(Cast<ARTS_Controller>(GetController())->GetHUD())->BreakSelectionEvent();
				if (build_mode)
				{
					if (SelectedBuilding != nullptr)
					{
						Cast<ABuilding>(SelectedBuilding)->DestroyConstruction();
						SelectedBuilding = nullptr;
					}
					build_mode = false;
					bSpawnConstruction = false;
					bRotateConstruction = false;
				}
			}
		}
	}
	Cast<ARTS_Controller>(GetController())->bShowMouseCursor = true;
	bScrolling = false;
	RMB_click = false;
}
////////////////////////////////////////////// rmb lmb end ///////////////////////////////////////////////////
void ACPP_RTS_camera::IA_Move_X(float rate)
{
	CUpdateMousePosition();
	BorderMove(scroll_speed);
	CMoveMouseXY();
}
void ACPP_RTS_camera::IA_Move_Y(float rate)
{
	CUpdateMousePosition();
	BorderMove(scroll_speed);
	CMoveMouseXY();
}
void ACPP_RTS_camera::IA_Turn_X(float rate)
{
	if (MMB_click_held)
	{
		TotalRotation = rate * rotation_speed + TotalRotation;
		TotalRotation = TotalRotation - 360.0f * static_cast<int32>(TotalRotation / 360.0f);
		AddActorWorldRotation(FRotator(0.0f, rotation_speed * rate, 0.0f));
	}
}
////////////////////////////////////////// END of input ///////////////////////////////////////////////////////
void ACPP_RTS_camera::CancelBuiding()
{
	if (SelectedBuilding != nullptr)
	{
		Cast<ABuilding>(SelectedBuilding)->DestroyConstruction();
		SelectedBuilding = nullptr;
	}
	build_mode = false;

	bSpawnConstruction = false;
	bRotateConstruction = false;
}

void ACPP_RTS_camera::CMoveMouseXY()
{
	if (LMB_click)
	{
		if (bDefaultControl)
		{
			bSelection = true;
		}
		else
		{
			if ((CGetRadiusVector(LMB_click_pos_x, LMB_click_pos_y, mouse_pos_x, mouse_pos_y) / (CGetRadiusVector(0.0f, 0.0f, viewport_size_x, viewport_size_y) / 100.0f)) > 1.5f)
			{
				bSelection = true;
			}
			else
			{
				bSelection = false;
			}	
		}
	}
}

AActor* ACPP_RTS_camera::GetNearestActor(TArray<AActor*> AActorArray, FVector Destination)
{
	float min;
	float current;
	int32 ActorNum = 0;
	min = AActorArray[ActorNum]->GetActorLocation().Distance(Destination,  AActorArray[0]->GetActorLocation());
	for (int32 i = 0; i < AActorArray.Num(); ++i)
	{
		current = AActorArray[i]->GetActorLocation().Distance(Destination, AActorArray[i]->GetActorLocation());
		if (current < min)
		{
			min = current;
			ActorNum = i;
		}
	}
	return AActorArray[ActorNum];
}

FVector ACPP_RTS_camera::CropLocationVector(FVector Location, float x, float y)
{
	if (Location.X * x > 0 && FMath::Abs(Location.X) > FMath::Abs(x))
	{
		Location.X = x;
	}
	if (Location.X * x < 0 && FMath::Abs(Location.X) > FMath::Abs(x))
	{
		Location.X = -1.0f * x;
	}


	if (Location.Y * y > 0 && FMath::Abs(Location.Y) > FMath::Abs(y))
	{
		Location.Y = y;
	}
	if (Location.Y * y < 0 && FMath::Abs(Location.Y) > FMath::Abs(y))
	{
		Location.Y = -1.0f * y;
	}
	return Location;
}

void ACPP_RTS_camera::GetForbiddenAxis()
{
	TArray<AActor*> BlockingLocationActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlockingRTSCameraVolume::StaticClass(), BlockingLocationActor);
	if (BlockingLocationActor.Num())
	{
		ForbiddenAxisX = (Cast<ABlockingRTSCameraVolume>(BlockingLocationActor[0]))->ForbiddenX;
		ForbiddenAxisY = (Cast<ABlockingRTSCameraVolume>(BlockingLocationActor[0]))->ForbiddenY;
	}
}

void ACPP_RTS_camera::SetUnitWidgetsVisibility(bool Visible)
{
	TSubclassOf<class AWarUnit> UnitFilterClass = AWarUnit::StaticClass();
	TSubclassOf<class ABuilding> BuildingFilterClass = ABuilding::StaticClass();
	AActor* EachActor = nullptr;
	for (TActorIterator<AActor> Itr(GetWorld(), UnitFilterClass); Itr; ++Itr)
	{
		EachActor = *Itr;
		if (Cast<AWarUnit>(EachActor))
		{
			Cast<AWarUnit>(EachActor)->HealthWidgetComponent->SetVisibility(Visible);
		}
	}
	for (TActorIterator<AActor> Itr(GetWorld(), BuildingFilterClass); Itr; ++Itr)
	{
		EachActor = *Itr;
		if (Cast<ABuilding>(EachActor))
		{
			Cast<ABuilding>(EachActor)->HealthWidgetComponent->SetVisibility(Visible);
		}
	}
}

void ACPP_RTS_camera::CameraMoved_Implementation()
{

}

void ACPP_RTS_camera::SetFogRevealRadius(float radius)
{
	AdditiveFogBrush->SetScalarParameterValue(FName("DrawSize"), radius);
}



float ACPP_RTS_camera::FindUnitRotationAngle(FVector Destination, TArray<AActor*> Units, int32 LeaderIndex)
{
	return UKismetMathLibrary::FindLookAtRotation(Units[LeaderIndex]->GetActorLocation(), Destination).Yaw;
}

void ACPP_RTS_camera::RotateUnits(float Angle, int32 LeaderIndex, TArray<FVector>& TargetDisplacements)
{
	Angle = Angle * 0.0174533f;
	float DiffX;
	float DiffY;
	float Cos = FMath::Cos(Angle);;
	float Sin = FMath::Sin(Angle);
	//float DifferenceX;
	//float DifferenceY;
	for (int i = 0; i < TargetDisplacements.Num(); ++i)
	{
		DiffX = TargetDisplacements[i].X - TargetDisplacements[LeaderIndex].X;
		DiffY = TargetDisplacements[i].Y - TargetDisplacements[LeaderIndex].Y;
		TargetDisplacements[i].X = TargetDisplacements[LeaderIndex].X + (DiffX * Cos - DiffY * Sin);
		TargetDisplacements[i].Y = TargetDisplacements[LeaderIndex].Y + (DiffY * Cos + DiffX * Sin);
	}
}

void ACPP_RTS_camera::UnitsMovementSquare(FVector Destination, float _distanceBetweenUnits, bool _rotateUnits, TArray<AActor*>& Units)
{
	TArray<FVector> TargetDisplacements;
	int32 ColumnsCountMin = 0;
	int32 ColumnsCount = UKismetMathLibrary::Max(FGenericPlatformMath::CeilToInt(UKismetMathLibrary::Sqrt(static_cast<float>(Units.Num()))), ColumnsCountMin);
	int32 LeaderIndex = UKismetMathLibrary::Min(ColumnsCount, (Units.Num())) / 2;
	if (Units.Num() == 1)
	{
		TargetDisplacements.Add(FVector(0.0f, 0.0f, 0.0f));
	}
	else
	{
		for (int i = 0; i < Units.Num(); ++i)
		{
			TargetDisplacements.Add(FVector(static_cast<float>((-1 * (i / ColumnsCount))) * _distanceBetweenUnits, static_cast<float>((i % ColumnsCount) - (ColumnsCount / 2)) * _distanceBetweenUnits, 0.0f));
		}
	}

	if (_rotateUnits)
	{
		RotateUnits(FindUnitRotationAngle(Destination, Units, LeaderIndex), LeaderIndex, TargetDisplacements);
	}

	for (int i = 0; i < Units.Num(); ++i)
	{
		Cast<ARTS_Controller>(GetController())->CRPC_SetDestination(Units[i], Destination + TargetDisplacements[i], ClickedActor);
	}
}