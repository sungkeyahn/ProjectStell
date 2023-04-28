#pragma once

#include "ProjectStell.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsInAttackRange_BTDecorator.generated.h"

UCLASS()
class PROJECTSTELL_API UIsInAttackRange_BTDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	UIsInAttackRange_BTDecorator();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
