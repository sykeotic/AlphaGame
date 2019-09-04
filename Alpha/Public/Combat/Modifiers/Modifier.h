#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "ModifierData.h"
#include "Modifier.generated.h"

class UConditionTree;

UENUM(BlueprintType)
enum class EFeedbackType : uint8 {
	ALIVE UMETA(DisplayName = "Alive"),
	ACTIVE UMETA(DisplayName = "Active"),
	EFFECTS_APPLIED UMETA(DisplayName = "Effects Applied")
};

USTRUCT(BlueprintType)
struct FContext {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bHasDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Duration;

	UConditionTree* Conditions;
};

class APlayableCharacter;

UCLASS()
class ALPHA_API AModifier : public AActor
{
	GENERATED_BODY()
	
public:	
	AModifier();

	bool AreConditionsTrue();
	bool IsActive();
	void SetIsActive(bool bActiveStatus);

	FContext GetContext();

	void ApplyEffects();

	void AssignValues(FModifierDataStruct InData);

	FModifierDataStruct ModifierData;

protected:
	virtual void BeginPlay() override;

private:
	FContext Context;

	EFeedbackType FeedbackType;
};
