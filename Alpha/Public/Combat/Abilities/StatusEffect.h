#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatusEffect.generated.h"

UCLASS(ABSTRACT)
class ALPHA_API AStatusEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	AStatusEffect();

protected:
	virtual void BeginPlay() override;

public:	

	virtual float AttackModifier(float InDamage);

	virtual float DefenseModifier(float InDamage);

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	float StatusDuration;

	FTimerHandle DurationTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	UParticleSystem* ActorFX;

	UPROPERTY(Transient)
	UParticleSystemComponent* UsePSC;
};
