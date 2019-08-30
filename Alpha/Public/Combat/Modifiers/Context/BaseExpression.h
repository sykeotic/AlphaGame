// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Engine/DataTable.h"
#include "BaseExpression.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ALPHA_API UBaseExpression : public UObject
{
	GENERATED_BODY()

public:
	virtual bool Evaluate();
};
