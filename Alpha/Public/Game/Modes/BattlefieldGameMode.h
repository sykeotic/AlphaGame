// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainMenuGameMode.h"
#include "BattlefieldGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API ABattlefieldGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	void InitTeams();

protected:

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void StartMatch() override;	

	void InitTeam(FMainMenuTeamStruct InTeamStruct);

private:

	UGameModeData* CurrentGameModeData;
	TMap<uint8, FMainMenuTeamStruct> TeamData;
};
