#pragma once

#include "ProjectStell.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TurnToTarget_BTTask.generated.h"

UCLASS()
class PROJECTSTELL_API UTurnToTarget_BTTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTurnToTarget_BTTask();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
