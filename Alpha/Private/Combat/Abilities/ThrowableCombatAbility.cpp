#include "ThrowableCombatAbility.h"
#include "CombatComponent.h"
#include "Logger.h"
#include "ThrowableProjectile.h"
#include "PlayableCharacter.h"

AThrowableCombatAbility::AThrowableCombatAbility() {

}

void AThrowableCombatAbility::OnUse() {
	if (!bFirstUse) {
		Super::OnUse();
	}
}

void AThrowableCombatAbility::ExecuteUse() {
	ULogger::ScreenMessage(FColor::Red, "FIRING SPELL");
	if (ThrowableProjectileClass != NULL) {
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
			AThrowableProjectile* const Projectile = World->SpawnActor<AThrowableProjectile>(ThrowableProjectileClass, LocationFrom, RotationFrom, SpawnParams);
			Projectile->SetCombatActorOwner(this);
			if (Projectile)
			{
				FVector const LaunchDir = RotationFrom.Vector();
				Projectile->Fire(LaunchDir);
				ULogger::ScreenMessage(FColor::Red, "Spawned Proj Firing");
			}
			else {
				ULogger::ScreenMessage(FColor::Red, "Spawned Proj Null");
			}
		}
	}
	else {
		ULogger::ScreenMessage(FColor::Red, "Proj Null");
	}
}
