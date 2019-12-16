#include "BattlefieldAIController.h"
#include "PlayableCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Data/Game/AI/AIData.h"
#include "PlayableCharacter.h"
#include "TeamComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Game/TeamComponent.h"
#include "Data/Game/FactionData.h"
#include "TimerManager.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "BehaviorTree/BlackboardComponent.h"

ABattlefieldAIController::ABattlefieldAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("BehaviorTreeComponent"));
	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>(FName("BehaviorTree"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackboardComp"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("AIPerceptionComp"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("Sight Config"));
	Sight->SightRadius = 1000.f;
	Sight->LoseSightRadius = 1500.f;
	Sight->PeripheralVisionAngleDegrees = 130.f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*Sight);
}

void ABattlefieldAIController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AIController::BeginPlay"));
}

void ABattlefieldAIController::AssignData(UAIData* InData)
{
	AIData = InData;
	BehaviorTree = AIData->BehaviorTree;
	BlackboardEnemyKey = AIData->BlackboardEnemyKey;
	BlackboardCanSeeEnemyKey = AIData->BlackboardCanSeeEnemyKey;
	Sight->SightRadius = InData->SightRadius;
	Sight->LoseSightRadius = InData->LoseSightRadius;
	Sight->PeripheralVisionAngleDegrees = InData->PeripheralVisionAngleDegrees;
	Sight->DetectionByAffiliation.bDetectEnemies = InData->DetectEnemies;
	Sight->DetectionByAffiliation.bDetectFriendlies = InData->DetectAllies;
	Sight->DetectionByAffiliation.bDetectNeutrals = InData->DetectNeutrals;
	if (BehaviorTree)
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		// BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ABattlefieldAIController::OnPerceptionUpdated);
}

AActor* ABattlefieldAIController::GetSeeingPawn()
{
	UObject* object = Blackboard->GetValueAsObject(BlackboardEnemyKey);
	return object ? Cast<AActor>(object) : nullptr;
}

void ABattlefieldAIController::HandlePawnDeath()
{
	UnPossess();
	GetWorld()->DestroyActor(AIStatusText);
	bPossessed = false;
	FTimerHandle SpawnTimer;
	BehaviorTreeComponent->StopTree();
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ABattlefieldAIController::SpawnNewPawn, 3.f, false);
}

void ABattlefieldAIController::JumpRandomly()
{
	FTimerHandle JumpTimer;
	float TimeHandle = FMath::RandRange(1.f, 5.f);
	GetWorldTimerManager().SetTimer(JumpTimer, this, &ABattlefieldAIController::ExecuteJump, TimeHandle, true);
}

void ABattlefieldAIController::ExecuteJump()
{
	AICharacter->Jump();
}

void ABattlefieldAIController::SpawnNewPawn()
{
	float ChangeFloat = 0.f;
	FVector Vec = { FMath::FRandRange(0.f + ChangeFloat, 2000.f + ChangeFloat), FMath::FRandRange(0.f + ChangeFloat, 2000.f + ChangeFloat), 350.0f };
	FRotator Rot = { 0, 0, 0 };
	uint8 Selection = FMath::RandRange(0, ControllerTeam->FactionData->AvailableHeroes.Num() - 1);
	AICharacter = ControllerTeam->SpawnTeamCharacter(Vec, Rot, Selection);
	Possess(AICharacter);
	bPossessed = true;
	AICharacter->SetIsAIPlayer(true);
	SpawnTextActor();
	EngageBrainPulseLoop();
	// RunBehaviorTree(BehaviorTree);
}

void ABattlefieldAIController::SpawnTextActor()
{
	if (AICharacter) {
		AIStatusText = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), FVector(200.f, 200.f, 200.f), FRotator(0.f, 0.f, 0.f));
		AIStatusText->GetTextRender()->SetText(FText::FromString(TEXT("SPAWN")));
		AIStatusText->GetTextRender()->SetTextRenderColor(FColor::Yellow);
		AIStatusText->SetActorScale3D(FVector(.7f, .7f, .7f));
		AIStatusText->AttachToComponent(Cast<USceneComponent>(AICharacter->GetMesh()), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		AIStatusText->GetTextRender()->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
	}
}

void ABattlefieldAIController::EngageBrainPulseLoop()
{
	GetWorld()->GetTimerManager().SetTimer(PulseHandler, this, &ABattlefieldAIController::BrainPulse, 1.f, false);
}

void ABattlefieldAIController::SetPossessed(bool inPossessed)
{
	bPossessed = inPossessed;
}

void ABattlefieldAIController::BrainPulse()
{
	if (!AICharacter) {
		UE_LOG(LogTemp, Warning, TEXT("AIController::SpawnNewPawn No AI Character"));
	}
	if (!AIStatusText) {
		UE_LOG(LogTemp, Warning, TEXT("AIController::SpawnNewPawn No AIStatusText"));
	}
	if (AICharacter && bPossessed) {
		if (VisibleEnemies.Num() <= 0) {
			bInCombat = false;
			if (AICharacter && AIStatusText) {
				AIStatusText->GetTextRender()->SetText(FText::FromString(TEXT("PEACE")));
				AIStatusText->GetTextRender()->SetTextRenderColor(FColor::Green);
			}
			NonCombatPulse();
		}
		else {
			bInCombat = true;
			if (AICharacter && AIStatusText) {
				AIStatusText->GetTextRender()->SetText(FText::FromString(TEXT("COMBAT")));
				AIStatusText->GetTextRender()->SetTextRenderColor(FColor::Red);
			}
			CombatPulse();
		}
	}
	if (bPossessed) {
		GetWorld()->GetTimerManager().SetTimer(PulseHandler, this, &ABattlefieldAIController::BrainPulse, 1.f, true);
	}
}

void ABattlefieldAIController::NonCombatPulse()
{
	// TODO Make the AI think
}

void ABattlefieldAIController::CombatPulse()
{
	// TODO Make the AI think
}

void ABattlefieldAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	
	for (AActor* Actor : UpdatedActors) {
		if (Actor->IsA<APlayableCharacter>() && !GetSeeingPawn() && Cast<APlayableCharacter>(Actor)->GetOwnerTeam() != ControllerTeam )
		{
			if (!VisibleEnemies.Contains(Actor)) {
				VisibleEnemies.Add(Actor);
			}
			else {
				VisibleEnemies.Remove(Actor);
			}
		}
	}
}

void ABattlefieldAIController::SetAICharacter(APlayableCharacter* InChar)
{
	AICharacter = InChar;
}

APlayableCharacter* ABattlefieldAIController::GetAICharacter()
{
	return AICharacter;
}

void ABattlefieldAIController::SetControllerTeam(UTeamComponent* InTeam)
{
	ControllerTeam = InTeam;
}

UTeamComponent* ABattlefieldAIController::GetControllerTeam()
{
	return ControllerTeam;
}
