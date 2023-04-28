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
//초기화 관련
public:
	UStat();
protected:
	virtual void InitializeComponent() override;
//디폴트 데이터 관련 
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
		float MaxHp = 10;
//현제 데이터 관련
protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentHp = -1;
//데이터 상호작용 관련
private:
	void SetHp(float newHp);
public:
	FOnHpIsZeroDelegate OnHpIsZero;
	FOnHpChangedDelegate OnHpChanged;
	void SetDamage(float NewDamage);
	float GetHpRatio()const;
};