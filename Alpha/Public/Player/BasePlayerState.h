// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	FName PlayerName;
	int32 HeroKills;
	int32 PawnKills;
	int32 Ping;

	virtual void CopyProperties(class APlayerState* PlayerState);

	virtual void OverrideWith(class APlayerState* PlayerState);
};
