// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleTargetCombatAbility.h"

// Sets default values
ASingleTargetCombatAbility::ASingleTargetCombatAbility()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASingleTargetCombatAbility::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASingleTargetCombatAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

