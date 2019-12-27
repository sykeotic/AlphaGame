#include "BaseEffect.h"
#include "Modifier.h"
#include "PlayableCharacter.h"
#include "Data/Combat/Effects/BaseEffectData.h"
#include "Feedback.h"
#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Logger.h"

void ABaseEffect::ApplyEffectsToActor(AActor* AffectedActor)
{
	APlayableCharacter* AffChar = Cast<APlayableCharacter>(AffectedActor);
	if (AffChar) {
		if (BaseEffectData.OnImpactFeedback->VisualFX)
		{
			UParticleSystemComponent* UsePSC;
			if (ModifierOwner->GetActorOwner()) {
				UsePSC = UGameplayStatics::SpawnEmitterAttached(BaseEffectData.OnImpactFeedback->VisualFX, AffChar->GetMesh(), BaseEffectData.EffectFXLocation);
			}
		}

		UAudioComponent* AC = nullptr;
		USoundCue* InSound = BaseEffectData.OnImpactFeedback->PickRandomSound();
		if (InSound)
		{
			AC = UGameplayStatics::SpawnSoundAttached(InSound, AffChar->GetRootComponent());
		}

	}
}

void ABaseEffect::AssignValues(UBaseEffectData* InData)
{
	BaseEffectData = InData->BaseEffectDataStruct;
}

void ABaseEffect::DeactivateEffect(AActor* AffectedActor)
{

}

void ABaseEffect::SetModifierOwner(AModifier* InMod)
{
	ModifierOwner = InMod;
}
