#include "ObjectiveOverlapActor.h"
#include "PlayableCharacter.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "PlayablePawn.h"
#include "FactionData.h"
#include "HumanPlayerController.h"
#include "PlayerControllerData.h"
#include "TeamComponent.h"
#include "Battlefield/BattlefieldGameState.h"
#include "Game/Modes/Battlefield/BattlefieldGameState.h"
#include "Components/SkeletalMeshComponent.h"
#include "Logger.h"

AObjectiveOverlapActor::AObjectiveOverlapActor() {
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = SkeletalMeshComp;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	RequiredCaptureScore = 0;
}

void AObjectiveOverlapActor::BeginPlay() {
	Super::BeginPlay();
	bResetting = false;
	CaptureModifier = 0;
	PercentProgress = 0.0f;
	OBJECTIVE_STATE = EObjectiveState::NEUTRAL;
	PREV_STATE = EObjectiveState::NEUTRAL;
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AObjectiveOverlapActor::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AObjectiveOverlapActor::OnOverlapEnd);
}

void AObjectiveOverlapActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherComp != nullptr) && (OtherActor != this) && (OtherActor->GetClass()->IsChildOf(APawn::StaticClass())))
	{
		if (!InZoneActors.Contains(OtherActor)) {
			InZoneActors.AddUnique(OtherActor);
			if (!ContestingTeam)
				ContestingTeam = Cast<APlayableCharacter>(OtherActor)->GetOwnerTeam();
			if (OwningTeam != Cast<APlayableCharacter>(OtherActor)->GetOwnerTeam() && ContestingTeam)
				AssertObjectiveState();
		}
	}
}

void AObjectiveOverlapActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if ((OtherActor != nullptr) && (OtherComp != nullptr) && (OtherActor != this) && (OtherActor->GetClass()->IsChildOf(APlayablePawn::StaticClass()) || OtherActor->GetClass()->IsChildOf(APlayableCharacter::StaticClass())))
	{
		if (InZoneActors.Contains(OtherActor)) {
			InZoneActors.Remove(OtherActor);
			if (OwningTeam != Cast<APlayableCharacter>(OtherActor)->GetOwnerTeam())
				AssertObjectiveState();
		}
	}
}

void AObjectiveOverlapActor::AssertObjectiveState() {
	if (InZoneActors.Num() > 0) {
		AdjustModifier();
		if (OBJECTIVE_STATE == EObjectiveState::NEUTRAL) {
			StartCapturing();
		}
		else if (OBJECTIVE_STATE == EObjectiveState::CAPTURED) {
			for (AActor* CurrActor : InZoneActors) {
				APlayableCharacter* CurrChar = Cast<APlayableCharacter>(CurrActor);
				if (CurrChar->IsValidLowLevel()) {
					if (OwningTeam != CurrChar->GetOwnerTeam()) {
						StartCapturing();
					}
				}
				else {
					APlayablePawn* CurrPawn = Cast<APlayablePawn>(CurrActor);
					if (CurrPawn->IsValidLowLevel()) {
						if (CurrPawn->OwnerTeam != OwningTeam) {
							StartCapturing();
						}
					}
				}
			}
		}
		else if (OBJECTIVE_STATE == EObjectiveState::CAPTURING) {
			AdjustModifier();
		}
	}
	else {
		ResetObjective();
	}
}

void AObjectiveOverlapActor::StartCapturing() {
	ChangeState(EObjectiveState::CAPTURING);
	APlayableCharacter* FirstChar = Cast<APlayableCharacter>(InZoneActors.Last());
	AHumanPlayerController* InController = Cast<AHumanPlayerController>(FirstChar->GetController());
	if (InController)
		CreateCaptureDisplay(InController);
	if (FirstChar) {
		ContestingTeam = FirstChar->GetOwnerTeam();
		CurrentCaptureScore += CaptureModifier;
		GetWorldTimerManager().SetTimer(CaptureTimer, this, &AObjectiveOverlapActor::TimerTick, .1f, false);
	}
	else {
		ULogger::ScreenMessage(FColor::Blue, "First Char not valid");
	}
}

void AObjectiveOverlapActor::TimerTick() {
	if (CurrentCaptureScore <= 0) {
		ResetObjectiveFinished();
	}
	else if (CurrentCaptureScore >= RequiredCaptureScore && !bResetting) {
		HandleCapture();
	}
	else {
		CurrentCaptureScore += CaptureModifier;
		PercentProgress = CurrentCaptureScore / RequiredCaptureScore;
		if (CurrentCaptureScore <= 0)
			CurrentCaptureScore = 0;
		if (!bResetting) {
			GetWorldTimerManager().SetTimer(CaptureTimer, this, &AObjectiveOverlapActor::TimerTick, .1f, false);
		}
		else {
			GetWorldTimerManager().SetTimer(ResetTimer, this, &AObjectiveOverlapActor::TimerTick, .1f, false);
		}
	}
}

void AObjectiveOverlapActor::AdjustModifier() {
	uint32 NumHeroes = 0;
	uint32 NumPawns = 0;
	APlayableCharacter* CurrChar = Cast<APlayableCharacter>(CurrActor);
	for (AActor* CurrActor : InZoneActors) {
		if (CurrChar->IsHero()) {
			if (CurrChar->GetOwnerTeam() == ContestingTeam) {
				NumHeroes++;
			}
			else {
				NumHeroes--;
			}
		}
		else {
			if (CurrChar->GetOwnerTeam() == ContestingTeam) {
				NumPawns++;
			}
			else {
				NumPawns--;
			}
		}
	}
	if (NumHeroes == 0 && NumPawns == 0) {
		CaptureModifier = -5;
	}
	else {
		CaptureModifier = (NumHeroes * 5) + (NumPawns);
	}
}

void AObjectiveOverlapActor::HandleCapture() {
	if (ContestingTeam->IsValidLowLevel()) {
		ChangeState(EObjectiveState::CAPTURED);
		OwningTeam->RemoveObjective(this);
		OwningTeam = ContestingTeam;
		OwningTeam->AddObjective(this);
		SkeletalMeshComp->SetSkeletalMesh(OwningTeam->FactionData->FactionFlagMesh);
		ResetObjectiveFinished();
		PlayActorSound(CapturedSound);
		ULogger::ScreenMessage(FColor::Yellow, "Objective Captured by " + OwningTeam->TeamName);
		ABattlefieldGameState* CurrGameState = Cast<ABattlefieldGameState>(GetWorld()->GetGameState());
		CurrGameState->ObjectiveCaptured(OwningTeam, this);
	}
}

void AObjectiveOverlapActor::CreateCaptureDisplay(AHumanPlayerController* InController)
{
	InController->CreateCastingWidget();
}

void AObjectiveOverlapActor::DestroyCaptureDisplay(AHumanPlayerController* InController)
{
	InController->RemoveCastingWidget();
}

void AObjectiveOverlapActor::ResetObjective() {
	AdjustModifier();
	bResetting = true;
	GetWorldTimerManager().SetTimer(ResetTimer, this, &AObjectiveOverlapActor::TimerTick, .1f, false);
}

void AObjectiveOverlapActor::ResetObjectiveFinished() {
	ContestingTeam = nullptr;
	ChangeState(EObjectiveState::CAPTURED);
	for (AActor* CurrActor : InZoneActors) {
		AHumanPlayerController* InController = Cast<AHumanPlayerController>(Cast<APlayableCharacter>(CurrActor)->GetController());
		if (InController)
			DestroyCaptureDisplay(InController);
	}
	CurrentCaptureScore = 0;
	PercentProgress = 0.0f;
	InZoneActors.Empty();
	bResetting = false;
	CaptureModifier = 0;
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

UAudioComponent* AObjectiveOverlapActor::PlayActorSound(USoundCue* SoundToPlay) {
	UAudioComponent* AC = nullptr;
	if (SoundToPlay)
	{
		AC = UGameplayStatics::SpawnSoundAtLocation(SkeletalMeshComp, SoundToPlay, SkeletalMeshComp->GetComponentLocation(), SkeletalMeshComp->GetComponentRotation());
	}
	else {
		ULogger::ScreenMessage(FColor::Red, "Null Sound");
	}

	return AC;
}

void AObjectiveOverlapActor::ChangeState(EObjectiveState InState) {
	PREV_STATE = OBJECTIVE_STATE;
	OBJECTIVE_STATE = InState;
}