#include "NPC/Attack_BTTask.h"
#include "NPC/Enemy.h"
#include "NPC/EnemyCtrl.h"

UAttack_BTTask::UAttack_BTTask()
{
	bNotifyTick = true;
	IsAttacking = false;
}
void UAttack_BTTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSecondes);
	if (!IsAttacking)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
EBTNodeResult::Type UAttack_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MonsterCharacter = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MonsterCharacter)
		return EBTNodeResult::Failed;

	/*���� ������ �����ϴ� ������ �ʿ�*/
	MonsterCharacter->Attack(0);
	IsAttacking = true;
	MonsterCharacter->OnAttackEnd.AddLambda([this]() -> void {IsAttacking = false;});
	return EBTNodeResult::Succeeded;
}
