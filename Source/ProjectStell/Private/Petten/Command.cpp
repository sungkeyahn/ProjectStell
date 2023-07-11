#include "Petten/Command.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharaterCtrl.h"

#include "UI/GamePauseMenuWidget.h"
#include "UI/GameClearMenuWidget.h"
#include "UI/GameOverMenuWidget.h"
#include "UI/CharacterHUDWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/ItemSlotWidget.h"


Command::Command(){}
Command::~Command(){}

void MoveCommand::Execute()
{
	APlayerCharacter* pc = Cast<APlayerCharacter>(_actor);
	if (_vectorIsForward)
		pc->UpDown(_newAxisValue);
	else
		pc->LeftRight(_newAxisValue);
}
void MoveCommand::Cancel()
{

}


void AttackCommand::Execute()
{
	APlayerCharacter* pc = Cast<APlayerCharacter>(_actor);
	if (_isLeftAttack)
		pc->LeftAttack();
	else 
		pc->RightAttack();
}
void AttackCommand::Cancel()
{
}

void EvasionCommand::Execute()
{
	APlayerCharacter* pc = Cast<APlayerCharacter>(_actor);
	pc->Evasion();
}
void EvasionCommand::Cancel()
{
}

void EquipmentCommand::Execute()
{
	APlayerCharacter* pc = Cast<APlayerCharacter>(_actor);
	if (_isLeftEquipment)
		pc->Equipment_Left();
	else
		pc->Equipment_Right();
}
void EquipmentCommand::Cancel()
{
}

void ShowGUICommand::Execute()
{
	APlayerCharaterCtrl* pctrl = Cast<APlayerCharaterCtrl>(_actor); //액터가 아닌 UObject를 받는걸로 변경
	switch(_uiNumber)
	{
	case 0:
		CreateWidget<UGamePauseMenuWidget>(pctrl, pctrl->MenuWidgetClass)->AddToViewport();
		pctrl->SetPause(true);
		pctrl->ChangeInputMode(1);
		break;
	case 1:
		CreateWidget<UGameClearMenuWidget>(pctrl, pctrl->ClearWidgetClass)->AddToViewport();
		pctrl->ChangeInputMode(1);
		break;
	case 2:
		CreateWidget<UGameOverMenuWidget>(pctrl, pctrl->GameOverWidgetClass)->AddToViewport();
		pctrl->ChangeInputMode(1);
		break;
	case 3:
		if (pctrl->isInvenopen)
		{
			pctrl->InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			pctrl->ChangeInputMode(0);
			pctrl->isInvenopen = false;
		}
		else
		{
			pctrl->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			pctrl->ChangeInputMode(2);
			pctrl->isInvenopen = true;
		}
		break;
	}
}
void ShowGUICommand::Cancel()
{
}
