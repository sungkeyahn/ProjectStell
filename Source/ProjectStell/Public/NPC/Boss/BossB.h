// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/Enemy.h"
#include "BossB.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSkillEndDelegate);

UCLASS()
class PROJECTSTELL_API ABossB : public AEnemy
{
	GENERATED_BODY()
public:
	ABossB();
protected:
	virtual void BeginPlay() override;
	//스킬 관련 -> 점프공격 skill로 분류해야할듯 
private:
	FTimerHandle SkillCoolTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
		float SkillCoolTime = 15.f;
	float CurCoolTime;
	void SkillCoolTimer(); //isSummonAble =true; 쿨타임 차면 소환 가능 변수 체크
	void Skill_JumpAttack(); //점프 공격에 대한 로직을 가지고 있는 함수 
	UFUNCTION()
		void OnSkillEndFun(class UAnimMontage* Montage, bool bInterrupted);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
		FMonsterAttackInfoStruct SkillAttackInfo;

public:
	FOnSkillEndDelegate OnSkillEnd;
	void Skill(); //테스크에서 호출할 함수 
};
