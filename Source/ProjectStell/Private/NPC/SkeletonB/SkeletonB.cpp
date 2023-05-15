// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/SkeletonB/SkeletonB.h"
#include "NPC/SkeletonB/SkeletonBCtrl.h"
#include "NPC/EnemyAnim.h"
#include "Stat/Stat.h"

#include "DrawDebugHelpers.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Player/ComboManager.h"

ASkeletonB::ASkeletonB()
{}
void ASkeletonB::WakeUp()
{
	anim->IsSleep = false;
}
