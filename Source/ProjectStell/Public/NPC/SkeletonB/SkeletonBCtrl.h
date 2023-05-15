// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/EnemyCtrl.h"
#include "SkeletonBCtrl.generated.h"

UCLASS()
class PROJECTSTELL_API ASkeletonBCtrl : public AEnemyCtrl
{
	GENERATED_BODY()
public:
	ASkeletonBCtrl();
	virtual void PlayerFind()override;
};
