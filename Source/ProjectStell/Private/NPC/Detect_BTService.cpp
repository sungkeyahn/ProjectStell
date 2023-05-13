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
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyCtrl::TargetActorKey, p);
				Cast<AEnemyCtrl>(OwnerComp.GetAIOwner())->PlayerFind(); //플레이어를 감지했다고 컨트롤러에 전달 

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
