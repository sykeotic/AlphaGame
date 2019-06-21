// Fill out your copyright notice in the Description page of Project Settings.
#include "GeneralHUD.h"
#include "Logger.h"
#include "GameFramework/PlayerController.h"

FVector2D AGeneralHUD::Get2DMousePosition() {
	float PosX;
	float PosY;
	GetOwningPlayerController()->GetMousePosition(PosX, PosY);

	return FVector2D(PosX, PosY);
}

void AGeneralHUD::DrawHUD() {
	if (bStartSelecting) {

		if (FoundActors.Num() > 0) {
			for (int32 i = 0; i < FoundActors.Num(); i++) {
				FoundActors[i]->SwitchOffDecal();
			}
		}

		FoundActors.Empty();
		CurrentPoint = Get2DMousePosition();
		float DiffX = CurrentPoint.X - InitialPoint.X;
		float DiffY = CurrentPoint.Y - InitialPoint.Y;
		DrawRect(FLinearColor(0, 0, 1, .15f), InitialPoint.X, InitialPoint.Y, DiffX, DiffY);
		GetActorsInSelectionRectangle<APlayableCharacter>(InitialPoint, CurrentPoint, FoundActors, false, false);
		if (FoundActors.Num() > 0) {
			for (int32 i = 0; i < FoundActors.Num(); i++) {
				FoundActors[i]->SwitchOnDecal();
			}
		}
	}
}