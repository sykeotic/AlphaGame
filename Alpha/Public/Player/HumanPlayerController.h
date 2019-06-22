// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayableCharacter.h"
#include "PlayableGeneralPawn.h"
#include "DrawDebugHelpers.h"
#include "CombatActor.h"
#include "HumanPlayerController.generated.h"

UENUM(BlueprintType)
enum class EPlayerType : uint8 {
	HERO UMETA(DisplayName = "Hero"),
	GENERAL UMETA(DisplayName = "General")
};
/**
 * 
 */
UCLASS()
class ALPHA_API AHumanPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AHumanPlayerController();
	void PlayerTypeSelection();

	UFUNCTION(BlueprintCallable)
	void HeroSelect(FString& InKey);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Game")
	void AssignMesh(const FString& InKey);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Game")
	void GetValues(const FString& InKey);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Game")
	void SetupHero(const FString& InKey);

	UFUNCTION(BlueprintCallable)
	void GeneralSelect();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	APlayableCharacter* HeroChar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	APlayableGeneralPawn* GeneralChar;

	bool bHeroChosen;
	bool bGeneralChosen;

private:
	EPlayerType PlayerType;
};
