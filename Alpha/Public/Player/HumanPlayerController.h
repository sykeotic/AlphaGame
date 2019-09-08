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

USTRUCT(BlueprintType)
struct FPlayerControllerData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> HeroWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> GeneralWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> RoleSelectWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName HeroRoleLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName GeneralRoleLabel;
};
/**
 * 
 */
class UTeamComponent;
class APlayableCharacter;
class APlayableGeneralPawn;
class UWidgetComponent;

UCLASS()
class ALPHA_API AHumanPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHumanPlayerController();

	UFUNCTION(BlueprintCallable)
		void HeroSelect();

	UFUNCTION(BlueprintCallable)
		void GeneralSelect();

	void BeginPlay() override;

	void Unbind();

	void SetupGeneralHUD();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class AGeneralHUD* GeneralHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		APlayableCharacter* HeroChar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		APlayableGeneralPawn* GeneralChar;

	UTeamComponent* ControllerTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
		TSubclassOf<class APlayableCharacter> CharacterBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
		FVector InLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
		FRotator InRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
		TSubclassOf<class APlayableGeneralPawn> PawnBlueprint;

	bool bHeroChosen;
	bool bGeneralChosen;

	UWidgetComponent* WidgetComponent;
	UUserWidget* UserWidget;

	FPlayerControllerData* PlayerControllerData;

private:
	EPlayerType PlayerType;
};