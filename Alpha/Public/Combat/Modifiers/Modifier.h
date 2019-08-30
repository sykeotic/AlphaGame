#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Modifier.generated.h"

UENUM(BlueprintType)
enum class EFeedbackType : uint8 {
	ALIVE UMETA(DisplayName = "Alive"),
	ACTIVE UMETA(DisplayName = "Active"),
	EFFECTS_APPLIED UMETA(DisplayName = "Effects Applied")
};

USTRUCT(BlueprintType)
struct FFeedbacks
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<USoundCue*> SoundFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SoundFXBuffer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* VisualFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VisualFXBuffer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EFeedbackType FeedbackType;

	UAudioComponent* PlaySoundFX();
	USoundCue* PickRandomSoundCue();
	float PlayVisualFX();
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

	//Condition[]

	bool AreConditionsTrue();
	bool IsActive();
};

UCLASS()
class ALPHA_API AModifier : public AActor
{
	GENERATED_BODY()
	
public:	
	AModifier();

protected:
	virtual void BeginPlay() override;

public:	
};
