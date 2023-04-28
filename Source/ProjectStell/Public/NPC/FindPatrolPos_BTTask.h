#pragma once

#include "ProjectStell.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindPatrolPos_BTTask.generated.h"

UCLASS()
class PROJECTSTELL_API UFindPatrolPos_BTTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UFindPatrolPos_BTTask();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
