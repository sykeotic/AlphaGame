// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "ProjectileData.generated.h"

class UFeedback;

USTRUCT(BlueprintType)
struct FProjectileDataStruct {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector FXScaleTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SphereRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName CollisionProfile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFeedback* Feedback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDiesUponCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bShouldBounce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bRotationFollowsVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TimeAfterHitToDetonate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InitialSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Bounciness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GravityScale;
};


/**
 * 
 */
UCLASS()
class ALPHA_API UProjectileData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FProjectileDataStruct ProjectileDataStruct;
};
