// Fill out your copyright notice in the Description page of Project Settings.

#include "FogOfWar.h"
#include "RTS_FogUnit.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "../WarUnit.h"
#include "../Building.h"
// Sets default values
AFogOfWar::AFogOfWar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFogOfWar::BeginPlay()
{
	Super::BeginPlay();
	RTS_Camera = Cast<ACPP_RTS_camera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void AFogOfWar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Reveal();
}

void AFogOfWar::Reveal()
{
	if (!IsValid(RTS_Camera))
		return;
	FVector UnitLocation;
	UKismetRenderingLibrary::ClearRenderTarget2D(this, RTS_Camera->FogRenderTarget, FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), RTS_Camera->FogRenderTarget, Canvas, Size, Context);
	for (int i = 0; i < FogUnits.Num(); ++i)
	{
		if (!Cast<IRTS_FogUnit>(FogUnits[i]))
			continue;
		UnitLocation = FogUnits[i]->GetActorLocation();
		//RTS_Camera->SetFogRevealRadius(Cast<IRTS_FogUnit>(FogUnits[i])->INF_GetFogRevealRadius() / LevelSize.X);
		if (Cast<AWarUnit>(FogUnits[i]))
			RTS_Camera->SetFogRevealRadius(Cast<AWarUnit>(FogUnits[i])->SphereVision->GetScaledSphereRadius() / LevelSize.X);
		if (Cast<ABuilding>(FogUnits[i]))
			RTS_Camera->SetFogRevealRadius(Cast<ABuilding>(FogUnits[i])->SphereVision->GetScaledSphereRadius() / LevelSize.X);
		// low preformance begin
		RevealAtLocation(FVector2D((UnitLocation.X / LevelSize.X), (UnitLocation.Y / LevelSize.Y)) + 0.5f);
		// low performance end
	}
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
}

void AFogOfWar::RevealAtLocation(FVector2D Location)
{
	RTS_Camera->AdditiveFogBrush->SetVectorParameterValue(FName("DrawLocation"), FLinearColor(Location.X, Location.Y, 0.0f));
	// low performance begin
	//UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(),  RTS_Camera->FogRenderTarget, RTS_Camera->AdditiveFogBrush);
	//UKismetRenderingLibrary::DrawMaterial
	Canvas->K2_DrawMaterial(RTS_Camera->AdditiveFogBrush, FVector2D(0.0f, 0.0f), Size, FVector2D(0.0f, 0.0f));
	// low performance end
}

// Dynamic Texture
struct UpdateTextureRegionsParams
{
	UTexture2D* Texture;
	int32 MipIndex;
	uint32 NumRegions;
	FUpdateTextureRegion2D* Regions;
	uint32 SrcPitch;
	uint32 SrcBpp;
	uint8* SrcData;
	bool FreeData;
};

// Send the render command to update the texture
void UpdateTextureRegions(UpdateTextureRegionsParams& params)
{
	if (params.Texture && params.Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)params.Texture->Resource;
		RegionData->MipIndex = params.MipIndex;
		RegionData->NumRegions = params.NumRegions;
		RegionData->Regions = params.Regions;
		RegionData->SrcPitch = params.SrcPitch;
		RegionData->SrcBpp = params.SrcBpp;
		RegionData->SrcData = params.SrcData;

		//ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER
			/*ENQUEUE_RENDER_COMMAND(
			UpdateTextureRegionsData, FUpdateTextureRegionsData*, RegionData, RegionData, bool,
			FreeData, params.FreeData,
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex], RegionData->SrcPitch,
							RegionData->SrcData +
								RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch +
								RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp);
					}
				}
				if (FreeData)
				{
					FMemory::Free(RegionData->Regions);
					FMemory::Free(RegionData->SrcData);
				}
				delete RegionData;
			});*/
		//ENQUEUE_RENDER_COMMAND(FSetShaderMapOnColorTransformResources)(
			//[ColorTransform, ShaderMap](FRHICommandListImmediate& RHICmdList)
			//{

			//}
	}
}

void SetDynamicTexturePixelsToColor(DynamicTexturePixel* pixels, int32 numPixels, DynamicTexturePixel color)
{
	if (pixels)
	{
		for (int i = 0; i < numPixels; i++)
			pixels[i] = color;
	}
}

DynamicTexture::~DynamicTexture()
{
	if (Pixels)
		delete[] Pixels;
	if (UpdateTextureRegion)
		delete UpdateTextureRegion;
}

void DynamicTexture::Initialize(UStaticMeshComponent* staticMesh, int32 materialIndex, int32 width,
	int32 height)
{
	if (!staticMesh)
		return;

	Width = width;
	Height = height;

	if (!Width || !Height)
	{
		// Dynamic Texture needs width and height in order to function!
		return;
	}

	// Texture2D setup
	{
		Texture2D = UTexture2D::CreateTransient(Width, Height);
		// Ensure there's no compression (we're editing pixel-by-pixel)
		Texture2D->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		// Turn off Gamma correction
		Texture2D->SRGB = 0;
		// Make sure it never gets garbage collected
		Texture2D->AddToRoot();
		// Update the texture with these new settings
		Texture2D->UpdateResource();
	}

	// TextureRegion setup
	UpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, Width, Height);

	// Pixels setup
	{
		NumPixels = Width * Height;
		if (Pixels)
			delete[] Pixels;
		Pixels = new DynamicTexturePixel[NumPixels];

		SetDynamicTexturePixelsToColor(Pixels, NumPixels, { 0, 0, 0, 255 });
	}

	// Material setup
	{
		UMaterialInstanceDynamic* material = staticMesh->CreateAndSetMaterialInstanceDynamic(materialIndex);

		if (!material)
			return;

		DynamicMaterials.Empty();
		DynamicMaterials.Add(material);

		for (UMaterialInstanceDynamic* dynamicMaterial : DynamicMaterials)
		{
			if (dynamicMaterial)
				dynamicMaterial->SetTextureParameterValue("DynamicTextureParam", Texture2D);
		}
	}

	Ready = true;
}

void DynamicTexture::Update()
{
	if (Ready)
	{
		UpdateTextureRegionsParams params = {
			/*Texture = */ Texture2D,
			/*MipIndex = */ 0,
			/*NumRegions = */ 1,
			/*Regions = */ UpdateTextureRegion,
			/*SrcPitch = */ static_cast<uint32>(Width * sizeof(DynamicTexturePixel)),
			/*SrcBpp = */ sizeof(DynamicTexturePixel),
			/*SrcData = */ reinterpret_cast<uint8*>(Pixels),
			/*FreeData = */ false,
		};
		UpdateTextureRegions(params);
	}
}

DynamicTexturePixel* DynamicTexture::GetPixel(int32 row, int32 column)
{
	if (!Pixels || row >= Height || column >= Width)
		return nullptr;

	return &Pixels[(row * Width) + column];
}

void AFogOfWar::GenerateTexture()
{
	//Create a string containing the texture's path
	FString PackageName = TEXT("/Game/ProceduralTextures/");
	FString BaseTextureName = FString("ProcTexture");
	PackageName += BaseTextureName;

	//Create the package that will store our texture
	UPackage* Package = CreatePackage(NULL, *PackageName);
	GLog->Log("project dir:" + FPaths::ProjectDir());

	//Create a unique name for our asset. For example, if a texture named ProcTexture already exists the editor
	//will name the new texture as "ProcTexture_1"
	FName TextureName = MakeUniqueObjectName(Package, UTexture2D::StaticClass(), FName(*BaseTextureName));
	Package->FullyLoad();

	UTexture2D* NewTexture = NewObject & lt;
	UTexture2D > (Package, TextureName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	if (NewTexture)
	{
		int32 TextureWidth = 1024;
		int32 TextureHeight = 1024;

		//Prevent the object and all its descedants from being deleted during garbage collecion
		NewTexture->AddToRoot();

		//Initialize the platform data to store necessary information regarding our texture asset
		NewTexture->PlatformData = new FTexturePlatformData();
		NewTexture->PlatformData->SizeX = TextureWidth;
		NewTexture->PlatformData->SizeY = TextureHeight;
		NewTexture->PlatformData->NumSlices = 1;
		NewTexture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;


		//uint8* Pixels = GeneratePixels(TextureHeight, TextureWidth);
		//Each element of the array contains a single color so in order to represent information in
		//RGBA we need to have an array of length TextureWidth * TextureHeight * 4
		uint8* Pixels = new uint8[TextureWidth * TextureHeight * 4];
		for (int32 y = 0; y & lt; TextureHeight; y++)
		{
			for (int32 x = 0; x & lt; TextureWidth; x++)
			{
				//Get the current pixel
				int32 CurrentPixelIndex = ((y * TextureWidth) + x);
				//Get a random vector that will represent the RGB values for the current pixel
				FColor RandomColor = FColor::MakeRandomColor();

				Pixels[4 * CurrentPixelIndex] = RandomColor.B; //b
				Pixels[4 * CurrentPixelIndex + 1] = RandomColor.G; //g
				Pixels[4 * CurrentPixelIndex + 2] = RandomColor.R; //r
				Pixels[4 * CurrentPixelIndex + 3] = 255; //set A channel always to maximum
			}
		}

		//Allocate first mipmap.
		FTexture2DMipMap* Mip = new FTexture2DMipMap();
		NewTexture->PlatformData->Mips.Add(Mip);
		Mip->SizeX = TextureWidth;
		Mip->SizeY = TextureHeight;

		//Lock the mipmap data so it can be modified
		Mip->BulkData.Lock(LOCK_READ_WRITE);
		uint8* TextureData = (uint8*)Mip->BulkData.Realloc(TextureWidth * TextureHeight * 4);
		//Copy the pixel data into the Texture data
		FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * TextureHeight * TextureWidth * 4);
		Mip->BulkData.Unlock();


		//Initialize a new texture
		NewTexture->Source.Init(TextureWidth, TextureHeight, 1, 1, ETextureSourceFormat::TSF_BGRA8, Pixels);
		NewTexture->UpdateResource();

		//Mark the package as dirty so the editor will prompt you to save the file if you haven't
		Package->MarkPackageDirty();

		//Notify the editor that we created a new asset
		FAssetRegistryModule::AssetCreated(NewTexture);

		//Auto-save the new  asset
		FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
		bool bSaved = UPackage::SavePackage(Package, NewTexture, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

		//Since we don't need access to the pixel data anymore free the memory
		delete[] Pixels;
	}

}