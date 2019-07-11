// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerState.h"

void ABasePlayerState::CopyProperties(class APlayerState* PlayerState) {
	Super::CopyProperties(PlayerState);
	if (PlayerState) {
		ABasePlayerState* TestPlayerState = Cast<ABasePlayerState>(PlayerState);
	}
}
void ABasePlayerState::OverrideWith(class APlayerState* PlayerState) {
	Super::OverrideWith(PlayerState);
	if (PlayerState) {
		ABasePlayerState* TestPlayerState = Cast<ABasePlayerState>(PlayerState);
	}
}