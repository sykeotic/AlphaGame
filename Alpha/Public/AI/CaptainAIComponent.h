#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "CaptainAIComponent.generated.h"

class ABattlefieldAIController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALPHA_API ACaptainAI : public AInfo
{
	GENERATED_BODY()

public:	
	ACaptainAI();

	void InitCaptain(TArray<ABattlefieldAIController*> InTroops);

private:
	TArray<ABattlefieldAIController*> AssignedTroops;

};
