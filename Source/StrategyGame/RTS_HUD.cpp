// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_HUD.h"
#include "EngineUtils.h"
#include "WarUnit.h"
#include "Building.h"
#include "Components/CapsuleComponent.h"

void ARTS_HUD::BeginPlay()
{
	Super::BeginPlay();
	UIBorderWidth = 2.0f;
	SelectionColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
	SelectionColorTransparent = FLinearColor(0.0f, 0.125f, 0.0f, 0.05f);
	UIBorderColor = FLinearColor(0.061246f, 0.061246f, 0.061246f, 1.0f);
	SelectionBorderWidth = 1.0f;
}

void ARTS_HUD::ReceiveDrawHUD(int32 SizeX, int32 SizeY)
{
}

void ARTS_HUD::DrawHUD()
{
	Super::DrawHUD();
	GetOwningPlayerController()->GetViewportSize(ViewportSizeX, ViewportSizeY);
	MinimapSize = static_cast<float>(ViewportSizeX) / 6.85;
	
	if (IsValid(MinimapTexture))
	{
		MinimapH = static_cast<float>(MinimapTexture->GetSizeY()) / (static_cast<float>(MinimapTexture->GetSizeY()) / MinimapSize);
		MinimapW = static_cast<float>(MinimapTexture->GetSizeX()) / (static_cast<float>(MinimapTexture->GetSizeY()) / MinimapSize);
	}
	MinimapCoordX = (MinimapH - MinimapW) / 2.0f;
	MinimapCoordXAndUi = UIBorderWidth + MinimapCoordX;
	MinimapCoordY = (static_cast<float>(ViewportSizeY) - UIBorderWidth) - MinimapSize;
	DrawRect(UIBorderColor, 0.0f, (MinimapCoordY - (UIBorderWidth + 6.0f) + UIBorderWidth), (MinimapH + UIBorderWidth + 6.0f), (MinimapH + UIBorderWidth + 6.0f));
	DrawRect(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), UIBorderWidth, MinimapCoordY, MinimapH, MinimapH);
	if (IsValid(MinimapTexture))
		DrawTexture(MinimapTexture, (MinimapCoordX + UIBorderWidth), MinimapCoordY, MinimapW, MinimapH, 0.0f, 0.0f, 1.0f, 1.0f);
	DrawRect(UIBorderColor, (UIBorderWidth + MinimapCoordX - 2.0f), MinimapCoordY, 1.0f, MinimapH);
	DrawRect(UIBorderColor, (UIBorderWidth + MinimapCoordX + MinimapW + 1.0f), MinimapCoordY, 1.0f, MinimapH);
	DrawMinimapFogOfWar();
	AActor* EachActor;
	for (TActorIterator<AActor> Itr(GetWorld(), AWarUnit::StaticClass()); Itr; ++Itr)
	{
		EachActor = *Itr;
		if (Cast<AWarUnit>(EachActor))
		{
			DrawMinimapPoint(MinimapCoordXAndUi, MinimapCoordY, MinimapW, MinimapH, Cast<AWarUnit>(EachActor)->UnitColor, MinimapCoordTL, MinimapCoordRB, EachActor->GetActorLocation(), 2.0f);
		}
	}
	for (TActorIterator<AActor> Itr(GetWorld(), ABuilding::StaticClass()); Itr; ++Itr)
	{
		EachActor = *Itr;
		if (Cast<ABuilding>(EachActor))
		{
			DrawMinimapPoint(MinimapCoordXAndUi, MinimapCoordY, MinimapW, MinimapH, Cast<ABuilding>(EachActor)->BuildingColor, MinimapCoordTL, MinimapCoordRB, EachActor->GetActorLocation(), 4.0f);
		}
	}
	//GetWorld()->
	if (Cast<ACPP_RTS_camera>(GetOwningPlayerController()->GetPawn()))
	{
		ACPP_RTS_camera* RTS_Camera = Cast<ACPP_RTS_camera>(GetOwningPlayerController()->GetPawn());
		if (!(RTS_Camera->build_mode))
		{
			if (RTS_Camera->bSelection)
			{
				DrawSelection(RTS_Camera->LMB_click_pos_x, RTS_Camera->LMB_click_pos_y, (RTS_Camera->mouse_pos_x - RTS_Camera->LMB_click_pos_x), (RTS_Camera->mouse_pos_y - RTS_Camera->LMB_click_pos_y));

				CGetActorsInSelectionRectangle(AWarUnit::StaticClass(), FVector2D(RTS_Camera->LMB_click_pos_x, RTS_Camera->LMB_click_pos_y), FVector2D(RTS_Camera->mouse_pos_x, RTS_Camera->mouse_pos_y), RTS_Camera->WhileSelectionWarUnitsArray);
				CGetActorsInSelectionRectangle(ABuilding::StaticClass(), FVector2D(RTS_Camera->LMB_click_pos_x, RTS_Camera->LMB_click_pos_y), FVector2D(RTS_Camera->mouse_pos_x, RTS_Camera->mouse_pos_y), RTS_Camera->WhileSelectionBuildingsArray);
			}
		}
	}
}
void ARTS_HUD::SelectionEvent_Implementation()
{

}

void ARTS_HUD::BreakSelectionEvent_Implementation()
{

}

void ARTS_HUD::DrawMinimapFogOfWar_Implementation()
{

}

void ARTS_HUD::CGetActorsInSelectionRectangle(TSubclassOf<class AActor> ClassFilter, const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<AActor*>& OutActors, bool bIncludeNonCollidingComponents, bool bActorMustBeFullyEnclosed)
{
	// Because this is a HUD function it is likely to get called each tick,
	// so make sure any previous contents of the out actor array have been cleared!
	OutActors.Reset();

	//Create Selection Rectangle from Points
	FBox2D SelectionRectangle(ForceInit);

	//This method ensures that an appropriate rectangle is generated, 
	//		no matter what the coordinates of first and second point actually are.
	SelectionRectangle += FirstPoint;
	SelectionRectangle += SecondPoint;


	//The Actor Bounds Point Mapping
	const FVector BoundsPointMapping[8] =
	{
		FVector(1.f, 1.f, 1.f),
		FVector(1.f, 1.f, -1.f),
		FVector(1.f, -1.f, 1.f),
		FVector(1.f, -1.f, -1.f),
		FVector(-1.f, 1.f, 1.f),
		FVector(-1.f, 1.f, -1.f),
		FVector(-1.f, -1.f, 1.f),
		FVector(-1.f, -1.f, -1.f) };

	//~~~

	//For Each Actor of the Class Filter Type
	for (TActorIterator<AActor> Itr(GetWorld(), ClassFilter); Itr; ++Itr)
	{
		AActor* EachActor = *Itr;
		if (!IsValid(EachActor))
			continue;
		//Get Actor Bounds				//casting to base class, checked by template in the .h
		//const FBox EachActorBounds = EachActor->GetComponentsBoundingBox(bIncludeNonCollidingComponents); /* All Components? */
		FBox EachActorBounds;
		if (Cast<ABuilding>(EachActor))
		{
			EachActorBounds = Cast<ABuilding>(EachActor)->CollisionCapsule->Bounds.GetBox();
		}
		if (Cast<AWarUnit>(EachActor))
			EachActorBounds = Cast<AWarUnit>(EachActor)->GetCapsuleComponent()->Bounds.GetBox();
		//Cast<AWarUnit>(EachActor)->GetCapsuleComponent()->Bounds;
		//Center
		const FVector BoxCenter = EachActorBounds.GetCenter();

		//Extents
		const FVector BoxExtents = EachActorBounds.GetExtent();

		// Build 2D bounding box of actor in screen space
		FBox2D ActorBox2D(ForceInit);
		for (uint8 BoundsPointItr = 0; BoundsPointItr < 8; BoundsPointItr++)
		{
			// Project vert into screen space.
			const FVector ProjectedWorldLocation = Project(BoxCenter + (BoundsPointMapping[BoundsPointItr] * BoxExtents));
			// Add to 2D bounding box
			ActorBox2D += FVector2D(ProjectedWorldLocation.X, ProjectedWorldLocation.Y);
		}

		//Selection Box must fully enclose the Projected Actor Bounds
		if (bActorMustBeFullyEnclosed)
		{
			if (SelectionRectangle.IsInside(ActorBox2D))
			{
				OutActors.Add(EachActor);
			}
		}
		//Partial Intersection with Projected Actor Bounds
		else
		{
			if (SelectionRectangle.Intersect(ActorBox2D))
			{
				OutActors.Add(EachActor);
			}
		}
	}
}

void ARTS_HUD::DrawMinimapPoint(float minimap_coord_x, float minimap_coord_y, float minimap_w, float minimap_h, FLinearColor PointColor, FVector2D MapCoordTL, FVector2D MapCoordRB, FVector ObjectLocation, float Size)
{
	float MapToMinimap = FVector2D::Distance(MapCoordTL, MapCoordRB) / FVector2D::Distance(FVector2D(0.0f), FVector2D(minimap_w, minimap_h));
	DrawRect(PointColor, minimap_coord_x + (abs(MapCoordTL.Y - ObjectLocation.Y) / MapToMinimap), minimap_coord_y + (abs(MapCoordTL.X - ObjectLocation.X) / MapToMinimap), Size, Size);
}

void ARTS_HUD::DrawSelection(float screen_x, float screen_y, float screen_w, float screen_h)
{
	DrawRect(SelectionColorTransparent, screen_x, screen_y, screen_w, screen_h);
	DrawRect(SelectionColor, screen_x, screen_y, screen_w, SelectionBorderWidth);
	DrawRect(SelectionColor, screen_x, screen_y, SelectionBorderWidth, screen_h);
	DrawRect(SelectionColor, screen_x, screen_y + screen_h, screen_w, SelectionBorderWidth);
	DrawRect(SelectionColor, screen_x + screen_w, screen_y, SelectionBorderWidth, screen_h);
}