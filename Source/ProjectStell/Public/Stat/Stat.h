// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProjectStell.h"
#include "Components/ActorComponent.h"
#include "Stat.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FStatStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHp = 10;
};

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
	virtual void InitializeComponent() override;
	virtual void SetStat(float newStat);
//���� ����
protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentHp = -1;
//��� ����
public:
	FOnHpIsZeroDelegate OnHpIsZero;
	FOnHpChangedDelegate OnHpChanged;
	void SetHp(float newHp);
	void SetDamage(float NewDamage);
	float GetHpRatio()const;
};