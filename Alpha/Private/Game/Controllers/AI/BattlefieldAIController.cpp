#include "BattlefieldAIController.h"
#include "PlayableCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Data/Game/AI/AIData.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

ABattlefieldAIController::ABattlefieldAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("BehaviorTreeComponent"));
	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>(FName("BehaviorTree"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackboardComp"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("AIPerceptionComp"));
}

void ABattlefieldAIController::AssignData(UAIData* InData)
{
	AIData = InData;
	BehaviorTree = AIData->BehaviorTree;
	BlackboardEnemyKey = AIData->BlackboardEnemyKey;
	Blackboard = AIData->Blackboard;
	AIPerceptionComponent = AIData->AIPerceptionComponent;
	if (BehaviorTree)
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
}

AActor* ABattlefieldAIController::GetSeeingPawn()
{
	UObject* object = Blackboard->GetValueAsObject(BlackboardEnemyKey);
	return object ? Cast<AActor>(object) : nullptr;
}

void ABattlefieldAIController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AIController::BeginPlay"));
}

void ABattlefieldAIController::OnPerceptionUpdated(TArray<AActor*> UpdatedActors)
{
	for (AActor* Actor : UpdatedActors){
		if (Actor->IsA<APlayableCharacter>() && !GetSeeingPawn())
		{
			Blackboard->SetValueAsObject(BlackboardEnemyKey, Actor);
			return;
		}
	}
	Blackboard->SetValueAsObject(BlackboardEnemyKey, nullptr);
}
