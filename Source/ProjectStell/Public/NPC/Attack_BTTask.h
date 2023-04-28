#pragma once

#include "ProjectStell.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Attack_BTTask.generated.h"

UCLASS()
class PROJECTSTELL_API UAttack_BTTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UAttack_BTTask();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes) override;
private:
	bool IsAttacking = false;
	/*
	* 공격 패턴에 대하여 어떤 방식으로 결정할 것인가?
	*/


};
