#include "GameplayUtils.h"
#include "CombatActor.h"
#include "CombatComponent.h"
#include "StatsComponent.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "PlayableCharacter.h"

UGameplayUtils::UGameplayUtils() {
	static::ConstructorHelpers::FObjectFinder<UDataTable>InputCharacterDataTable(TEXT("DataTable'/Game/Data/DataTables/CharacterData.CharacterData'"));
	if (InputCharacterDataTable.Succeeded()) {
		CharacterDataTable = InputCharacterDataTable.Object;
	}

	static::ConstructorHelpers::FObjectFinder<UDataTable>InputStatsTable(TEXT("DataTable'/Game/Data/DataTables/PawnStatsData.PawnStatsData'"));
	if (InputStatsTable.Succeeded()) {
		PawnStatsDataTable = InputStatsTable.Object;
	}

	static::ConstructorHelpers::FObjectFinder<UDataTable>InputPawnGraphicsTable(TEXT("DataTable'/Game/Data/DataTables/PawnGraphicsData.PawnGraphicsData'"));
	if (InputPawnGraphicsTable.Succeeded()) {
		PawnGraphicsDataTable = InputPawnGraphicsTable.Object;
	}

	static::ConstructorHelpers::FObjectFinder<UDataTable>InputPlayerCameraTable(TEXT("DataTable'/Game/Data/DataTables/PlayerCameraData.PlayerCameraData'"));
	if (InputPlayerCameraTable.Succeeded()) {
		PlayerCameraDataTable = InputPlayerCameraTable.Object;
	}

	static::ConstructorHelpers::FObjectFinder<UDataTable>InputGameModeDataTable(TEXT("DataTable'/Game/Data/DataTables/GameModeData.GameModeData'"));
	if (InputGameModeDataTable.Succeeded()) {
		GameModeDataTable = InputGameModeDataTable.Object;
	}

	static::ConstructorHelpers::FObjectFinder<UDataTable>InputRaceDataTable(TEXT("DataTable'/Game/Data/DataTables/RaceData.RaceData'"));
	if (InputRaceDataTable.Succeeded()) {
		RaceDataTable = InputRaceDataTable.Object;
	}

	static::ConstructorHelpers::FObjectFinder<UDataTable>InputCombatActorDataTable(TEXT("DataTable'/Game/Data/DataTables/CombatActorData.CombatActorData'"));
	if (InputCombatActorDataTable.Succeeded()) {
		CombatActorDataTable = InputCombatActorDataTable.Object;
	}
}

FCharacterData* UGameplayUtils::RetrieveCharacterDataRow(FName InKey) {
	FCharacterData* CharData = UGameplayUtils::CharacterDataTable->FindRow<FCharacterData>(InKey, "Finding Character Data");
	return CharData;
}

FPlayerCameraData* UGameplayUtils::RetrieveCameraDataRow(FName InKey) {
	FPlayerCameraData* CamData = UGameplayUtils::PlayerCameraDataTable->FindRow<FPlayerCameraData>(InKey, "Finding Character Data");
	return CamData;
}

FPawnGraphicsData* UGameplayUtils::RetrievePawnGraphicsDataRow(FName InKey) {
	FPawnGraphicsData* PawnGraphicsData = UGameplayUtils::PawnGraphicsDataTable->FindRow<FPawnGraphicsData>(InKey, "Finding Character Data");
	return PawnGraphicsData;
}

FPawnStatsData* UGameplayUtils::RetrievePawnStatsDataRow(FName InKey) {
	FPawnStatsData*  PawnStatsData = UGameplayUtils::PawnStatsDataTable->FindRow<FPawnStatsData>(InKey, "Finding Character Data");
	return PawnStatsData;
}

void UGameplayUtils::LoadDataTables() {


}