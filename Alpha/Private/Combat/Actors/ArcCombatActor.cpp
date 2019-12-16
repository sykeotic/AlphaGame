#include "ArcCombatActor.h"
#include "BaseCombatActor.h"
#include "PlayableCharacter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "CombatComponent.h"
#include "Logger.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

AArcCombatActor::AArcCombatActor()
{

}

void AArcCombatActor::OnUse()
{
	Super::OnUse();
}

void AArcCombatActor::ExecuteUse()
{
	ArcSweep();
}

void AArcCombatActor::AssignValues(UBaseCombatActorData* InData)
{
	Super::AssignValues(InData);
	UArcCombatActorData* TempData = Cast<UArcCombatActorData>(InData);
	ArcCombatActorDataStruct = TempData->ArcCombatActorDataStruct;
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
		ULogger::ScreenMessage(FColor::Green, "Sweep Done");
		if (SphereCollision) {
			ULogger::ScreenMessage(FColor::Red, "SOMEONE WAS HIT WITH FIERY BREATH OF DOOM");
			for (FHitResult HitResult : HitResults) {
				ApplyModifiers(HitResult.Actor.Get());
			}
		}
		else {
			ULogger::ScreenMessage(FColor::Green, "No one was hit :(");
		}
	}
}
