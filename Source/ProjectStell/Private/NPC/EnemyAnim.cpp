// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/EnemyAnim.h"
UEnemyAnim::UEnemyAnim()
{
	CurrentPawnSpeed = 0.0f;
	IsDead = false;
}
void UEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto pawn = TryGetPawnOwner();
	if (::IsValid(pawn))
	{
		if (!IsDead)
		{
			CurrentPawnSpeed = pawn->GetVelocity().Size();

			auto character = Cast<ACharacter>(pawn); //불완전 코드
			if (character)
			{
				IsInAir = character->GetMovementComponent()->IsFalling();
			}
		}
	}
}
void UEnemyAnim::PlayEnemyMontage(class UAnimMontage* montage)
{
	if (!IsDead)
		Montage_Play(montage, 1.0f);
}
void UEnemyAnim::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}
void UEnemyAnim::AnimNotify_HitEndCheck()
{
	OnHitEndCheck.Broadcast();
}

