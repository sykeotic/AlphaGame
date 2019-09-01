// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Feedback.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API UFeedback : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<USoundCue*> SoundFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SoundFXBuffer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* VisualFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VisualFXBuffer;

	USoundCue* PickRandomSound();
};
