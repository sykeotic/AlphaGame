#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatComponent.h"
#include "CombatActor.generated.h"

UCLASS(ABSTRACT)
class ACombatActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACombatActor();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	UCombatComponent* ComponentOwner;
	UMaterial* WeaponMaterial;

	void SetComponentOwner(UCombatComponent* InComp);
	UCombatComponent* GetComponentOwner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	FName ProjectileSpawnLocation;
	float UseRange;
	float Damage;

	virtual void OnUse();

	virtual void AssignWeaponValues(UStaticMesh* InStaticMesh, FName InProjectileSpawnLocation, ERange IN_RANGE, EActorType IN_ACTOR_TYPE, float InDmg, float InRange);

protected:

	EActorType ACTOR_TYPE;
	EValidTargets VALID_TARGETS;
	ERange RANGE_TYPE;

public:	

};
