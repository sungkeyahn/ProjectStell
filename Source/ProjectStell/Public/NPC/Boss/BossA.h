// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/Enemy.h"
#include "BossA.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnSummonEndDelegate); //소환 노드에 종료 신호 전달

UCLASS()
class PROJECTSTELL_API ABossA : public AEnemy
{
	GENERATED_BODY()
public:
	ABossA();
protected:
	virtual void BeginPlay() override;
//스킬 관련 -> 소환
private:
	FTimerHandle SkillCoolTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
		float SkillCoolTime = 15.f;
	float CurCoolTime;
	void SkillCoolTimer(); //isSummonAble =true; 쿨타임 차면 소환 가능 변수 체크
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
		TSubclassOf<AEnemy> SpawnMonsters;
	void SpawnEnemy(); //소환 기능만 담당 -> 테스크에서 호출하게되듯?

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* SkillMontage;
public:
	//FOnSummonEndDelegate OnSummonEnd;
	void SummonMonster(); //스킬 애니메이션 실행 -> 스폰 -> 탈진-> 후 처리 까지 
};
