// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/EnemyCtrl.h"
#include "SkeletonACtrl.generated.h"

UCLASS()
class PROJECTSTELL_API ASkeletonACtrl : public AEnemyCtrl
{
	GENERATED_BODY()
public:
	ASkeletonACtrl();
	virtual void PlayerFind()override;
	virtual void Guard();
};
