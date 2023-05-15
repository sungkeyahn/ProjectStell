// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/SkeletonB/SkeletonBCtrl.h"
#include "NPC/SkeletonB/SkeletonB.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
ASkeletonBCtrl::ASkeletonBCtrl()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBobj(TEXT("BlackboardData'/Game/1_AI/EnemyBB.EnemyBB'"));
	if (BBobj.Succeeded())
	{
		BBAsset = BBobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTobj(TEXT("BehaviorTree'/Game/1_AI/SkeletonB_BT.SkeletonB_BT'"));
	if (BTobj.Succeeded())
	{
		BTAsset = BTobj.Object;
	}
}

void ASkeletonBCtrl::PlayerFind()
{
	Cast<ASkeletonB>(GetPawn())->WakeUp();
}


