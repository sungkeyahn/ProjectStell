// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/Stat.h"
#include "Stell.h"

UStat::UStat()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	CurLevel = 1;
}
void UStat::BeginPlay()
{
	Super::BeginPlay();	
}
void UStat::InitializeComponent()
{
	Super::InitializeComponent();
	SetLevel(CurLevel);
}
void UStat::SetHp(float newHp)
{
	CurHp = newHp;
	OnHpChanged.Broadcast();
	if (CurHp < KINDA_SMALL_NUMBER)
	{
		CurHp = 0.f;
		OnHpIsZero.Broadcast();
	}
}
void UStat::SetDamage(float NewDamage)
{
	if (nullptr == CurStat)return;
	SetHp(FMath::Clamp<float>(CurHp - NewDamage, 0.0f, CurStat->MaxHp));
}
float UStat::GetHpRatio()const
{
	if (CurStat == nullptr) return 0.f;
	if (CurStat->MaxHp < KINDA_SMALL_NUMBER) return 0.f;
	return (CurHp / CurStat->MaxHp);
}
void UStat::SetLevel(int32 newLevel)
{
	auto GameInst = Cast<UStell>(UGameplayStatics::GetGameInstance((GetWorld())));
	if (GameInst == nullptr) return;
	CurStat = GameInst->GetDataTableRow(newLevel);
	if (CurStat != nullptr)
	{
		CurLevel = newLevel;
		SetHp(CurStat->MaxHp);
	}
}
int32 UStat::GetLevel()const
{
	return CurLevel;
}



