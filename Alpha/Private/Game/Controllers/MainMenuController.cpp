#include "Game/Controllers/MainMenuController.h"
#include "Blueprint/UserWidget.h"

void AMainMenuController::SetCurrentWidget(TSubclassOf<UUserWidget> InWidget)
{
	CurrentWidget = CreateWidget<UUserWidget>(this, InWidget);
	CurrentWidget->AddToViewport();
	bShowMouseCursor = true;
}
