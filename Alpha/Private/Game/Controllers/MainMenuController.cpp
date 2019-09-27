#include "Game/Controllers/MainMenuController.h"
#include "Blueprint/UserWidget.h"

void AMainMenuController::SetTeamIndex(uint8 InTeamIndex)
{
	PlayerMenuStruct.SelectedTeamIndex = InTeamIndex;
}

void AMainMenuController::SetRole(EPlayerType InRole)
{
	PlayerMenuStruct.SelectedRole = InRole;
}

void AMainMenuController::SetCurrentWidget(TSubclassOf<UUserWidget> InWidget)
{
	CurrentWidget = CreateWidget<UUserWidget>(this, InWidget);
	CurrentWidget->AddToViewport();
	bShowMouseCursor = true;
}
