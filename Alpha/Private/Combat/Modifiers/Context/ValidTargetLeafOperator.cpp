#include "ValidTargetLeafOperator.h"
#include "ConditionTree.h"
#include "PlayableCharacter.h"
#include "Logger.h"
#include "FactionData.h"
#include "FactionData.h"
#include "Game/TeamComponent.h"
#include "Containers/UnrealString.h"
#include "Modifier.h"

bool UValidTargetLeafOperator::Evaluate()
{
	AActor* ModifierOwner = GetOwnerConditionTree()->GetModifierOwner()->GetActorOwner();
	AActor* OriginatingActor = GetOwnerConditionTree()->GetModifierOwner()->GetOriginatingActor();

	UTeamComponent* OwnerTeam = nullptr;
	UTeamComponent* OriginatingActorTeam = nullptr;

	if (ModifierOwner && OriginatingActor) {
		APlayableCharacter* OwnerChar = Cast<APlayableCharacter>(ModifierOwner);
		APlayableCharacter* OriginatingChar = Cast<APlayableCharacter>(OriginatingActor);

		if (OwnerChar && OriginatingChar) {
			OwnerTeam = OwnerChar->GetOwnerTeam();
			OriginatingActorTeam = OriginatingChar->GetOwnerTeam();
		}
	}

	if (OwnerTeam && OriginatingActorTeam && ModifierOwner && OriginatingActor) {
		switch (Expression.Operator)
		{
		case ELeafOperator::EQUALS:
			switch (Expression.RightValue)
			{
			case EValidTargetsOperand::ALLIES:
				return OwnerTeam->TeamIndex == OriginatingActorTeam->TeamIndex;
				break;
			case EValidTargetsOperand::ENEMIES:
				return OwnerTeam->TeamIndex != OriginatingActorTeam->TeamIndex;
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
				return OwnerTeam->TeamIndex != OriginatingActorTeam->TeamIndex;
				break;
			case EValidTargetsOperand::ENEMIES:
				return OwnerTeam->TeamIndex == OriginatingActorTeam->TeamIndex;
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
