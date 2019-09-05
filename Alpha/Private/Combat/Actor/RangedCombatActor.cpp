#include "RangedCombatActor.h"
#include "BaseProjectile.h"
#include "Combat/Components/CombatComponent.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Logger.h"

ARangedCombatActor::ARangedCombatActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARangedCombatActor::OnUse()
{
	Super::OnUse();
}

void ARangedCombatActor::ExecuteUse()
{
	ULogger::ScreenMessage(FColor::Red, "RangedCombatActor::ExecuteUse - Modifiers: " + FString::FromInt(Modifiers.Num()));
	ULogger::ScreenMessage(FColor::Red, "RangedCombatActor::ExecuteUse - Effects: " + FString::FromInt(Modifiers[0]->GetEffectCount()));
	LaunchProjectile();
}

void ARangedCombatActor::AssignValues(UBaseCombatActorData* InData)
{
	Super::AssignValues(InData);
	URangedCombatActorData* TempData = Cast<URangedCombatActorData>(InData);
	RangedDataStruct = TempData->RangedCombatActorDataStruct;
}

void ARangedCombatActor::LaunchProjectile()
{
	if (RangedDataStruct.ProjectileClass != NULL) {
		FRotator RotationFrom;
		FVector OwnerLoc;
		ComponentOwner->GetOwner()->GetActorEyesViewPoint(OwnerLoc, RotationFrom);
		FVector LocationFrom = MeshComp->GetSocketLocation(RangedDataStruct.ProjectileSpawnAtSocket);
		RotationFrom.Pitch += 10.0f;
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			ABaseProjectile* const Projectile = World->SpawnActor<ABaseProjectile>(RangedDataStruct.ProjectileClass, LocationFrom, RotationFrom, SpawnParams);
			Projectile->InitProjectileData(RangedDataStruct.ProjectileData->ProjectileDataStruct, LocationFrom);
			Projectile->SetCombatActorOwner(this);
			if (Projectile)
			{
				FVector const LaunchDir = RotationFrom.Vector();
				Projectile->Fire(LaunchDir);
			}
		}
	}
}
