// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/EnemyAnim.h"
#include "NPC/Enemy.h"
UEnemyAnim::UEnemyAnim()
{}
void UEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto pawn = TryGetPawnOwner();
	if (::IsValid(pawn))
		if (!IsDead)
			CurrentPawnSpeed = pawn->GetVelocity().Size();
}
void UEnemyAnim::PlayEnemyMontage(class UAnimMontage* montage)
{
	if (!IsDead)
		Montage_Play(montage, 1.0f);
}
void UEnemyAnim::AnimNotify_HitCheck()
{
	OnAttackHitCheck.Broadcast();
}

