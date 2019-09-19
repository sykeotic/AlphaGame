// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Runtime/Engine/Classes/Animation/AnimMontage.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "GameplayUtils.generated.h"

class ACombatActor;
class APlayableCharacter;
class UBasePawnData;
class UFactionData;

UENUM(BlueprintType)
enum class EValidTargets : uint8 {
	ALLIES UMETA(DisplayName = "Allies"),
	ENEMIES UMETA(DisplayName = "Enemies"),
	ALL UMETA(DisplayName = "All"),
	SELF UMETA(DisplayName = "Self")
};

UENUM(BlueprintType)
enum class ERange : uint8 {
	MELEE UMETA(DisplayName = "Melee"),
	RANGED UMETA(DisplayName = "Ranged"),
	THROWN UMETA(DisplayName = "Thrown")
};

UENUM(BlueprintType)
enum class EActorType : uint8 {
	WEAPON UMETA(DisplayName = "Weapon"),
	SPELL UMETA(DisplayName = "Spell"),
	CONSUMABLE UMETA(DisplayName = "Consumable")
};


USTRUCT(BlueprintType)
struct FFactionDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName FactionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UFactionData> FactionData;
	
};

USTRUCT(BlueprintType)
struct FGameModeData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName GameModeName;
};

USTRUCT(BlueprintType)
struct FDataTablePaths : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName DataTablePath;
};

/**
 *
 */
class UCombatComponent;
class UStatsComponent;
class UDataTable;
class ACombatActor;
struct FCharacterData;
struct FPlayerCameraData;
struct FPawnGraphicsData;
struct FPawnStatsData;
struct FPlayerControllerData;

UCLASS()
class ALPHA_API UGameplayUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UGameplayUtils();
};