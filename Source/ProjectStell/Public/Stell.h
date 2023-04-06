// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "Stell.generated.h"

USTRUCT(BlueprintType)
struct FBaseStatData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FBaseStatData():Level(0),MaxHp(0){}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHp;
};
USTRUCT(BlueprintType)
struct FPlayerStatDataByLevel : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float STR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float DEX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float DEF;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float LUCK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxExp;
};
UCLASS()
class PROJECTSTELL_API UStell : public UGameInstance
{
	GENERATED_BODY()
public:
	UStell();
	virtual void Init()override;
//로딩에 쓸 데이터 테이블
private:
	UPROPERTY()
		class UDataTable* StatDataTable;
	UPROPERTY()
		class UDataTable* StatDataTable2;
//데이터 테이블마다 Getter가 필요
public:
	FBaseStatData* GetDataTableRow(int32 Level);
	FPlayerStatDataByLevel* GetPlayerStatDataTableRow(int32 Level);
};
