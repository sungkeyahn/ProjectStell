#include "NPC/TurnToTarget_BTTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPC/Enemy.h"
#include "NPC/EnemyCtrl.h"
#include "Player/PlayerCharacter.h"

UTurnToTarget_BTTask::UTurnToTarget_BTTask()
{
    NodeName = TEXT("Turn");
}
EBTNodeResult::Type UTurnToTarget_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto MonsterCharacter = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MonsterCharacter)
		return EBTNodeResult::Failed;
	auto Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyCtrl::TargetActorKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - MonsterCharacter->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	MonsterCharacter->SetActorRotation(FMath::RInterpTo(MonsterCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));
	return EBTNodeResult::Succeeded;
}
