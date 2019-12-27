#include "ValidTargetLeafOperator.h"
#include "ConditionTree.h"
#include "Game/TeamComponent.h"
#include "Modifier.h"

bool UValidTargetLeafOperator::Evaluate()
{
	ULogger::ScreenMessage(FColor::Emerald, "Evaluating Valid Target");

	AActor* ModifierOwner = GetOwnerConditionTree()->GetModifierOwner()->GetActorOwner();
	AActor* OriginatingActor = GetOwnerConditionTree()->GetModifierOwner()->GetOriginatingActor();

	UTeamComponent* OwnerTeam = Cast<UTeamComponent>(ModifierOwner->GetComponentByClass(UTeamComponent::StaticClass()));
	UTeamComponent* OriginatingActorTeam = Cast<UTeamComponent>(OriginatingActor->GetComponentByClass(UTeamComponent::StaticClass()));

	if (OwnerTeam && OriginatingActorTeam && ModifierOwner && OriginatingActor) {
		switch (Expression.Operator)
		{
		case ELeafOperator::EQUALS:
			switch (Expression.RightValue)
			{
			case EValidTargetsOperand::ALLIES:
				return OwnerTeam == OriginatingActorTeam;
				break;
			case EValidTargetsOperand::ENEMIES:
				return OwnerTeam != OriginatingActorTeam;
				break;
			case EValidTargetsOperand::SELF:
				return ModifierOwner == OriginatingActor;
				break;
			default:
				break;
			}
		case ELeafOperator::NOT_EQUAL:
			switch (Expression.RightValue)
			{
			case EValidTargetsOperand::ALLIES:
				return OwnerTeam != OriginatingActorTeam;
				break;
			case EValidTargetsOperand::ENEMIES:
				return OwnerTeam == OriginatingActorTeam;
				break;
			case EValidTargetsOperand::SELF:
				return ModifierOwner != OriginatingActor;
				break;
			default:
				break;
			}
		default:
			break;
		}
	}
	return false;
}
