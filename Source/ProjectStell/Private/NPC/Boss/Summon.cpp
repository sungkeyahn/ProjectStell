// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Boss/Summon.h"
#include "NPC/Boss/BossA.h"
#include "NPC/Boss/BossACtrl.h"

USummon::USummon()
{
	IsSummon = false;
}
EBTNodeResult::Type USummon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MonsterCharacter = Cast<ABossA>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MonsterCharacter) return EBTNodeResult::Failed;

	MonsterCharacter->SummonMonster();
	Cast<ABossACtrl>(OwnerComp.GetAIOwner())->SummonAble(false);
	return EBTNodeResult::Succeeded;
}
