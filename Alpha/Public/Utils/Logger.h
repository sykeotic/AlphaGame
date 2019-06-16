// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Logger.generated.h"

/**
 * 
 */
UCLASS()
class ALPHA_API ULogger : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool bIsTestRun;
	static void ScreenMessage(FColor InColor, FString Message);	
	static void SetIsTestRun(bool IsTest);
};

bool  ULogger::bIsTestRun = false;