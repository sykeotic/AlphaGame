// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayableCharacter.h"
#include "GeneralHUD.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API AGeneralHUD : public AHUD
{
	GENERATED_BODY()

public:

	FVector2D InitialPoint;
	FVector2D CurrentPoint;

	bool bStartSelecting = false;

	FVector2D Get2DMousePosition();
	virtual void DrawHUD();

	TArray<APlayableCharacter*> SelectedActors;
};
