// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCombatActorData.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "MeleeCombatActorData.generated.h"


USTRUCT(BlueprintType)
struct FMeleeCombatActorDataStruct {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SphereRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HitBoxActiveDuration;
};

/**
 * 
 */
UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class ALPHA_API UMeleeCombatActorData : public UBaseCombatActorData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMeleeCombatActorDataStruct MeleeCombatActorDataStruct;
};
