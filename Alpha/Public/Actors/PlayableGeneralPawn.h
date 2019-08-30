#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GeneralHUD.h"
#include "PlayableGeneralPawn.generated.h"

class AHumanPlayerController;
class UGeneralPawnMovement;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* GeneralCamera;

	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArm;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void LeftClickPressed();
	void LeftClickReleased();
	void RightClickReleased();

	void MoveForward(float Value);
	void MoveRight(float Value);

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	void TeardownInput();

		UPROPERTY(EditDefaultsOnly)
	UGeneralPawnMovement* PawnMovementComponent;

	void SetPlayerOwner(AHumanPlayerController* InController);

	AGeneralHUD* GetHUD();

	AHumanPlayerController* Owner;

	TArray<APlayableCharacter*> SelectedActors;
};
