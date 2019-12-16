#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "CaptainAIComponent.generated.h"

class ABattlefieldAIController;
class AObjectiveOverlapActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALPHA_API ACaptainAI : public AInfo
{
	GENERATED_BODY()

public:	
	ACaptainAI();

	void InitCaptain(TArray<ABattlefieldAIController*> InTroops);

	void SetAssignedTroops(TArray<ABattlefieldAIController*> InTroops);
	TArray<ABattlefieldAIController*> GetAssignedTroops();

	AObjectiveOverlapActor* GetCurrentGoal();
	void SetGoal(AObjectiveOverlapActor* InGoal);

private:
	TArray<ABattlefieldAIController*> AssignedTroops;

	AObjectiveOverlapActor* CurrentGoal;

};
