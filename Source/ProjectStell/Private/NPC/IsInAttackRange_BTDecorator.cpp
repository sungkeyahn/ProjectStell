#include "NPC/IsInAttackRange_BTDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPC/Enemy.h"
#include "NPC/EnemyCtrl.h"

UIsInAttackRange_BTDecorator::UIsInAttackRange_BTDecorator()
{
	NodeName = TEXT("CanAttack");
}
bool UIsInAttackRange_BTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return false;

	auto Target = Cast<AEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyCtrl::TargetActorKey));
	if (nullptr == Target) return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 50.f); //ControllingPawn->CurrentAttackinfo.AttackRange
	return bResult;
}
