// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Stat/Stat.h"
#include "PlayerStat.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSpChangedDelegate);
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSTELL_API UPlayerStat : public UStat
{
	GENERATED_BODY()
public:
	UPlayerStat();
protected:
	virtual void InitializeComponent() override;
//디폴트 데이터 관련
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
		float MaxSp=100.f;
protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentSp = -1;
//데이터 상호작용 관련
private:
	void SetSp(float newSp);
public:
	FOnSpChangedDelegate OnSpChanged;
	void UseStamina(float NewStamina);
	float GetSpRatio()const;

};
