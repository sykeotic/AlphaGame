#include "CombatUtils.h"
#include "CombatActor.h"
#include "Logger.h"
#include "PlayableCharacter.h"

float UCombatUtils::CalculateDamage(float InDamage, APlayableCharacter* DefendingCharacter, APlayableCharacter* AttackingCharacter, ACombatActor* AttackingCombatActor) {
	float TempDamage = InDamage;
	ULogger::ScreenMessage(FColor::Blue, "Temp Damage: " + FString::SanitizeFloat(TempDamage));
	TempDamage = ResolveAttackModifier(AttackingCharacter, TempDamage);
	ULogger::ScreenMessage(FColor::Blue, "Attack Modifier Damage: " + FString::SanitizeFloat(TempDamage));
	TempDamage = ResolveDefenseModifier(DefendingCharacter, TempDamage);
	ULogger::ScreenMessage(FColor::Blue, "Defense Modifier Damage: " + FString::SanitizeFloat(TempDamage));
	TempDamage = ResolveArmorModifier(TempDamage, DefendingCharacter, AttackingCharacter, AttackingCombatActor);
	ULogger::ScreenMessage(FColor::Blue, "Armor Modifier Damage: " + FString::SanitizeFloat(TempDamage));
	return TempDamage;
}

float UCombatUtils::ResolveAttackModifier(APlayableCharacter* InChar, float InDamage) {
	float TempDamage = InDamage;
	return TempDamage;
}

float UCombatUtils::ResolveDefenseModifier(APlayableCharacter* InChar, float InDamage) {
	float TempDamage = InDamage;
	return TempDamage;
}

float UCombatUtils::ResolveArmorModifier(float InDamage, APlayableCharacter* DefendingCharacter, APlayableCharacter* OffensivePlayer, ACombatActor* AttackingCombatActor) {
	return ResolveReducedDamageFromArmor(InDamage, AttackingCombatActor->ArmorPierce, DefendingCharacter);
}

float UCombatUtils::ResolveReducedDamageFromArmor(float InDamage, float ArmorPierce, APlayableCharacter* DefendingCharacter) {
	float TempDamage = InDamage;
	ULogger::ScreenMessage(FColor::Blue, "ResolveReducedDamaged TempDamage Before: " + FString::SanitizeFloat(TempDamage));
	EArmorType ArmorType = DefendingCharacter->GetStatsComponent()->GetArmorType();
	switch (ArmorType)
	{
		case EArmorType::NO_ARMOR: {
			ULogger::ScreenMessage(FColor::Green, "NO ARMOR");
			TempDamage *= ResolveArmorPierce(100.0f, ArmorPierce);
		}
		break;
		case EArmorType::LIGHT_ARMOR: {
			ULogger::ScreenMessage(FColor::Green, "LIGHT ARMOR");
			TempDamage *= ResolveArmorPierce(90.0f, ArmorPierce);
		}
		break;
		case EArmorType::MEDIUM_ARMOR: {
			ULogger::ScreenMessage(FColor::Green, "MEDIUM ARMOR");
			TempDamage *= ResolveArmorPierce(75.0f, ArmorPierce);
		}
		break;
		case EArmorType::HEAVY_ARMOR: {
			ULogger::ScreenMessage(FColor::Green, "HEAVY  ARMOR");
			TempDamage *= ResolveArmorPierce(50.0f, ArmorPierce);
		}
		break;
		case EArmorType::INVULNERABLE: {
			ULogger::ScreenMessage(FColor::Green, "INVULNERABLE");
			TempDamage *= 0.0f;
		}
	}
	ULogger::ScreenMessage(FColor::Blue, "ResolveReducedDamaged TempDamage After: " + FString::SanitizeFloat(TempDamage));
	return TempDamage;
}

float UCombatUtils::ResolveArmorPierce(float BaseModifier, float ArmorPierce) {
	float ModifiedDamage = (BaseModifier + (BaseModifier * (ArmorPierce / 100.f))) / 100.0f;
	ULogger::ScreenMessage(FColor::Blue, "Armor Pierce Damage: " + FString::SanitizeFloat(ModifiedDamage));
	return ModifiedDamage >= 1.0f ? 1.0f : ModifiedDamage;
}