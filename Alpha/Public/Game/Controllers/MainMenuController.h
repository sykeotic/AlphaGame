// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HumanPlayerController.h"
#include "MainMenuController.generated.h"

USTRUCT(BlueprintType)
struct FPlayerMenuStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPlayerType SelectedRole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 SelectedTeamIndex;

};

/**
 * 
 */
UCLASS()
class ALPHA_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		void SetTeamIndex(uint8 InTeamIndex);

	UFUNCTION(BlueprintCallable)
		void SetRole(EPlayerType InRole);

	FPlayerMenuStruct PlayerMenuStruct;

	UUserWidget* CurrentWidget;	

	void SetCurrentWidget(TSubclassOf<UUserWidget> InWidget);
};
