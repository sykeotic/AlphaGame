// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGameMode.h"
#include "Logger.h"

ATestGameMode::ATestGameMode() {
	ULogger::SetIsTestRun(true);
}