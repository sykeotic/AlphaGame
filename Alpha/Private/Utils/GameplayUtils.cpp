#include "GameplayUtils.h"
#include "CombatActor.h"
#include "CombatComponent.h"
#include "StatsComponent.h"
#include "PlayableCharacter.h"

void UGameplayUtils::ConstructPlayableCharacter(APlayableCharacter* InChar, FCharacterData* InRow) {
	InChar->SetBaseTurnRate(InRow->CameraData.BaseTurnRate);
	InChar->SetBaseLookUpRate(InRow->CameraData.BaseLookupRate);
	InChar->bUseControllerRotationPitch = InRow->CameraData.bUseControllerPitch;
	InChar->bUseControllerRotationRoll = InRow->CameraData.bUseControllerRoll;
	InChar->bUseControllerRotationYaw = InRow->CameraData.bUseControllerYaw;
	InChar->GetCameraSpringArm()->bEnableCameraLag = true;
	InChar->GetCameraSpringArm()->bUsePawnControlRotation = InRow->CameraData.bUsePawnControlRotation;
	InChar->GetCameraSpringArm()->TargetArmLength = InRow->CameraData.BoomArmLength;
	InChar->GetCameraSpringArm()->SetRelativeTransform(InRow->CameraData.RelTransform);
	InChar->GetCharacterMovement()->bOrientRotationToMovement = true;
	InChar->GetCharacterMovement()->JumpZVelocity = InRow->JumpVelocity;
	InChar->GetCharacterMovement()->RotationRate = InRow->RotationRate;
	InChar->GetCharacterMovement()->MaxWalkSpeed = InRow->MoveSpeed;
	InChar->GetCapsuleComponent()->InitCapsuleSize(InRow->GraphicsData.CapsuleRadius, InRow->GraphicsData.CapsuleHeight);
	InChar->GetMesh()->SetRelativeLocation(InRow->GraphicsData.MeshRotation);
	InChar->GetMesh()->SetSkeletalMesh(InRow->GraphicsData.SkeletalMesh);
	InChar->GetMesh()->SetMaterial(0, InRow->GraphicsData.Material_0);
	InChar->GetMesh()->SetMaterial(1, InRow->GraphicsData.Material_1);
	InChar->GetDecal()->DecalSize = InRow->GraphicsData.DecalSize;
	InChar->GetDecal()->SetRelativeRotation(InRow->GraphicsData.DecalRotation.Quaternion());
	InChar->GetDecal()->SetVisibility(false);
	InChar->GetDecal()->SetDecalMaterial(InRow->GraphicsData.DecalMaterial);
}

void UGameplayUtils::ConstructCombatComponent(UCombatComponent* InComp, FCharacterData* InRow) {

}

void UGameplayUtils::ConstructStatsComponent(UStatsComponent* InStats, FPawnStatsData* InRow) {

}

void UGameplayUtils::ConstructCombatActor(ACombatActor* InActor, FCombatActorData* InRow) {

}