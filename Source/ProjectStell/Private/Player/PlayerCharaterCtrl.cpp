// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharaterCtrl.h"

#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterAnim.h"
#include "Stat/PlayerStat.h"


#include "UI/GamePauseMenuWidget.h"
#include "UI/GameClearMenuWidget.h"
#include "UI/GameOverMenuWidget.h"
#include "UI/CharacterHUDWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/ItemSlotWidget.h"


#include <Petten/Command.h>



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
void APlayerCharaterCtrl::SetupInputComponent()
{
	Super::SetupInputComponent();
	/**/
	InputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharaterCtrl::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharaterCtrl::MoveRight);
	InputComponent->BindAction(TEXT("Evasion"), EInputEvent::IE_Pressed, this, &APlayerCharaterCtrl::Evasion);
	InputComponent->BindAction(TEXT("LeftAttack"), EInputEvent::IE_Pressed, this, &APlayerCharaterCtrl::LeftAttack);
	InputComponent->BindAction(TEXT("RightAttack"), EInputEvent::IE_Pressed, this, &APlayerCharaterCtrl::RightAttack);
	InputComponent->BindAction(TEXT("Equipment_Left"), EInputEvent::IE_Pressed, this, &APlayerCharaterCtrl::LeftEquipment);
	InputComponent->BindAction(TEXT("Equipment_Right"), EInputEvent::IE_Pressed, this, &APlayerCharaterCtrl::RightEquipment);
	
	InputComponent->BindAction(TEXT("GamePause"),EInputEvent::IE_Pressed,this, &APlayerCharaterCtrl::ShowUI_GamePause);
	InputComponent->BindAction(TEXT("OpenInventory"), EInputEvent::IE_Pressed, this, &APlayerCharaterCtrl::ShowUI_Inventory);

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
void APlayerCharaterCtrl::ShowUI_GamePause()
{
/*
ClearWidget = CreateWidget<UGameClearMenuWidget>(this, ClearWidgetClass);
if (nullptr == ClearWidget)return;
ClearWidget->AddToViewport();
ChangeInputMode(1);
*/
	ShowGUICommand showGUI = ShowGUICommand(this, 0);
	showGUI.Execute();
}
void APlayerCharaterCtrl::ShowUI_GameClear()
{
	ShowGUICommand showGUI = ShowGUICommand(this, 1);
	showGUI.Execute();
}
void APlayerCharaterCtrl::ShowUI_GameOver()
{
	ShowGUICommand showGUI = ShowGUICommand(this, 2);
	showGUI.Execute();
}
void APlayerCharaterCtrl::ShowUI_Inventory() //인벤 보여주기 인벤 숨기기 기능이 합쳐진 함수임으로 개편이 필요 
{
	ShowGUICommand showGUI = ShowGUICommand(this, 3);
	showGUI.Execute();
}

void APlayerCharaterCtrl::MoveForward(float newAxis)
{
	MoveCommand move = MoveCommand(GetCharacter(), newAxis, true);
	move.Execute();
}
void APlayerCharaterCtrl::MoveRight(float newAxis)
{
	MoveCommand move = MoveCommand(GetCharacter(), newAxis, false);
	move.Execute();
}
void APlayerCharaterCtrl::Evasion()
{
	EvasionCommand evasion = EvasionCommand(GetCharacter());
	evasion.Execute();
}
void APlayerCharaterCtrl::LeftAttack()
{
	AttackCommand attack = AttackCommand(GetCharacter(), true);
	attack.Execute();
}
void APlayerCharaterCtrl::RightAttack()
{
	AttackCommand attack = AttackCommand(GetCharacter(),false);
	attack.Execute();
}
void APlayerCharaterCtrl::LeftEquipment()
{
	EquipmentCommand equipment = EquipmentCommand(GetCharacter(), true);
	equipment.Execute();
}
void APlayerCharaterCtrl::RightEquipment()
{
	EquipmentCommand equipment = EquipmentCommand(GetCharacter(), false);
	equipment.Execute();
}

//백업용 코드
/*
void APlayerCharaterCtrl::OpenMainMenu()
{
	ShowGUICommand showGUI = ShowGUICommand(GetCharacter(),0);
	showGUI.Execute();

	MenuWidget = CreateWidget<UGamePauseMenuWidget>(this, MenuWidgetClass);
	if (nullptr == MenuWidget)return;
	MenuWidget->AddToViewport(3);
	SetPause(true);
	ChangeInputMode(1);
}
void APlayerCharaterCtrl::OpenInventory()
{
	ShowGUICommand showGUI = ShowGUICommand(GetCharacter(), 1);
	showGUI.Execute();

	if (isInvenopen)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		ChangeInputMode(0);
		isInvenopen = false;
	}
	else
	{
		if (nullptr == InventoryWidget)return;
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		ChangeInputMode(2);
		isInvenopen = true;
	}
}
*/