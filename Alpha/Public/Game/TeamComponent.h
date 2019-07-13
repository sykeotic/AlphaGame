#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HumanPlayerController.h"
#include "PlayablePawn.h"
#include "ObjectiveOverlapActor.h"
#include "SpawnLocationActor.h"
#include "TeamComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALPHA_API UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTeamComponent();

protected:
	virtual void BeginPlay() override;

public:
	TArray<AHumanPlayerController*> HumanPlayerControllerArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<APlayableCharacter*> TeamHeroes;
	TArray<APlayablePawn*> TeamPawns;
	TArray<ASpawnLocationActor*> TeamSpawnLocations;
	TArray<class AObjectiveOverlapActor*> OwnedObjectives;

	UPROPERTY(EditDefaultsOnly)
	TArray < TSubclassOf<class APlayableCharacter >> PlayableCharacterClassArray;

	UPROPERTY(EditDefaultsOnly)
	TArray < TSubclassOf<class APlayablePawn >> PlayablePawnClassArray;

	void SpawnTeamPawn();
	void SpawnTeamCharacter(TSubclassOf<class APlayableCharacter> CharClass, FVector SpawnLocation, FRotator SpawnRotation);
		
};
