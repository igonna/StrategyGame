// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AssetRegistryModule.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "GameFramework/Pawn.h"
#include "../CPP_RTS_camera.h"
#include "RTS_FogUnit.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogOfWar.generated.h"

#pragma once

// See https://wiki.unrealengine.com/Procedural_Materials

struct DynamicTexturePixel
{
	// Do not rearrage or add members to this struct
	uint8 b;
	uint8 g;
	uint8 r;
	uint8 a;
};

void SetDynamicTexturePixelsToColor(DynamicTexturePixel* pixels, int32 numPixels,
	DynamicTexturePixel color);

struct DynamicTexture
{
private:
	TArray<class UMaterialInstanceDynamic*> DynamicMaterials;
	UTexture2D* Texture2D;
	FUpdateTextureRegion2D* UpdateTextureRegion;

	bool Ready;

public:
	int32 Width;
	int32 Height;

	int32 NumPixels;

	// You can modify this as desired (it should be null when InitializeDynamicTexture() is called).
	// Call Update() once you've finished modifying the Pixels to let Unreal know we want to use the
	// new ones
	DynamicTexturePixel* Pixels;

	~DynamicTexture();

	void Initialize(UStaticMeshComponent* staticMesh, int32 materialIndex, int32 width,
		int32 height);

	void Update();

	DynamicTexturePixel* GetPixel(int32 row, int32 column);
};

UCLASS()
class STRATEGYGAME_API AFogOfWar : public AActor//, public IRTS_FogUnit
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFogOfWar();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray <AActor*> FogUnits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D LevelSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ACPP_RTS_camera* RTS_Camera;

	// Functions
	UFUNCTION(BlueprintCallable)
	void Reveal();
	UFUNCTION(BlueprintCallable)
	void RevealAtLocation(FVector2D Location);

	// Draw
	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;

	void GenerateTexture();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
