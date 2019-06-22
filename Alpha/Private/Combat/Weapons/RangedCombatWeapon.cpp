#include "RangedCombatWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Logger.h"
#include "CombatComponent.h"
#include "PlayableCharacter.h"
#include "Engine.h"

ARangedCombatWeapon::ARangedCombatWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ARangedCombatWeapon::OnUse(){
	Super::OnUse();
	FRotator OwnerView;
	FVector OwnerLoc;
	ComponentOwner->GetOwner()->GetActorEyesViewPoint(OwnerLoc, OwnerView);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	FVector WeaponLocation = MeshComp->GetSocketLocation(ProjectileSpawnLocation);
	FRotator WeaponRotation = MeshComp->GetSocketRotation(ProjectileSpawnLocation);

	FVector ShotDirection = OwnerView.Vector();

	UWorld* World = GetWorld();
	FVector TraceEnd = WeaponLocation + (OwnerView.Vector() * UseRange);

	FCollisionQueryParams QueryParams;
	FHitResult Hit;

	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(ComponentOwner->GetOwner());
	QueryParams.bTraceComplex = true;

	if (World->LineTraceSingleByChannel(Hit, WeaponLocation, TraceEnd, ECC_Visibility, QueryParams)) {
		if (ComponentOwner->GetOwner() && ComponentOwner) {
			DrawDebugLine(GetWorld(), WeaponLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
			AActor* HitActor = Hit.GetActor();
			if (IsValid(HitActor)) {
				ULogger::ScreenMessage(FColor::Red, "HitActor Valid!");
			}
			UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, Hit, ComponentOwner->GetOwner()->GetInstigatorController(), this, DamageType);
		}
		else {
			ULogger::ScreenMessage(FColor::Red, "Owner null");
		}
	}
	else {
		ULogger::ScreenMessage(FColor::Blue, TEXT("LineTrace failed!"));
	}
}