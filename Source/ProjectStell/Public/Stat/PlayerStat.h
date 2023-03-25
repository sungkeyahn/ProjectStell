// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Stat/Stat.h"
#include "PlayerStat.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSTELL_API UPlayerStat : public UStat
{
	GENERATED_BODY()
public:
	UPlayerStat();
protected:
	virtual void InitializeComponent() override;
//데이터 테이블으로 로딩해서 적용시킬 수치들 (비휘발성)
private:
	struct FPlayerStatDataByLevel* CurPlayerStat = nullptr;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurSTR;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurDEX;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurDEF;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurLUCK;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurMaxExp;
//휘발성 데이터



};
