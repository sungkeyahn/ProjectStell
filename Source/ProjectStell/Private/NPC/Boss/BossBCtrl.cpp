// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Boss/BossBCtrl.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ABossBCtrl::ABossBCtrl()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBobj(TEXT("BlackboardData'/Game/1_AI/EnemyBB.EnemyBB'"));
	if (BBobj.Succeeded())
	{
		BBAsset = BBobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTobj(TEXT("BehaviorTree'/Game/1_AI/BossB_BT.BossB_BT'"));
	if (BTobj.Succeeded())
	{
		BTAsset = BTobj.Object;
	}
}

void ABossBCtrl::SkillActivate(bool b)
{
	blackboard->SetValueAsBool(isSkillKey, b);
}
