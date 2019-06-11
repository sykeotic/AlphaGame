// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedCombatWeapon.h"

// Sets default values
ARangedCombatWeapon::ARangedCombatWeapon()
{
 	// Set this Weapon to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARangedCombatWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARangedCombatWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

