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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UConditionTree* Conditions;
};

class APlayableCharacter;
class ABaseEffect;

UCLASS()
class ALPHA_API AModifier : public AActor
{
	GENERATED_BODY()
	
public:	
	AModifier();

	bool AreConditionsTrue();
	UConditionTree* GetConditionTree();
	bool IsActive();
	void SetIsActive(bool bActiveStatus);

	FContext GetContext();
	int32 GetEffectCount();

	void SetActorOwner(AActor* InActor);
	AActor* GetActorOwner();

	void SetOriginatingActor(AActor* InActor);
	AActor* GetOriginatingActor();

	void ApplyEffects(AActor* AffectedActor);

	void AssignValues(FModifierDataStruct InData);

	void Deactivate(AActor* InActor);

	FModifierDataStruct ModifierData;

protected:
	virtual void BeginPlay() override;

private:
	FContext Context;
	AActor* ActorOwner;
	AActor* OriginatingActor;
	EFeedbackType FeedbackType;
	TArray<ABaseEffect*> Effects;
};
