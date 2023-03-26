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
//데이터 테이블 행 구조채 
private:
	struct FBaseStatData* CurStat = nullptr;
//현재 스텟 관련
protected:
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 CurLevel;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurHp;
//체력 관련
public:
	FOnHpIsZeroDelegate OnHpIsZero;
	FOnHpChangedDelegate OnHpChanged;
	void SetHp(float newHp);
	void SetDamage(float NewDamage);
	float GetHpRatio()const;
//레벨 관련
	virtual void SetLevel(int32 newLevel);
	int32 GetLevel()const;
};