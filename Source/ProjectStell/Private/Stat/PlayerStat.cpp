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
}
void UPlayerStat::SetStat(float newStat)
{
	Super::SetStat(newStat);
}
/*
void UPlayerStat::SetLevel(int32 newLevel)
{
	Super::SetLevel(newLevel);
	auto GameInst = Cast<UStell>(UGameplayStatics::GetGameInstance((GetWorld())));
	if (GameInst == nullptr) return;
	CurPlayerStat = GameInst->GetPlayerStatDataTableRow(newLevel);
	if (CurPlayerStat != nullptr)
	{
		CurSTR = CurPlayerStat->STR;
		CurDEX = CurPlayerStat->DEX;
		CurDEF = CurPlayerStat->DEF;
		CurLUCK = CurPlayerStat->LUCK;
		CurMaxExp = CurPlayerStat->MaxExp;
	}
}*/

