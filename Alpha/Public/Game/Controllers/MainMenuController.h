// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HumanPlayerController.h"
#include "MainMenuController.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()

public:

	UUserWidget* CurrentWidget;	

	void SetCurrentWidget(TSubclassOf<UUserWidget> InWidget);
};
