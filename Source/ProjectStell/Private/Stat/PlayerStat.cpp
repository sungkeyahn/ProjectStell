// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/PlayerStat.h"
#include "Stell.h"

UPlayerStat::UPlayerStat()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}
void UPlayerStat::InitializeComponent()
{
	Super::InitializeComponent();
	auto GameInst = Cast<UStell>(UGameplayStatics::GetGameInstance((GetWorld())));
	if (GameInst == nullptr) return;
	CurPlayerStat = GameInst->GetPlayerStatDataTableRow(CurLevel);
}
