// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlefieldAIController.h"

void ABattlefieldAIController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AIController::BeginPlay"));
}
