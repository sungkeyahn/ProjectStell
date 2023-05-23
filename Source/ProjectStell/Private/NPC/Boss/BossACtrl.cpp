// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Boss/BossACtrl.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ABossACtrl::ABossACtrl()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBobj(TEXT("BlackboardData'/Game/1_AI/EnemyBB.EnemyBB'"));
	if (BBobj.Succeeded())
	{
		BBAsset = BBobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTobj(TEXT("BehaviorTree'/Game/1_AI/BossA_BT.BossA_BT'"));
	if (BTobj.Succeeded())
	{
		BTAsset = BTobj.Object;
	}
}
void ABossACtrl::SummonAble(bool b)
{
	if (blackboard->GetValueAsObject(TargetActorKey))
	{
		blackboard->SetValueAsBool(isSummonKey, b);
	}
}
