// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharaterCtrl.h"
#include "UI/GamePauseMenuWidget.h"

APlayerCharaterCtrl::APlayerCharaterCtrl()
{
	static ConstructorHelpers::FClassFinder<UGamePauseMenuWidget> MENU(TEXT("WidgetBlueprint'/Game/1_UI/UI_Menu.UI_Menu_C'"));
	if (MENU.Succeeded())
	{
		MenuWidgetClass = MENU.Class;
	}
}
void APlayerCharaterCtrl::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
void APlayerCharaterCtrl::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}
void APlayerCharaterCtrl::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"),EInputEvent::IE_Pressed,this,&APlayerCharaterCtrl::GamePause);
}
void APlayerCharaterCtrl::BeginPlay()
{
	Super::BeginPlay();
	ChangeInputMode(true);
}
void APlayerCharaterCtrl::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}
void APlayerCharaterCtrl::GamePause()
{
	MenuWidget = CreateWidget<UGamePauseMenuWidget>(this, MenuWidgetClass);
	if(nullptr == MenuWidget)return;
	MenuWidget->AddToViewport(3);
	SetPause(true);
	ChangeInputMode(false);
}
