#include "SingleTargetCombatAbility.h"
#include "CombatComponent.h"
#include "Logger.h"
#include "CombatProjectile.h"
#include "PlayableCharacter.h"

ASingleTargetCombatAbility::ASingleTargetCombatAbility()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASingleTargetCombatAbility::OnUse() {
	Super::OnUse();
	if (ProjectileClass != NULL) {
		FRotator RotationFrom;
		FVector OwnerLoc;
		ComponentOwner->GetOwner()->GetActorEyesViewPoint(OwnerLoc, RotationFrom);
		FVector LocationFrom = ComponentOwner->CharacterOwner->GetMesh()->GetSocketLocation(ActorSocketLocation);
		RotationFrom.Pitch += 10.0f;
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			ACombatProjectile* const Projectile = World->SpawnActor<ACombatProjectile>(ProjectileClass, LocationFrom, RotationFrom, SpawnParams);
			Projectile->SetCombatActorOwner(this);
			if (Projectile)
			{
				FVector const LaunchDir = RotationFrom.Vector();
				Projectile->Fire(LaunchDir);
			}
		}
		BoolSpam();
	}
	else {
		ULogger::ScreenMessage(FColor::Red, "Proj Null");
	}
}

