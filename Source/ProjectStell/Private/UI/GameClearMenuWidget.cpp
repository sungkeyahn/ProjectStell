#include "UI/GameClearMenuWidget.h"
#include "Components/Button.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameClearMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UGameClearMenuWidget::OnReturnToTitleClicked);

	ExitGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnExitGame")));
	if (nullptr != ExitGameButton)
		ExitGameButton->OnClicked.AddDynamic(this, &UGameClearMenuWidget::OnExitGameClicked);
}
void UGameClearMenuWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}
void UGameClearMenuWidget::OnExitGameClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
