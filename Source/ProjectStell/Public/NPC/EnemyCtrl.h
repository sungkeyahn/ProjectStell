// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "AIController.h"
#include "EnemyCtrl.generated.h"

UCLASS()
class PROJECTSTELL_API AEnemyCtrl : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyCtrl();
	virtual void OnPossess(APawn* aPawn)override;
protected:
	class UBlackboardComponent* blackboard;
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	UPROPERTY()
		class UBlackboardData* BBAsset;
public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetActorKey;
	static const FName isGuardKey;
	static const FName isSummonKey;
	static const FName isSkillKey;

public:
	void RunBT();
	void StopBT();

public:
	virtual void PlayerFind();

};
