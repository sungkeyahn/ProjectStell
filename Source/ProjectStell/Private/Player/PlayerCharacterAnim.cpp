// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacterAnim.h"
#include "Weapon/Weapon.h"

UPlayerCharacterAnim::UPlayerCharacterAnim()
{
	CurrentPawnSpeed = 0.0f;
	IsDead = false;
}
void UPlayerCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	auto pawn = TryGetPawnOwner();
	if (!::IsValid(pawn)) return;
	if (!IsDead)
	{
		CurrentPawnSpeed = pawn->GetVelocity().Size();
	}
}
void UPlayerCharacterAnim::SetNextAttack(UAnimMontage* nextMontage, float playSpeed)
{
	NextMontage = nextMontage;
	NextMontagePlaySpeed = playSpeed;
}
void UPlayerCharacterAnim::AnimNotify_HitCheck()
{
	OnAttackHitCheck.Broadcast();
}
void UPlayerCharacterAnim::AnimNotify_ConnectCheck()
{
	OnAttackTimeCheck.Broadcast();
	//if (NextMontage == nullptr)
	//	  OnAttackEndCheck.Broadcast();
	//else
	//PlayPlayerMontage(NextMontage, NextMontagePlaySpeed);
	//SetNextAttack(nullptr, 1.f);
}

void UPlayerCharacterAnim::AnimNotify_EndCheck()
{
	OnAttackEndCheck.Broadcast();
}
void UPlayerCharacterAnim::AnimNotify_DashStart()
{
	OnDashStart.Broadcast();
}
void UPlayerCharacterAnim::AnimNotify_DashEnd()
{
	OnDashEnd.Broadcast();
}
void UPlayerCharacterAnim::PlayPlayerMontage(UAnimMontage* montage, float PalySpeed)
{
	if (montage == nullptr)return;
		Montage_Play(montage, PalySpeed);
}
void UPlayerCharacterAnim::StopPlayerMontage(UAnimMontage* montage)
{
	if (montage == nullptr)return;
	if (Montage_IsPlaying(montage))
		Montage_Stop(0.0f, montage);

}


