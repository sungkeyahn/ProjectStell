// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Components/ActorComponent.h"
#include "Stat.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHpChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSTELL_API UStat : public UActorComponent
{
	GENERATED_BODY()
//초기화
public:
	UStat();
protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
//해당 클래스가 받을 데이터테이블의 행을 나타내는 구조채 
private:
	struct FBaseStatData* CurStat = nullptr;
//게임에 존재하는 모든 물체가 가지고 있을 스텟들
protected:
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 CurLevel;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurHp;
//체력 관련
public:
	FOnHpIsZeroDelegate OnCharacterHpIsZero;
	FOnHpChangedDelegate OnCharacterHpChanged;
	void SetHp(float newHp);
	float GetHpRatio()const;
//레벨 관련
	virtual void SetLevel(int32 newLevel);
	int32 GetLevel()const;
};