#include "ArcCombatActor.h"
#include "BaseCombatActor.h"
#include "PlayableCharacter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "CombatComponent.h"
#include "Logger.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/StaticMeshActor.h"
#include "Data/Combat/BaseCombatActorData.h"
#include "CombatComponent.h"
#include "Actors/Characters/PlayableCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AArcCombatActor::AArcCombatActor()
{

}

void AArcCombatActor::OnUse()
{
	Super::OnUse();
	MeshComp->SetVisibility(true);
}

void AArcCombatActor::ExecuteUse()
{
	ArcSweep();
	FTimerHandle HideMeshTimer;
	if(!BaseCombatActorData.AlwaysDisplayMesh)
		GetWorldTimerManager().SetTimer(HideMeshTimer, this, &AArcCombatActor::SetMeshToInvisible, 1.5f, false);
}  

void AArcCombatActor::AssignValues(UBaseCombatActorData* InData)
{
	Super::AssignValues(InData);
	UArcCombatActorData* TempData = Cast<UArcCombatActorData>(InData);
	ArcCombatActorDataStruct = TempData->ArcCombatActorDataStruct;
}

void AArcCombatActor::PlayVisualFX()
{
	USkeletalMeshComponent* OwnerMesh = ComponentOwner->GetCharacterOwner()->GetMesh();
		if (MeshComp) {
			UsePSC = UGameplayStatics::SpawnEmitterAttached(BaseCombatActorData.Feedback->VisualFX, OwnerMesh, ArcCombatActorDataStruct.ArcSpawnFromSocket);
		}
}

void AArcCombatActor::ArcSweep()
{
	float NumCones = ArcCombatActorDataStruct.NumberOfCones;
	float ConeTraceArcWidth = ArcCombatActorDataStruct.ConeTraceArcWidthInDegrees;
	float SweepRadius = ArcCombatActorDataStruct.ConeSweepRadius;
	float DegreeIncrements = ConeTraceArcWidth / NumCones;
	float YawValueFirst = ConeTraceArcWidth * -.5;
	float ArcDistance = ArcCombatActorDataStruct.ArcRange;
	ULogger::ScreenMessage(FColor::Green, "Arc Sweeping");
	TArray<AActor*> AlreadyHitActors;
	for (int i = 0; i < NumCones; i++) {
		float YawValueSecond = DegreeIncrements * i;
		float YawFinal = YawValueFirst + YawValueSecond;
		FVector RotatedVector = ComponentOwner->GetCharacterOwner()->GetActorForwardVector().RotateAngleAxis(YawFinal, FVector(0, 0, 1)) * ArcDistance;
		FVector CharOwnerLocation = ComponentOwner->GetCharacterOwner()->GetActorLocation();
		FVector FinalVectorInput = RotatedVector + CharOwnerLocation;
		TArray<TEnumAsByte <EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(ComponentOwner->GetCharacterOwner());
		TArray<FHitResult> HitResults;
		bool SphereCollision = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), CharOwnerLocation, FinalVectorInput, SweepRadius, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResults, true);
		if (SphereCollision) {
			for (FHitResult HitResult : HitResults) {
				AActor* HitActor = HitResult.Actor.Get();
				if (!AlreadyHitActors.Contains(HitActor)) {
					ApplyModifiers(HitActor);
					AlreadyHitActors.Add(HitActor);
				}				
			}
		}
		else {
			ULogger::ScreenMessage(FColor::Green, "No one was hit :(");
		}
	}

}

void AArcCombatActor::SetMeshToInvisible()
{
	MeshComp->SetVisibility(false);
}
