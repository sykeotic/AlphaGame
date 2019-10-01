#include "BattlefieldGameMode.h"
#include "Engine/World.h"
#include "Logger.h"
#include "TeamComponent.h"
#include "Game/Modes/Battlefield/BattlefieldGameState.h"
#include "HumanPlayerController.h"
#include "FactionData.h"
#include "PlayableCharacter.h"
#include "GameModeData.h"
#include "Kismet/GameplayStatics.h"
#include "Data/Game/AI/AIData.h"
#include "BattlefieldAIController.h"
#include "Game/PlayableGameInstance.h"

ABattlefieldGameMode::ABattlefieldGameMode()
{
	GameStateClass = ABattlefieldGameState::StaticClass();
	bTeamsInit = false;
}

void ABattlefieldGameMode::InitTeams()
{
	UE_LOG(LogTemp, Warning, TEXT("BattlefieldGameMode::InitTeams - Initializing Teams"));
	CurrentGameInstance = Cast<UPlayableGameInstance>(GetGameInstance());
	CurrentGameModeData = CurrentGameInstance->GetGameModeData();
	TeamData = CurrentGameInstance->GetTeamData();
	for (auto& Elem : TeamData) {
		InitTeam(Elem.Key, TeamData.FindRef(Elem.Key));
	}
}

void ABattlefieldGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BattlefieldGameMode::BeginPlay - BeginPlay"));	
}

void ABattlefieldGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (!bTeamsInit) {
		InitTeams();
		bTeamsInit = true;
	}
	UE_LOG(LogTemp, Warning, TEXT("BattlefieldGameMode::PostLogin - Initializing New Player"));
	AHumanPlayerController* PlayerController = Cast<AHumanPlayerController>(NewPlayer);
	PlayerController->AssignData(CurrentGameInstance->GetGameModeData()->PlayerControllerData);
	ABattlefieldGameState* CurrentGameState = Cast<ABattlefieldGameState>(UGameplayStatics::GetGameState(GetWorld()));
	int32 PersistentID = Cast<UPlayableGameInstance>(PlayerController->GetGameInstance())->GetPersistentID();
	if (CurrentGameState) {		
		PlayerController->ControllerTeam = CurrentGameState->FindTeamForPlayer(PersistentID);
		PlayerController->PlayerType = PlayerController->ControllerTeam->CurrentPlayerData.FindRef(PersistentID).SelectedRole;
		PlayerController->ShowHeroSelectWidget();
	}
	else {

	}
}

void ABattlefieldGameMode::StartMatch()
{
	Super::StartMatch();
	UE_LOG(LogTemp, Warning, TEXT("BattlefieldGameMode::StartMatch - Starting Match"));
}

void ABattlefieldGameMode::InitTeam(uint8 InIndex, FMainMenuTeamStruct InTeamStruct)
{
	UE_LOG(LogTemp, Warning, TEXT("BattlefieldGameMode::InitTeam - Initializing New Team"));
	CurrentGameInstance = Cast<UPlayableGameInstance>(GetGameInstance());
	ABattlefieldGameState* CurrentGameState = Cast<ABattlefieldGameState>(UGameplayStatics::GetGameState(GetWorld()));
	UTeamComponent* NewTeam = NewObject<UTeamComponent>(this, UTeamComponent::StaticClass());
	NewTeam->TeamName = InTeamStruct.SelectedFactionData->FactionName.ToString();
	NewTeam->TeamIndex = InIndex;
	NewTeam->FactionData = InTeamStruct.SelectedFactionData;
	NewTeam->CurrentPlayerData = InTeamStruct.CurrentPlayerData;
	float ChangeFloat = 0.f;
	uint8 Sentinel = 0;
	while (NewTeam->PlayerControllerArray.Num() <= 8) {
		if (Sentinel > 50) break;
		FVector Vec = { FMath::FRandRange(0.f + ChangeFloat, 2000.f + ChangeFloat), FMath::FRandRange(0.f + ChangeFloat, 2000.f + ChangeFloat), 250.0f };
		FRotator Rot = { 0, 0, 0 };
		APlayableCharacter* AIChar = NewTeam->SpawnTeamCharacter(Vec, Rot, 0);
		if (CurrentGameModeData) {
			UE_LOG(LogTemp, Warning, TEXT("BattlefieldGameMode::InitTeam - CurrentGameModeData OK"));
			AIChar->AIControllerClass = CurrentGameModeData->AIData->AIControllerClass;
			AIChar->SpawnDefaultController();
			Cast<ABattlefieldAIController>(AIChar->GetController())->AssignData(CurrentGameModeData->AIData);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("BattlefieldGameMode::InitTeam - CurrentGameModeData NULL"));
			AIChar->AIControllerClass = ABattlefieldAIController::StaticClass();
			AIChar->SpawnDefaultController();
		}		
		AIChar->SetOwnerTeam(NewTeam);
		Sentinel++;
	}
	ChangeFloat += 5000.f;
	CurrentGameState->AddTeam(NewTeam);
}
