#include "NPC/SkeletonA/SkeletonACtrl.h"
#include "NPC/SkeletonA/SkeletonA.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ASkeletonACtrl::ASkeletonACtrl()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBobj(TEXT("BlackboardData'/Game/1_AI/EnemyBB.EnemyBB'"));
	if (BBobj.Succeeded())
	{
		BBAsset = BBobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTobj(TEXT("BehaviorTree'/Game/1_AI/SkeletonA_BT.SkeletonA_BT'"));
	if (BTobj.Succeeded())
	{
		BTAsset = BTobj.Object;
	}
}
void ASkeletonACtrl::PlayerFind()
{
	Cast<ASkeletonA>(GetPawn())->WakeUp();
}
void ASkeletonACtrl::Guard()
{
	if (UseBlackboard(BBAsset, blackboard))
		blackboard->SetValueAsBool(isGuardKey, true);
	Cast<ASkeletonA>(GetPawn())->OnGuardEnd.AddLambda([this]() -> void {blackboard->SetValueAsBool(isGuardKey, false); });
}


