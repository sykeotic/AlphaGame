#include "RangedCombatWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Logger.h"
#include "CombatProjectile.h"
#include "CombatComponent.h"
#include "PlayableCharacter.h"
#include "Engine.h"

ARangedCombatWeapon::ARangedCombatWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ARangedCombatWeapon::OnUse() {
	FTimerHandle DelayAnimHandle;
	if (ProjectileClass != NULL) {
		Super::OnUse();
		FRotator RotationFrom;
		FVector OwnerLoc;
		ComponentOwner->GetOwner()->GetActorEyesViewPoint(OwnerLoc, RotationFrom);
		FVector LocationFrom = MeshComp->GetSocketLocation(ProjectileSpawnLocation);
		RotationFrom.Pitch += 10.0f;
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			ACombatProjectile* const Projectile = World->SpawnActor<ACombatProjectile>(ProjectileClass, LocationFrom, RotationFrom, SpawnParams);
			Projectile->CombatActorOwner = this;
			if (Projectile)
			{
				FVector const LaunchDir = RotationFrom.Vector();
				Projectile->Fire(LaunchDir);
			}
		}
	}
}