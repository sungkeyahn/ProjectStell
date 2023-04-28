// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "BehaviorTree/BTService.h"
#include "Detect_BTService.generated.h"

UCLASS()
class PROJECTSTELL_API UDetect_BTService : public UBTService
{
	GENERATED_BODY()
public:
	UDetect_BTService();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
