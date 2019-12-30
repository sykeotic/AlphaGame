#include "ModifyTagEffect.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Logger.h"
#include "Modifier.h"
#include "PlayableCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BattlefieldAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ConditionTree.h"
#include "ModifyTagEffectData.h"

void AModifyTagEffect::ApplyEffectsToActor(AActor* AffectedActor, bool bPlayFeedback)
{
	Super::ApplyEffectsToActor(AffectedActor, bPlayFeedback);
	if (ModifyTagData.Type == EModifyTagType::TAUNT) {
		TauntActor(AffectedActor);
	}
}

void AModifyTagEffect::AssignValues(UBaseEffectData* InData)
{
	Super::AssignValues(InData);
	UModifyTagEffectData* TempData = Cast<UModifyTagEffectData>(InData);
	ModifyTagData = TempData->ModifyTagEffectDataStruct;
}

void AModifyTagEffect::DeactivateEffect(AActor* AffectedActor)
{
	if (ModifyTagData.Type == EModifyTagType::TAUNT) {
		APlayableCharacter* CharCast = Cast<APlayableCharacter>(AffectedActor);
		if (CharCast) {
			CharCast->CharacterAttackStop();
			CharCast->GetCharacterMovement()->StopMovementImmediately();
			ABattlefieldAIController* Cont = Cast<ABattlefieldAIController>(CharCast->GetController());
			if (Cont) {
				Cont->GetBehaviorTreeComponent()->StartTree(*Cont->GetBehaviorTree());
			}
		}
	}
}

void AModifyTagEffect::TauntActor(AActor* AffectedActor)
{
	if (AffectedActor) {
		APlayableCharacter* CharCast = Cast<APlayableCharacter>(AffectedActor);
		if (CharCast) {
			if (FMath::Abs(FVector::Distance(CharCast->GetActorLocation(), ModifierOwner->GetOriginatingActor()->GetActorLocation())) <= 400.f) {
				CharCast->GetCharacterMovement()->StopMovementImmediately();
				CharCast->CharacterAttackStart();
			}
			else {
				ABattlefieldAIController* Cont = Cast<ABattlefieldAIController>(CharCast->GetController());
				if (Cont) {
					Cont->GetBehaviorTreeComponent()->StopTree(EBTStopMode::Safe);
					Cont->MoveToActor(ModifierOwner->GetOriginatingActor());
				}
			}
		}
	}
}

