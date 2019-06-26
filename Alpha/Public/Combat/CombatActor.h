#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "CombatActor.generated.h"

UCLASS(ABSTRACT)
class ACombatActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACombatActor();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Instanced, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCombatComponent* ComponentOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UMaterial* WeaponMaterial;

	void SetCombatComponentOwner(UCombatComponent* InComp);

	UFUNCTION(BlueprintCallable)
		UCombatComponent* GetCombatComponentOwner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	FName ProjectileSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float UseRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float UseCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage;

	EActorType ACTOR_TYPE;
	EValidTargets VALID_TARGETS;
	ERange RANGE_TYPE;

	virtual void OnUse();

	virtual void AssignWeaponValues(float InCooldown, UStaticMesh* InStaticMesh, FName InProjectileSpawnLocation, ERange IN_RANGE, EActorType IN_ACTOR_TYPE, float InDmg, float InRange);
};
