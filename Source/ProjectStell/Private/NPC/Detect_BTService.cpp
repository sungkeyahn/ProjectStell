#include "NPC/Detect_BTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "NPC/Enemy.h"
#include "NPC/EnemyCtrl.h"
#include "Player/PlayerCharacter.h"

UDetect_BTService::UDetect_BTService()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}
void UDetect_BTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 500.0f;
	if (nullptr == World) return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,Center,FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APlayerCharacter* p = Cast<APlayerCharacter>(OverlapResult.GetActor());
			if (p && p->GetController()->IsPlayerController())
			{
				//플레이어를 처음 인지하면 일어나는 행위는 스켈레톤 잡몹만 해당하는 기능이다. 
				//설정하기 위하여 블랙보드에 키를 쓰는 방식이 맞는듯?
				//콜백 함수?
				//AEnemy* monster = Cast<AEnemy>(ControllingPawn)->DeadTime;
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyCtrl::TargetActorKey, p);

				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, p->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), p->GetActorLocation(), FColor::Blue, false, 0.27f);
				
				return;
			}
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyCtrl::TargetActorKey, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
