// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HumanPlayerController.h"
#include "MainMenuGameMode.generated.h"

class UFactionData;
class UGameModeData;
class ULevelData;

USTRUCT(BlueprintType)
struct FMainMenuPlayerStruct
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPlayerType SelectedRole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PlayerID;
};

USTRUCT(BlueprintType)
struct FMainMenuTeamStruct
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UFactionData> SelectedFactionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int32, FMainMenuPlayerStruct> CurrentPlayerData;
};

USTRUCT(BlueprintType)
struct FMainMenuGameStruct
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, EPlayerType> AvailableRoles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, TSoftObjectPtr<UFactionData>> AvailableFactions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, TSoftObjectPtr<ULevelData>> AvailableLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, TSoftObjectPtr<UGameModeData>> AvailableGameModes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf < UUserWidget > MainMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf < UUserWidget > PlayMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf < UUserWidget > OptionsWidget;
};

/**
 * 
 */
UCLASS(BlueprintType)
class ALPHA_API AMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMainMenuGameStruct GameStruct;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TMap<uint8, FMainMenuTeamStruct> Teams;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ULevelData* SelectedLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UGameModeData* SelectedGameMode;

	UFUNCTION(BlueprintCallable)
		void SetPlayerRole(int32 PlayerID, FString InKey);

	UFUNCTION(BlueprintCallable)
		void SetMatchLevel(FString InKey);

	UFUNCTION(BlueprintCallable)
		void SetGameMode(FString InKey);

	UFUNCTION(BlueprintCallable)
		void SetFaction(uint8 TeamID, FString InKey);

	UFUNCTION(BlueprintCallable)
		void SetExportData();

protected:
	AMainMenuGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
