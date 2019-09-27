// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainMenuGameMode.generated.h"

class UFactionData;

USTRUCT(BlueprintType)
struct FMainMenuTeamStruct
{
	GENERATED_BODY()
public:
	UFactionData* SelectedTeamData;

	uint8 TeamIndex;

};

USTRUCT(BlueprintType)
struct FMainMenuGameStruct
{
	GENERATED_BODY()
public:
	TArray<FMainMenuTeamStruct> Teams;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> MainMenuWidget;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf < UUserWidget> PlayMenuWidget;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf < UUserWidget> OptionsWidget;
};

/**
 * 
 */
UCLASS(BlueprintType)
class ALPHA_API AMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		FMainMenuGameStruct GameStruct;

protected:
	AMainMenuGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};
