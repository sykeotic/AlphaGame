// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "PlayerControllerData.generated.h"


class UUserWidget;

USTRUCT(BlueprintType)
struct FPlayerControllerDataStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> HeroWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> GeneralWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> RoleSelectWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> CharSelectionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName HeroRoleLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName GeneralRoleLabel;
};

UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class ALPHA_API UPlayerControllerData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerControllerDataStruct PlayerControllerData;
	
};
