// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h"
#include "DrawDebugHelpers.h"
#include "HumanPlayerController.generated.h"

UENUM(BlueprintType)
enum class EPlayerType : uint8 {
	HERO UMETA(DisplayName = "Hero"),
	GENERAL UMETA(DisplayName = "General")
};

/**
 * 
 */
class UTeamComponent;
class APlayableCharacter;
class APlayableGeneralPawn;
class UWidgetComponent;
class UPlayerControllerData;
class UFactionData;

UCLASS()
class ALPHA_API AHumanPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHumanPlayerController();

	UFUNCTION(BlueprintCallable)
		void HeroSelectIndex(uint8 HeroCharIndex);

	UFUNCTION(BlueprintCallable)
		void HeroSelectName(FName HeroCharName);

	UFUNCTION(BlueprintCallable)
		void GeneralSelect();

	UFUNCTION(BlueprintCallable)
		void ShowHeroSelectWidget();

	void BeginPlay() override;

	void Unbind();

	void CreateCastingWidget();
	void RemoveCastingWidget();

	void SetupGeneralHUD();

	UFUNCTION(BlueprintCallable)
		UFactionData* GetFactionData();

	UFUNCTION(BlueprintCallable)
		void AssignData(UPlayerControllerData* InData);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class AGeneralHUD* GeneralHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		APlayableCharacter* HeroChar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		APlayableGeneralPawn* GeneralChar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UTeamComponent* ControllerTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
		FVector InLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
		FRotator InRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
		TSubclassOf<class APlayableGeneralPawn> PawnBlueprint;

	bool bHeroChosen;
	bool bGeneralChosen;

	UWidgetComponent* WidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UUserWidget* UserWidget;

	UUserWidget* CastingWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UPlayerControllerData* PlayerControllerData;

	EPlayerType PlayerType;

private:
};