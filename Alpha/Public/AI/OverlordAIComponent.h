#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OverlordAIComponent.generated.h"

class ACaptainAI;
class ATroopAI;
class UTeamComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALPHA_API UOverlordAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOverlordAIComponent();

	void Init(TArray<ABattlefieldAIController*> InTroops);
	void SetAssignedTeam(UTeamComponent* InTeam);

protected:
	virtual void BeginPlay() override;

private:
	TArray<ACaptainAI*> AssignedCaptains;
	UTeamComponent* AssignedTeam;
};
