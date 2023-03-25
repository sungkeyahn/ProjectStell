// Fill out your copyright notice in the Description page of Project Settings.


#include "Stell.h"

UStell::UStell()
{
	//게임에서 사용될 모든 데이터테이블에 대한 애셋을 참조하는 부분 
	FString path = TEXT("DataTable'/Game/1_DataTable/BaseStatData.BaseStatData'");
	static ConstructorHelpers::FObjectFinder<UDataTable>d(*path);
	if (d.Succeeded()) StatDataTable = d.Object;
	if (StatDataTable->GetRowMap().Num() < 0) return;

	path = TEXT("DataTable'/Game/1_DataTable/PlayerStatDataByLevel.PlayerStatDataByLevel'");
	static ConstructorHelpers::FObjectFinder<UDataTable>dd(*path);
	if (dd.Succeeded()) StatDataTable2 = dd.Object;
	if (StatDataTable2->GetRowMap().Num() < 0) return;



}
void UStell::Init()
{
	Super::Init();
}
FBaseStatData* UStell::GetDataTableRow(int32 Level)
{
	return StatDataTable->FindRow<FBaseStatData>(*FString::FromInt(Level), TEXT(""));
}
FPlayerStatDataByLevel* UStell::GetPlayerStatDataTableRow(int32 Level)
{
	return StatDataTable2->FindRow<FPlayerStatDataByLevel>(*FString::FromInt(Level), TEXT(""));
}
