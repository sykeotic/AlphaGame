// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "BaseCombatActorData.generated.h"

UENUM(BlueprintType)
enum class ECombatActorTypeSpecifier : uint8 {
	RANGED UMETA(DisplayName = "Ranged"),
	MELEE UMETA(DisplayName = "Melee")
};

USTRUCT(BlueprintType)
struct FBaseCombatActorDataStruct {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* ActorMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName AttachToSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* ActorFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<USoundCue*> UseSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABaseCombatActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UseCooldown;
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

	//enum for Use Animations: HeavyAttackAnim, SpellAnim, etc
	
};
