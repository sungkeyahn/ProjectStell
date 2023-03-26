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
//�ʱ�ȭ
public:
	UStat();
protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
//������ ���̺� �� ����ä 
private:
	struct FBaseStatData* CurStat = nullptr;
//���� ���� ����
protected:
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 CurLevel;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurHp;
//ü�� ����
public:
	FOnHpIsZeroDelegate OnHpIsZero;
	FOnHpChangedDelegate OnHpChanged;
	void SetHp(float newHp);
	void SetDamage(float NewDamage);
	float GetHpRatio()const;
//���� ����
	virtual void SetLevel(int32 newLevel);
	int32 GetLevel()const;
};