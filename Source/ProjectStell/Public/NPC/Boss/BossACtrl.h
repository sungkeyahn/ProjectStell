// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/EnemyCtrl.h"
#include "BossACtrl.generated.h"

UCLASS()
class PROJECTSTELL_API ABossACtrl : public AEnemyCtrl
{
	GENERATED_BODY()
public:
	ABossACtrl();
	
	//소환가능하다는 것을 블랙보드에 알리는 함수 isSummonKey값을 변경하는 함수
	void SummonAble(bool b);

};
