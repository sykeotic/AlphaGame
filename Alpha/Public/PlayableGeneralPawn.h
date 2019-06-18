#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayableGeneralPawn.generated.h"

UCLASS()
class ALPHA_API APlayableGeneralPawn : public APawn
{
	GENERATED_BODY()

public:
	APlayableGeneralPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable, Category = "Game")
	//void AssignCameraValues(bool bUseYaw, bool bUsePitch, bool bUseRoll, float BoomArmLength, bool bInUseControlRotation, FTransform RelTransform);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* GeneralCamera;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
