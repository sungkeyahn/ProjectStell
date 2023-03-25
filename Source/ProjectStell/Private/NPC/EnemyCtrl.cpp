// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/EnemyCtrl.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AEnemyCtrl::HomePosKey(TEXT("HomePos")); //스폰위치도 겸임 임
const FName AEnemyCtrl::PatrolPosKey(TEXT("PatrolPos"));
const FName AEnemyCtrl::TargetActorKey(TEXT("TargetActor"));
AEnemyCtrl::AEnemyCtrl()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBobj(TEXT("BlackboardData'/Game/1_AI/EnemyBB.EnemyBB'"));
	if (BBobj.Succeeded())
	{
		BBAsset = BBobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTobj(TEXT("BehaviorTree'/Game/1_AI/EnemyBT.EnemyBT'"));
	if (BTobj.Succeeded())
	{
		BTAsset = BTobj.Object;
	}
}
void AEnemyCtrl::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}
void AEnemyCtrl::RunBT()
{
	UBlackboardComponent* blackboard = Blackboard;
	if (UseBlackboard(BBAsset, blackboard))
	{
		blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset)) {}
	}
}
void AEnemyCtrl::StopBT()
{
	auto BTcomp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTcomp != nullptr)
		BTcomp->StopTree(EBTStopMode::Safe);
}
