#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HumanPlayerController.h"
#include "PlayablePawn.h"
#include "ObjectiveOverlapActor.h"
#include "SpawnLocationActor.h"
#include "MainMenuGameMode.h"
#include "TeamComponent.generated.h"

class UFactionData;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALPHA_API UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTeamComponent();

protected:
	virtual void BeginPlay() override;

public:
	TArray<AController*> PlayerControllerArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFactionData* FactionData;

	TMap<int32, FMainMenuPlayerStruct> CurrentPlayerData;
	TArray<APlayableCharacter*> TeamHeroes;
	TArray<APlayablePawn*> TeamPawns;
	TArray<ASpawnLocationActor*> TeamSpawnLocations;
	TArray<AObjectiveOverlapActor*> OwnedObjectives;

	uint8 TeamIndex;
	FString TeamName;

	void SpawnTeamPawn();
	
	APlayableCharacter* SpawnTeamCharacter(FVector SpawnLocation, FRotator SpawnRotation, uint8 HeroArrayIndex);
	void SetFactionData(UFactionData* InData);
		
};
