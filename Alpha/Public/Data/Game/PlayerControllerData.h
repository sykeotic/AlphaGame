// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "PlayerControllerData.generated.h"


class UUserWidget;

UCLASS(BlueprintType)
class ALPHA_API UPlayerControllerData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> CharSelectionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> ObjectiveCapturingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName HeroRoleLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName GeneralRoleLabel;	
};
