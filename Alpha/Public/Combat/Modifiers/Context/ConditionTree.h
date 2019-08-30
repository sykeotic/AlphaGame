// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "BaseExpression.h"
#include "ConditionTree.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ALPHA_API UConditionTree : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
		TArray<UBaseExpression*> ExpressionTree;

	bool IsConditionTreeTrue();
};
