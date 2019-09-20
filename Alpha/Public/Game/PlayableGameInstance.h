#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Data/Game/GameInstanceData.h"
#include "HumanPlayerController.h"
#include "PlayableGameInstance.generated.h"

class UFactionData;



UCLASS(BlueprintType)
class ALPHA_API UPlayableGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		uint8 CurrentTeamIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EPlayerType Role;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FName CurrentFactionName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UGameInstanceData* GameData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UFactionData* SelectedFaction;

	UFUNCTION(BlueprintCallable)
		void AssignData(UGameInstanceData* InData);
};