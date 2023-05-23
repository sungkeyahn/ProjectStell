// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Summon.generated.h"

UCLASS()
class PROJECTSTELL_API USummon : public UBTTaskNode
{
	GENERATED_BODY()
public:
	USummon();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	bool IsSummon = false;
	
};
