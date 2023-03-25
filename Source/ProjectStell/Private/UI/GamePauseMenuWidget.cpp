// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GamePauseMenuWidget.h"
#include "Components/Button.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Kismet/KismetSystemLibrary.h"

void UGamePauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
		ResumeButton->OnClicked.AddDynamic(this, &UGamePauseMenuWidget::OnResumeClicked);

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UGamePauseMenuWidget::OnReturnToTitleClicked);

	ExitGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnExitGame")));
	if (nullptr != ExitGameButton)
		ExitGameButton->OnClicked.AddDynamic(this, &UGamePauseMenuWidget::OnExitGameClicked);
}
void UGamePauseMenuWidget::OnResumeClicked()
{
	auto playerctrl = Cast<APlayerCharaterCtrl>(GetOwningPlayer());
	if(nullptr == playerctrl)return;

	RemoveFromParent();
	playerctrl->ChangeInputMode(true);
	playerctrl->SetPause(false);
}
void UGamePauseMenuWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}
void UGamePauseMenuWidget::OnExitGameClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
