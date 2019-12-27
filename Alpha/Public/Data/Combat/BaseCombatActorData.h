// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Animation/AnimMontage.h"
#include "Engine/DataAsset.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "BaseCombatActorData.generated.h"

class UFeedback;
class UModifierData;

UENUM(BlueprintType)
enum class ECombatActorTypeSpecifier : uint8 {
	RANGED UMETA(DisplayName = "Ranged"),
	MELEE UMETA(DisplayName = "Melee"),
	NONE UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ECombatProfileValidTargets : uint8 {
	ENEMIES UMETA(DisplayName = "Enemies"),
	ALLIES UMETA(DisplayName = "Allies"),
	SELF UMETA(DisplayName = "Self"),
	ALL UMETA(DisplayName = "All"),
	NONE UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ECombatProfileTargetScope : uint8 {
	AREA_OF_EFFECT UMETA(DisplayName = "Area of Effect"),
	SINGLE_TARGET UMETA(DisplayName = "Single Target"),
	MELEE UMETA(DisplayName = "Melee"),
	NONE UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ECombatProfileEffects : uint8 {
	HEAL UMETA(DisplayName = "Heal"),
	DAMAGE UMETA(DisplayName = "Damage"),
	BUFF UMETA(DisplayName = "Buff"),
	DEBUFF UMETA(DisplayName = "Debuff"),
	NONE UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ECombatProfileStrengths : uint8 {
	LOW_HP UMETA(DisplayName = "Low HP %"),
	HIGH_HP UMETA(DisplayName = "High HP %"),
	NO_ARMOR UMETA(DisplayName = "No Armor"),
	LOW_ARMOR UMETA(DisplayName = "Low Armor"),
	MED_ARMOR UMETA(DisplayName = "Medium Armor"),
	HIGH_ARMOR UMETA(DisplayName = "High Armor"),
	NONE UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ECombatProfileWeaknesses : uint8 {
	LOW_HP UMETA(DisplayName = "Low HP %"),
	HIGH_HP UMETA(DisplayName = "High HP %"),
	NO_ARMOR UMETA(DisplayName = "No Armor"),
	LOW_ARMOR UMETA(DisplayName = "Low Armor"),
	MED_ARMOR UMETA(DisplayName = "Medium Armor"),
	HIGH_ARMOR UMETA(DisplayName = "High Armor"),
	NONE UMETA(DisplayName = "None")
};


USTRUCT(BlueprintType)
struct FBaseCombatActorDataStruct {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator MeshRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector MeshLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AlwaysDisplayMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName AttachToSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ExecutionDelay;
	UPROPERTY(EditDefaultsOnly)
		FName VisualFXSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<USoundCue*> UseSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABaseCombatActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UseCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFeedback* OnUseFeedback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UAnimMontage*> UseAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* EquipAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UModifierData*> ModifierData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECombatProfileValidTargets ProfileValidTargets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECombatProfileStrengths ProfileStrengths;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECombatProfileWeaknesses ProfileWeaknesses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECombatProfileEffects ProfileEffects;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECombatProfileTargetScope ProfileTargetScope;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECombatActorTypeSpecifier ProfileActorType;

};

/**
 * 
 */
UCLASS(Abstract)
class UBaseCombatActorData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBaseCombatActorDataStruct BaseCombatActorDataStruct;	
};
