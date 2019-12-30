#include "BaseEffect.h"
#include "Modifier.h"
#include "PlayableCharacter.h"
#include "Data/Combat/Effects/BaseEffectData.h"
#include "Feedback.h"
#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Logger.h"

void ABaseEffect::ApplyEffectsToActor(AActor* AffectedActor, bool bPlayFeedback)
{
	if (bPlayFeedback) {
		ShowEffectFeedback(AffectedActor);
	}
}

void ABaseEffect::AssignValues(UBaseEffectData* InData)
{
	BaseEffectData = InData->BaseEffectDataStruct;
}

void ABaseEffect::DeactivateEffect(AActor* AffectedActor)
{

}

void ABaseEffect::ShowEffectFeedback(AActor* AffectedActor)
{
	APlayableCharacter* AffChar = Cast<APlayableCharacter>(AffectedActor);
	if (AffChar && BaseEffectData.OnImpactFeedback) {
		if (BaseEffectData.OnImpactFeedback->VisualFX)
		{
			UParticleSystemComponent* UsePSC;
			if (ModifierOwner->GetActorOwner()) {
				// ULogger::ScreenMessage(FColor::Orange, "BaseEffect::ShowEffectFeedback()");
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

void ABaseEffect::SetModifierOwner(AModifier* InMod)
{
	ModifierOwner = InMod;
}
