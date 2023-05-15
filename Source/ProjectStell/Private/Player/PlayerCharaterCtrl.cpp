// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharaterCtrl.h"
#include "UI/GamePauseMenuWidget.h"
#include "UI/GameClearMenuWidget.h"
#include "UI/GameOverMenuWidget.h"
#include "UI/CharacterHUDWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/ItemSlotWidget.h"



APlayerCharaterCtrl::APlayerCharaterCtrl()
{
	static ConstructorHelpers::FClassFinder<UGamePauseMenuWidget> MENU(TEXT("WidgetBlueprint'/Game/1_UI/UI_Menu.UI_Menu_C'"));
	if (MENU.Succeeded()) MenuWidgetClass = MENU.Class;

	static ConstructorHelpers::FClassFinder<UGameClearMenuWidget> UI(TEXT("WidgetBlueprint'/Game/1_UI/UI_Clear.UI_Clear_C'"));
	if (UI.Succeeded()) ClearWidgetClass = UI.Class;

	static ConstructorHelpers::FClassFinder<UGameOverMenuWidget> GameOverUI(TEXT("WidgetBlueprint'/Game/1_UI/UI_GameOver.UI_GameOver_C'"));
	if (GameOverUI.Succeeded()) GameOverWidgetClass = GameOverUI.Class;
	
	static ConstructorHelpers::FClassFinder<UInventoryWidget> invenUI(TEXT("WidgetBlueprint'/Game/1_UI/Inventory.Inventory_C'"));
	if (invenUI.Succeeded()) InventoryWidgetClass = invenUI.Class;

	static ConstructorHelpers::FClassFinder<UItemSlotWidget> itemSlotUI(TEXT("WidgetBlueprint'/Game/1_UI/InvenSlot.InvenSlot_C'"));
	if (itemSlotUI.Succeeded()) ItemSlotWidgetClass = itemSlotUI.Class;
	
	static ConstructorHelpers::FClassFinder<UCharacterHUDWidget> HUDUI(TEXT("WidgetBlueprint'/Game/1_UI/PlayerHUD.PlayerHUD_C'"));
	if (HUDUI.Succeeded()) HUDWidgetClass = HUDUI.Class;

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
	InputComponent->BindAction(TEXT("OpenInventory"), EInputEvent::IE_Pressed, this, &APlayerCharaterCtrl::ShowInventoryUI);
}
void APlayerCharaterCtrl::BeginPlay()
{
	Super::BeginPlay();
	ChangeInputMode(0);
	HUDWidget = CreateWidget<UCharacterHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();
	InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidgetClass);
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	//ShowInventoryUI();
	//ShowInventoryUI();
}
void APlayerCharaterCtrl::ChangeInputMode(int32 bGameMode)
{
	if (bGameMode==0)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else if (bGameMode == 1)
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
	else if (bGameMode == 2)
	{
		SetInputMode(GameAndUIInputMode);
		bShowMouseCursor = true;
	}
}
void APlayerCharaterCtrl::GamePause() 
{
	MenuWidget = CreateWidget<UGamePauseMenuWidget>(this, MenuWidgetClass);
	if(nullptr == MenuWidget)return;
	MenuWidget->AddToViewport(3);
	SetPause(true);
	ChangeInputMode(1);
}
void APlayerCharaterCtrl::GameClear()
{
	ClearWidget = CreateWidget<UGameClearMenuWidget>(this, ClearWidgetClass);
	if (nullptr == ClearWidget)return;
	ClearWidget->AddToViewport();
	ChangeInputMode(1);
}
void APlayerCharaterCtrl::GameOver()
{
	GameOverWidget = CreateWidget<UGameOverMenuWidget>(this, GameOverWidgetClass);
	if (nullptr == GameOverWidget)return;
	GameOverWidget->AddToViewport();
	ChangeInputMode(1);
}
void APlayerCharaterCtrl::ShowInventoryUI()
{
	if (isInvenopen)
	{
		//InventoryWidget->RemoveFromParent();
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		ChangeInputMode(0);
		isInvenopen = false;
	}
	else
	{
		//InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidgetClass);
		if (nullptr == InventoryWidget)return;
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		//InventoryWidget->AddToViewport();
		ChangeInputMode(2);
		isInvenopen = true;
	}
}

UCharacterHUDWidget* APlayerCharaterCtrl::GetHUDWidget() const
{
	return HUDWidget;
}

UInventoryWidget* APlayerCharaterCtrl::GetInventoryWidget() const
{
	return InventoryWidget;
}

UItemSlotWidget* APlayerCharaterCtrl::GetItemSlotWidget() const
{
	return ItemSlotWidget;
}
