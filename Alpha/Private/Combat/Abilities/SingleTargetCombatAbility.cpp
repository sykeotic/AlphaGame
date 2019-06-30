#include "SingleTargetCombatAbility.h"
#include "CombatComponent.h"
#include "Logger.h"
#include "PlayableCharacter.h"

ASingleTargetCombatAbility::ASingleTargetCombatAbility()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASingleTargetCombatAbility::OnUse() {
	Super::OnUse();
	ULogger::ScreenMessage(FColor::Red, "ASTCA Use");
	/*
	Super::OnUse();
	FRotator OwnerView;
	FVector OwnerLoc;
	ComponentOwner->GetOwner()->GetActorEyesViewPoint(OwnerLoc, OwnerView);
	FVector From = ComponentOwner->CharacterOwner->GetMesh()->GetSocketLocation(ActorSocketLocation);
	FVector TraceEnd = From + (OwnerView.Vector() * UseRange);
	UTimelineComponent Timeline;
	Timeline = NewObject<UTimelineComponent>(this, FName("TimelineAnimation"));
	FMath::Lerp(From, TraceEnd, )
	*/
	Super::OnUse();
	FRotator RotationFrom;
	FVector OwnerLoc;
	ComponentOwner->GetOwner()->GetActorEyesViewPoint(OwnerLoc, RotationFrom);
	FVector LocationFrom = ComponentOwner->CharacterOwner->GetMesh()->GetSocketLocation(ActorSocketLocation);
	FVector TraceEnd = LocationFrom + (RotationFrom.Vector() * UseRange);
	DetachMeshFromOwner();
	MeshComp->DetachFromParent();
	FireObjectInDirection(LocationFrom, RotationFrom, TraceEnd);
}

