// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Boss/Skill.h"
#include "NPC/Boss/BossB.h"
#include "NPC/Boss/BossBCtrl.h"

USkill::USkill()
{
	bNotifyTick = true;
	IsSkill = false;
}
EBTNodeResult::Type USkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MonsterCharacter = Cast<ABossB>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MonsterCharacter) return EBTNodeResult::Failed;

	IsSkill = true;
	MonsterCharacter->OnSkillEnd.AddLambda([this]() -> void {IsSkill = false; });
	
	Cast<ABossBCtrl>(OwnerComp.GetAIOwner())->SkillActivate(false);

	/*여기서 스킬 사용*/
	MonsterCharacter->Skill();

	return EBTNodeResult::InProgress;
}
void USkill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSecondes);
	if (!IsSkill)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
