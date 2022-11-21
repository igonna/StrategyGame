// Fill out your copyright notice in the Description page of Project Settings.


#include "WarUnit.h"
#include "Engine/World.h"
//#include "AIController.h"
// Sets default values
AWarUnit::AWarUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
	CanAttack = true;
	//SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	//SphereCollision->SetupAttachment(RootComponent);
	SphereVision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereVision"));
	SphereVision->SetupAttachment(RootComponent);

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
	HealthWidgetComponent->SetupAttachment(RootComponent);
	HealthWidgetComponent->SetVisibility(false);
	//SphereCollision->SetRelativeLocation(FVector(0.0f, 0.0f, GetCapsuleComponent()->Height));
}

//////////////////////////////////////////////////////////////////////////
// Replicated Properties

void AWarUnit::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health.
	DOREPLIFETIME(AWarUnit, health);
	DOREPLIFETIME(AWarUnit, UnitColor);
	DOREPLIFETIME(AWarUnit, PlayerId);
	DOREPLIFETIME(AWarUnit, Attacing);
}

// Called when the game starts or when spawned
void AWarUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWarUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Speed = GetVelocity().Size();
	GetScreenLocation(); // Set ScreenPosition
	if (abs(ScreenPosition.Y) > 1.0f || abs(ScreenPosition.Y) == 0.0f)
	{
		GetMesh()->SetComponentTickIntervalAndCooldown(999.0f);
	}
	else
	{
		if (ScreenPosition.Y < 0.0f)
		{
			GetMesh()->SetComponentTickIntervalAndCooldown(0.05f);
		}
		else
		{
			GetMesh()->SetComponentTickIntervalAndCooldown(0.05f);
		}
	}

}

// Called to bind functionality to input
void AWarUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWarUnit::OnClickedEvent_Implementation()
{

}

void AWarUnit::SetDestination_Implementation(FVector Destination, AActor* ClickedActor)
{
	//(Cast<AAIController>(GetController()))->MoveToLocation(Destination);
}

void AWarUnit::Attack_Implementation(AActor* ActorForAttack)
{
	//(Cast<AAIController>(GetController()))->MoveToLocation(Destination);
}

void AWarUnit::INF_SetDestination_Implementation(FVector Destination, AActor* ClickedActor)
{
	SetDestination(Destination, ClickedActor);
}

void AWarUnit::INF_SetPlayerInfo_Implementation(int32 PlayerNum, FLinearColor UUnitColor)
{
    UnitColor = UUnitColor;
    PlayerId = PlayerNum;
    CRPC_SetUnitInfoAll(PlayerId, UnitColor);
}

void AWarUnit::CRPC_SetUnitInfoAll_Implementation(int32 PlayerNum, FLinearColor Color)
{
    UnitColor = Color;
    PlayerId = PlayerNum;
}

bool AWarUnit::GetActorPosInScreenSpace(APlayerController& Player, FVector2D& OutScreenPos)
{
	FVector WorldPosition = GetActorLocation();
	ULocalPlayer* const LP = &Player ? (&Player)->GetLocalPlayer() : nullptr;
	if (LP && LP->ViewportClient)
	{
		// get the projection data
		FSceneViewProjectionData ProjectionData;
		if (LP->GetProjectionData(LP->ViewportClient->Viewport, eSSP_FULL, /*out*/ ProjectionData))
		{
			FMatrix const ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();
			const bool bResult = FSceneView::ProjectWorldToScreen(WorldPosition, ProjectionData.GetConstrainedViewRect(), ViewProjectionMatrix, OutScreenPos);

			FPlane Result = ViewProjectionMatrix.TransformFVector4(FVector4(WorldPosition, 1.f));
			if (Result.W > 0.0f)
			{
				// the result of this will be x and y coords in -1..1 projection space
				const float RHW = 1.0f / Result.W;
				FPlane PosInScreenSpace = FPlane(Result.X * RHW, Result.Y * RHW, Result.Z * RHW, Result.W);

				OutScreenPos = FVector2D{ PosInScreenSpace.X, PosInScreenSpace.Y };
				return true;
			}
		}
	}
	OutScreenPos = FVector2D::ZeroVector;
	return false;
}

FVector2D AWarUnit::GetScreenLocation()
{
	GetActorPosInScreenSpace(*(GetWorld()->GetFirstPlayerController()), ScreenPosition);
	return ScreenPosition;
}

float AWarUnit::INF_GetFogRevealRadius_Implementation()
{
	return SphereVision->GetScaledSphereRadius();
}
