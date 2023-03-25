#include "Player/UICtrl.h"
#include "Blueprint/UserWidget.h"


void AUICtrl::BeginPlay()
{
	Super::BeginPlay();
	if(nullptr == UiWidgetClass)return;
	UiWidgetInstance = CreateWidget<UUserWidget>(this, UiWidgetClass);
	if(nullptr == UiWidgetInstance)return;
	UiWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UiWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}


