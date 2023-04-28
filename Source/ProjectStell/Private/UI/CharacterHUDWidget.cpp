// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Stat/PlayerStat.h"

void UCharacterHUDWidget::BindCharacterStat(UPlayerStat* Stat)
{
	if(nullptr == Stat)return;
	PlayerStat = Stat;
	Stat->OnHpChanged.AddUObject(this, &UCharacterHUDWidget::UpdateCharacterStat);
	Stat->OnSpChanged.AddUObject(this, &UCharacterHUDWidget::UpdateCharacterStat);
}

void UCharacterHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	if(nullptr == HPBar) return;
	SPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbSP")));
	if (nullptr == HPBar) return;
}

void UCharacterHUDWidget::UpdateCharacterStat()
{
	if (PlayerStat.IsValid())
	{
		HPBar->SetPercent(PlayerStat->GetHpRatio());
		SPBar->SetPercent(PlayerStat->GetSpRatio());
	}
}
