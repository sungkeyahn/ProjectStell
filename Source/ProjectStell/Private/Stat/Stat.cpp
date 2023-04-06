// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/Stat.h"
#include "Stell.h"

UStat::UStat()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}
void UStat::InitializeComponent()
{
	Super::InitializeComponent();
	SetStat(FStatStruct().MaxHp);
}
void UStat::SetHp(float newHp)
{
	CurrentHp = newHp;
	OnHpChanged.Broadcast();
	if (CurrentHp < KINDA_SMALL_NUMBER)
	{
		CurrentHp = 0.f;
		OnHpIsZero.Broadcast();
	}
}
void UStat::SetDamage(float NewDamage)
{
	SetHp(FMath::Clamp<float>(CurrentHp - NewDamage, 0.0f, 10));
}
float UStat::GetHpRatio()const
{
	if (CurrentHp<=0.f) return 0.f;
	return (CurrentHp / 10.f);
}
void UStat::SetStat(float newStat)
{
	/*
	auto GameInst = Cast<UStell>(UGameplayStatics::GetGameInstance((GetWorld())));
	if (GameInst == nullptr) return;
	CurHp = GameInst->GetDataTableRow(newStat);*/
	SetHp(newStat);
}




