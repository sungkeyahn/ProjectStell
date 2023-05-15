#include "UI/GameOverMenuWidget.h"
#include "Components/Button.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameOverMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UGameOverMenuWidget::OnReturnToTitleClicked);

	ReStartLevelButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReStartLevel")));
	if (nullptr != ReStartLevelButton)
		ReStartLevelButton->OnClicked.AddDynamic(this, &UGameOverMenuWidget::OnReStartLevelClicked);

	ExitGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnExitGame")));
	if (nullptr != ExitGameButton)
		ExitGameButton->OnClicked.AddDynamic(this, &UGameOverMenuWidget::OnExitGameClicked);
}

void UGameOverMenuWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

void UGameOverMenuWidget::OnReStartLevelClicked()
{
	FName name = FName(UGameplayStatics::GetCurrentLevelName(GetWorld(), true));
	UGameplayStatics::OpenLevel(GetWorld(), name);
}

void UGameOverMenuWidget::OnExitGameClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
