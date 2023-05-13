// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/SkeletonA/Guard.h"
#include "NPC/SkeletonA/SkeletonA.h"
#include "NPC/SkeletonA/SkeletonACtrl.h"

UGuard::UGuard()
{
	bNotifyTick = true;
	IsGuard = false;
}
EBTNodeResult::Type UGuard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MonsterCharacter = Cast<ASkeletonA>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MonsterCharacter) return EBTNodeResult::Failed;
	IsGuard = true;
	MonsterCharacter->OnGuardEnd.AddLambda([this]() -> void {IsGuard = false; });
	MonsterCharacter->Guard(); 

	return EBTNodeResult::InProgress;
}

void UGuard::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSecondes);
	if (!IsGuard)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
