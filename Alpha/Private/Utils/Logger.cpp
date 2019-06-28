// Fill out your copyright notice in the Description page of Project Settings.

#include "Logger.h"
#include "Engine.h"

void ULogger::ScreenMessage(FColor InColor, FString Message) {
	if (bIsTestRun && GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, InColor, Message);
	}
}

void ULogger::SetIsTestRun(bool IsTest) {
	ULogger::bIsTestRun = IsTest;
}