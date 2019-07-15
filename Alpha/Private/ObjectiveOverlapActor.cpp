#include "ObjectiveOverlapActor.h"
#include "PlayableCharacter.h"
#include "CombatActor.h"
#include "TestGameState.h"
#include "CombatProjectile.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Logger.h"

AObjectiveOverlapActor::AObjectiveOverlapActor() {
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	RequiredCaptureScore = 0;
}

void AObjectiveOverlapActor::BeginPlay(){
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
	if ((OtherActor != nullptr) && (OtherComp != nullptr) && (OtherActor != this) && (OtherActor->GetClass()->IsChildOf(APlayablePawn::StaticClass()) || OtherActor->GetClass()->IsChildOf(APlayableCharacter::StaticClass())))
	{
		if (!InZoneActors.Contains(OtherActor)) {
			InZoneActors.AddUnique(OtherActor);
			if (ContestingTeam == NULL || ContestingTeam == nullptr)
				ContestingTeam = Cast<APlayableCharacter>(OtherActor)->OwnerTeam;
			if(OwningTeam != Cast<APlayableCharacter>(OtherActor)->OwnerTeam)
				AssertObjectiveState();
		}
	}
}

void AObjectiveOverlapActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if ((OtherActor != nullptr) && (OtherComp != nullptr) && (OtherActor != this) && (OtherActor->GetClass()->IsChildOf(APlayablePawn::StaticClass()) || OtherActor->GetClass()->IsChildOf(APlayableCharacter::StaticClass())))
	{
		if (InZoneActors.Contains(OtherActor)) {
			InZoneActors.Remove(OtherActor);
			if (OwningTeam != Cast<APlayableCharacter>(OtherActor)->OwnerTeam)
				AssertObjectiveState();
		}
	}
}

void AObjectiveOverlapActor::AssertObjectiveState() {
	if (InZoneActors.Num() > 0) {
		AdjustModifier();
		if (OBJECTIVE_STATE == EObjectiveState::NEUTRAL) {
			ULogger::ScreenMessage(FColor::Blue, "Neutral Obj");
			StartCapturing();
		}
		else if (OBJECTIVE_STATE == EObjectiveState::CAPTURED) {
			ULogger::ScreenMessage(FColor::Blue, "Captured Obj");
			for (AActor* CurrActor : InZoneActors) {
				APlayableCharacter* CurrChar = Cast<APlayableCharacter>(CurrActor);
				if (CurrChar->IsValidLowLevel()) {
					if (OwningTeam != CurrChar->OwnerTeam) {
						ULogger::ScreenMessage(FColor::Blue, "Char valid, Captured Obj");
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
	}
	else {
		ULogger::ScreenMessage(FColor::Blue, "Resetting Obj");
		ResetObjective();
	}
}

void AObjectiveOverlapActor::StartCapturing() {
	ChangeState(EObjectiveState::CAPTURING);
	APlayableCharacter* FirstChar = Cast<APlayableCharacter>(InZoneActors.Last());
	CreateCaptureDisplay();
	ULogger::ScreenMessage(FColor::Blue, "StartCapturing()");
	if (FirstChar) {
		ContestingTeam = FirstChar->OwnerTeam;
		CurrentCaptureScore += CaptureModifier;
		ULogger::ScreenMessage(FColor::Blue, "Starting to Capture");
		GetWorldTimerManager().SetTimer(CaptureTimer, this, &AObjectiveOverlapActor::TimerTick, .1f, false);
	}
	else {
		ULogger::ScreenMessage(FColor::Blue, "First Char not valid");
	}
}

void AObjectiveOverlapActor::TimerTick() {
	if (CurrentCaptureScore <= 0 && bResetting) {
		ResetObjectiveFinished();
	}
	else if (CurrentCaptureScore >= RequiredCaptureScore && !bResetting) {
		ULogger::ScreenMessage(FColor::Yellow, "Required Score: " + FString::FromInt(RequiredCaptureScore));
		ULogger::ScreenMessage(FColor::Yellow, "Current Score: " + FString::FromInt(CurrentCaptureScore));
		HandleCapture();
	}
	else {
		CurrentCaptureScore += CaptureModifier;
		PercentProgress = CurrentCaptureScore / RequiredCaptureScore;
		if (CurrentCaptureScore <= 0)
			CurrentCaptureScore = 0;
		if (!bResetting) {
			ULogger::ScreenMessage(FColor::Green, "Capture Timer");
			GetWorldTimerManager().SetTimer(CaptureTimer, this, &AObjectiveOverlapActor::TimerTick, .1f, false);
		}
		else {
			ULogger::ScreenMessage(FColor::Green,  "Reset Timer");
			GetWorldTimerManager().SetTimer(ResetTimer, this, &AObjectiveOverlapActor::TimerTick, .1f, false);
		}
	}
	ULogger::ScreenMessage(FColor::Green, "Timer Tick: " + FString::FromInt(CurrentCaptureScore));
}

void AObjectiveOverlapActor::AdjustModifier() {
	uint32 NumHeroes = 0;
	uint32 NumPawns = 0;
	for (AActor* CurrActor : InZoneActors) {
		APlayableCharacter* CurrChar = Cast<APlayableCharacter>(CurrActor);
		if (CurrChar) {
			if (CurrChar->OwnerTeam == ContestingTeam) {
				NumHeroes++;
			}
			else {
				NumHeroes--;
			}
		} 
		else {
			APlayablePawn* CurrPawn = Cast<APlayablePawn>(CurrActor);
			if (CurrPawn->IsValidLowLevel()) {
				if (CurrPawn->OwnerTeam == ContestingTeam) {
					NumPawns++;
				}
				else {
					NumPawns--;
				}
			}
		}
	}
	if (NumHeroes == 0 && NumPawns == 0) {
		CaptureModifier = -5;
	}
	else {
		CaptureModifier = (NumHeroes * 5) + (NumPawns);
	}
	ULogger::ScreenMessage(FColor::Red, "Adjust Modifier: " + FString::FromInt(CaptureModifier));
}

void AObjectiveOverlapActor::HandleCapture() {
	ChangeState(EObjectiveState::CAPTURED);
	OwningTeam = ContestingTeam;
	ResetObjectiveFinished();
	PlayActorSound(CapturedSound);
	ULogger::ScreenMessage(FColor::Yellow, "Objective Captured");
	ATestGameState* CurrGameState = Cast<ATestGameState>(GetWorld()->GetGameState());
	CurrGameState->ObjectiveCaptured(OwningTeam, this);
}

void AObjectiveOverlapActor::ResetObjective() {
	AdjustModifier();
	bResetting = true;
	GetWorldTimerManager().SetTimer(ResetTimer, this, &AObjectiveOverlapActor::TimerTick, .1f, false);
}

void AObjectiveOverlapActor::ResetObjectiveFinished() {
	ContestingTeam = nullptr;
	ChangeState(PREV_STATE);
	DestroyCaptureDisplay();
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
		AC = UGameplayStatics::SpawnSoundAtLocation(MeshComp, SoundToPlay, MeshComp->GetComponentLocation(), MeshComp->GetComponentRotation());
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